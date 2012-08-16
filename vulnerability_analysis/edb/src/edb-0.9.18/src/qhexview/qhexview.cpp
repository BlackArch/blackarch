/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu

Copyright (C) 2010        Hugues Bruant
                          hugues.bruant@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "qhexview.h"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QFontDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QScrollBar>
#include <QSignalMapper>
#include <QTextStream>
#include <QtGlobal>
#include <QDebug>

#include <cctype>
#include <climits>
#include <memory>

namespace {
	struct show_separator_tag {};

	template <class T, size_t N>
	struct address_format {};

	template <class T>
	struct address_format<T, 4> {
		static QString format_address(T address, const show_separator_tag&) {
			static char buffer[10];
			qsnprintf(buffer, sizeof(buffer), "%04x:%04x", (address >> 16) & 0xffff, address & 0xffff);
			return QString::fromLocal8Bit(buffer);
		}

		static QString format_address(T address) {
			static char buffer[9];
			qsnprintf(buffer, sizeof(buffer), "%04x%04x", (address >> 16) & 0xffff, address & 0xffff);
			return QString::fromLocal8Bit(buffer);
		}
	};

	template <class T>
	struct address_format<T, 8> {
		static QString format_address(T address, const show_separator_tag&) {
			static char buffer[19];
			qsnprintf(buffer, sizeof(buffer), "%08x:%08x", (address >> 32) & 0xffffffff, address & 0xffffffff);
			return QString::fromLocal8Bit(buffer);
		}

		static QString format_address(T address) {
			static char buffer[18];
			qsnprintf(buffer, sizeof(buffer), "%08x%08x", (address >> 32) & 0xffffffff, address & 0xffffffff);
			return QString::fromLocal8Bit(buffer);
		}
	};

	template <class T>
	QString format_address(T address, bool show_separator) {
		if(show_separator) return address_format<T, sizeof(T)>::format_address(address, show_separator_tag());
		else               return address_format<T, sizeof(T)>::format_address(address);
	}
	
	//------------------------------------------------------------------------------
	// Name: is_printable(unsigned char ch)
	// Desc: determines if a character has a printable ascii symbol
	//------------------------------------------------------------------------------
	bool is_printable(unsigned char ch) {

		// if it's standard ascii use isprint/isspace, otherwise go with our observations
		if(ch < 0x80) {
			return std::isprint(ch) || std::isspace(ch);
		} else {
			return (ch & 0xff) >= 0xa0;
		}
	}
	
	//------------------------------------------------------------------------------
	// Name: add_toggle_action_to_menu(QMenu *menu, const QString &caption, bool checked, QObject *receiver, const char *slot)
	// Desc: convenience function used to add a checkable menu item to the context menu
	//------------------------------------------------------------------------------
	QAction *add_toggle_action_to_menu(QMenu *menu, const QString &caption, bool checked, QObject *receiver, const char *slot) {
		QAction *const action = new QAction(caption, menu);
    	action->setCheckable(true);
    	action->setChecked(checked);
		menu->addAction(action);
		QObject::connect(action, SIGNAL(toggled(bool)), receiver, slot);
		return action;
	}
}

//------------------------------------------------------------------------------
// Name: QHexView(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
QHexView::QHexView(QWidget *parent) : QAbstractScrollArea(parent),
		row_width_(16), word_width_(1), address_color_(Qt::red),
		show_hex_(true), show_ascii_(true), show_address_(true),
		show_comments_(true), origin_(0), address_offset_(0),
		selection_start_(-1), selection_end_(-1),
		highlighting_(Highlighting_None), even_word_(Qt::blue),
		non_printable_text_(Qt::red), unprintable_char_('.'), show_line1_(true),
		show_line2_(true), show_line3_(true), show_address_separator_(true) {

	// default to a simple monospace font
	setFont(QFont("Monospace", 8));

	setShowAddressSeparator(true);
}

//------------------------------------------------------------------------------
// Name: ~QHexView()
// Desc:
//------------------------------------------------------------------------------
QHexView::~QHexView() {
}

//------------------------------------------------------------------------------
// Name: setShowAddressSeparator(bool value)
// Desc:
//------------------------------------------------------------------------------
void QHexView::setShowAddressSeparator(bool value) {
	show_address_separator_ = value;
	updateScrollbars();
}

//------------------------------------------------------------------------------
// Name: formatAddress(address_t address)
// Desc:
//------------------------------------------------------------------------------
QString QHexView::formatAddress(address_t address) {
	return format_address(address, show_address_separator_);
}

//------------------------------------------------------------------------------
// Name: repaint()
// Desc:
//------------------------------------------------------------------------------
void QHexView::repaint() {
	viewport()->repaint();
}

//------------------------------------------------------------------------------
// Name: dataSize() const
// Desc: returns how much data we are viewing
//------------------------------------------------------------------------------
int QHexView::dataSize() const {
	return data_ ? data_->size() : 0;
}

//------------------------------------------------------------------------------
// Name: setFont(const QFont &f)
// Desc: overloaded version of setFont, calculates font metrics for later
//------------------------------------------------------------------------------
void QHexView::setFont(const QFont &f) {

	// recalculate all of our metrics/offsets
	const QFontMetrics fm(f);
	font_width_  = fm.width('X');
	font_height_ = fm.height();


	updateScrollbars();

	// TODO: assert that we are using a fixed font & find out if we care?
	QAbstractScrollArea::setFont(f);
}

//------------------------------------------------------------------------------
// Name: createStandardContextMenu()
// Desc: creates the 'standard' context menu for the widget
//------------------------------------------------------------------------------
QMenu *QHexView::createStandardContextMenu() {

	QMenu *const menu = new QMenu(this);

	menu->addAction(tr("Set &Font"), this, SLOT(mnuSetFont()));
	menu->addSeparator();
	add_toggle_action_to_menu(menu, tr("Show A&ddress"), show_address_, this, SLOT(setShowAddress(bool)));
	add_toggle_action_to_menu(menu, tr("Show &Hex"), show_hex_, this, SLOT(setShowHexDump(bool)));
	add_toggle_action_to_menu(menu, tr("Show &Ascii"), show_ascii_, this, SLOT(setShowAsciiDump(bool)));
	add_toggle_action_to_menu(menu, tr("Show &Comments"), show_comments_, this, SLOT(setShowComments(bool)));

	QSignalMapper *wordWidthMapper = new QSignalMapper(menu);

	QMenu *const wordMenu = new QMenu(tr("Set Word Width"), menu);
	QAction *const a1 = add_toggle_action_to_menu(wordMenu, tr("1 Byte"), word_width_ == 1, wordWidthMapper, SLOT(map()));
	QAction *const a2 = add_toggle_action_to_menu(wordMenu, tr("2 Bytes"), word_width_ == 2, wordWidthMapper, SLOT(map()));
	QAction *const a3 = add_toggle_action_to_menu(wordMenu, tr("4 Bytes"), word_width_ == 4, wordWidthMapper, SLOT(map()));
	QAction *const a4 = add_toggle_action_to_menu(wordMenu, tr("8 Bytes"), word_width_ == 8, wordWidthMapper, SLOT(map()));

	wordWidthMapper->setMapping(a1, 1);
	wordWidthMapper->setMapping(a2, 2);
	wordWidthMapper->setMapping(a3, 4);
	wordWidthMapper->setMapping(a4, 8);

	connect(wordWidthMapper, SIGNAL(mapped(int)), SLOT(setWordWidth(int)));

	QSignalMapper *rowWidthMapper = new QSignalMapper(menu);

	QMenu *const rowMenu = new QMenu(tr("Set Row Width"), menu);
	QAction *const a5 = add_toggle_action_to_menu(rowMenu, tr("1 Word"), row_width_ == 1, rowWidthMapper, SLOT(map()));
	QAction *const a6 = add_toggle_action_to_menu(rowMenu, tr("2 Words"), row_width_ == 2, rowWidthMapper, SLOT(map()));
	QAction *const a7 = add_toggle_action_to_menu(rowMenu, tr("4 Words"), row_width_ == 4, rowWidthMapper, SLOT(map()));
	QAction *const a8 = add_toggle_action_to_menu(rowMenu, tr("8 Words"), row_width_ == 8, rowWidthMapper, SLOT(map()));
	QAction *const a9 = add_toggle_action_to_menu(rowMenu, tr("16 Words"), row_width_ == 16, rowWidthMapper, SLOT(map()));

	rowWidthMapper->setMapping(a5, 1);
	rowWidthMapper->setMapping(a6, 2);
	rowWidthMapper->setMapping(a7, 4);
	rowWidthMapper->setMapping(a8, 8);
	rowWidthMapper->setMapping(a9, 16);

	connect(rowWidthMapper, SIGNAL(mapped(int)), SLOT(setRowWidth(int)));

	menu->addSeparator();
	menu->addMenu(wordMenu);
	menu->addMenu(rowMenu);

	menu->addSeparator();
	menu->addAction(tr("&Copy Selection To Clipboard"), this, SLOT(mnuCopy()));

	return menu;
}

//------------------------------------------------------------------------------
// Name: contextMenuEvent(QContextMenuEvent *event)
// Desc: default context menu event, simply shows standard menu
//------------------------------------------------------------------------------
void QHexView::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *const menu = createStandardContextMenu();
	menu->exec(event->globalPos());
	delete menu;
}

//------------------------------------------------------------------------------
// Name: mnuCopy()
// Desc:
//------------------------------------------------------------------------------
void QHexView::mnuCopy() {
	if(hasSelectedText()) {

		QString s;
		QTextStream ss(&s);

		// current actual offset (in bytes)

		const int chars_per_row = bytesPerRow();
		unsigned int offset = verticalScrollBar()->value() * chars_per_row;

		if(origin_ != 0) {
			if(offset > 0) {
				offset += origin_;
				offset -= chars_per_row;
			}
		}

		const unsigned int end   = qMax(selection_start_, selection_end_);
		const unsigned int start = qMin(selection_start_, selection_end_);
		const int data_size      = dataSize();

		// offset now refers to the first visible byte
		while(offset < end) {

			if((offset + chars_per_row) > start) {

				data_->seek(offset);
				const QByteArray row_data = data_->read(chars_per_row);

				if(!row_data.isEmpty()) {
					if(show_address_) {
						const address_t address_rva = address_offset_ + offset;
						const QString addressBuffer = formatAddress(address_rva);
						ss << addressBuffer << '|';
					}

					if(show_hex_) {
						drawHexDumpToBuffer(ss, offset, data_size, row_data);
						ss << "|";
					}

					if(show_ascii_) {
						drawAsciiDumpToBuffer(ss, offset, data_size, row_data);
						ss << "|";
					}

					if(show_comments_ && comment_server_) {
						drawCommentsToBuffer(ss, offset, data_size);
					}
				}

				ss << "\n";
			}
			offset += chars_per_row;
		}

		QApplication::clipboard()->setText(s);
		QApplication::clipboard()->setText(s, QClipboard::Selection);
	}
}

//------------------------------------------------------------------------------
// Name: mnuSetFont()
// Desc: slot used to set the font of the widget based on dialog selector
//------------------------------------------------------------------------------
void QHexView::mnuSetFont() {
    setFont(QFontDialog::getFont(0, font(), this));
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc: clears all data from the view
//------------------------------------------------------------------------------
void QHexView::clear() {
	if(data_ != 0) {
		data_.clear();
	}

	repaint();
}

//------------------------------------------------------------------------------
// Name: hasSelectedText() const
// Desc: returns true if any text is selected
//------------------------------------------------------------------------------
bool QHexView::hasSelectedText() const {
	return !(selection_start_ == -1 || selection_end_ == -1);
}

//------------------------------------------------------------------------------
// Name: isInViewableArea(int index) const
// Desc: returns true if the word at the given index is in the viewable area
//------------------------------------------------------------------------------
bool QHexView::isInViewableArea(int index) const {

	const int firstViewableWord = verticalScrollBar()->value() * row_width_;
	const int viewableLines     = viewport()->height() / font_height_;
	const int viewableWords     = viewableLines * row_width_;
	const int lastViewableWord  = firstViewableWord + viewableWords;

	return index >= firstViewableWord && index < lastViewableWord;
}

//------------------------------------------------------------------------------
// Name: keyPressEvent(QKeyEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QHexView::keyPressEvent(QKeyEvent *event) {
	if(event->modifiers() & Qt::ControlModifier) {
		switch(event->key()) {
		case Qt::Key_A:
			selectAll();
			repaint();
			break;
		case Qt::Key_Home:
			scrollTo(0);
			break;
		case Qt::Key_End:
			scrollTo(dataSize() - bytesPerRow());
			break;
		case Qt::Key_Down:

			do {
				int offset = verticalScrollBar()->value() * bytesPerRow();

				if(origin_ != 0) {
					if(offset > 0) {
						offset += origin_;
						offset -= bytesPerRow();
					}
				}

				if(offset + 1 < dataSize()) {
					scrollTo(offset + 1);
				}
			} while(0);

			// return so we don't pass on the key event
			return;
		case Qt::Key_Up:
			do {
				int offset = verticalScrollBar()->value() * bytesPerRow();

				if(origin_ != 0) {
					if(offset > 0) {
						offset += origin_;
						offset -= bytesPerRow();
					}
				}

				if(offset > 0) {
					scrollTo(offset - 1);
				}
			} while(0);

			// return so we don't pass on the key event
			return;
		}
	}

	QAbstractScrollArea::keyPressEvent(event);
}

//------------------------------------------------------------------------------
// Name: line3() const
// Desc: returns the x coordinate of the 3rd line
//------------------------------------------------------------------------------
int QHexView::line3() const {
	if(show_ascii_) {
		const int elements = bytesPerRow();
		return asciiDumpLeft() + (elements * font_width_) + (font_width_ / 2);
	} else {
		return line2();
	}
}

//------------------------------------------------------------------------------
// Name: line2() const
// Desc: returns the x coordinate of the 2nd line
//------------------------------------------------------------------------------
int QHexView::line2() const {
	if(show_hex_) {
		const int elements = row_width_ * (charsPerWord() + 1) - 1;
		return hexDumpLeft() + (elements * font_width_) + (font_width_ / 2);
	} else {
		return line1();
	}
}

//------------------------------------------------------------------------------
// Name: line1() const
// Desc: returns the x coordinate of the 1st line
//------------------------------------------------------------------------------
int QHexView::line1() const {
	if(show_address_) {
		const int elements = addressLen();
		return (elements * font_width_) + (font_width_ / 2);
	} else {
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: hexDumpLeft() const
// Desc: returns the x coordinate of the hex-dump field left edge
//------------------------------------------------------------------------------
int QHexView::hexDumpLeft() const {
	return line1() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: asciiDumpLeft() const
// Desc: returns the x coordinate of the ascii-dump field left edge
//------------------------------------------------------------------------------
int QHexView::asciiDumpLeft() const {
	return line2() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: commentLeft() const
// Desc: returns the x coordinate of the comment field left edge
//------------------------------------------------------------------------------
int QHexView::commentLeft() const {
	return line3() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: charsPerWord() const
// Desc: returns how many characters each word takes up
//------------------------------------------------------------------------------
unsigned int QHexView::charsPerWord() const {
	return word_width_ * 2;
}

//------------------------------------------------------------------------------
// Name: addressLen() const
// Desc: returns the lenth in characters the address will take up
//------------------------------------------------------------------------------
unsigned int QHexView::addressLen() const {
	static const unsigned int addressLength = (sizeof(address_t) * CHAR_BIT) / 4;
	return addressLength + (show_address_separator_ ? 1 : 0);
}

//------------------------------------------------------------------------------
// Name: updateScrollbars()
// Desc: recalculates scrollbar maximum value base on lines total and lines viewable
//------------------------------------------------------------------------------
void QHexView::updateScrollbars() {
	const int sz = dataSize();
	const int bpr = bytesPerRow();
	verticalScrollBar()->setMaximum(qMax(0, sz / bpr + ((sz % bpr) ? 1 : 0) - viewport()->height() / font_height_));
	horizontalScrollBar()->setMaximum(qMax(0, (line3() - viewport()->width()) / font_width_));
}

//------------------------------------------------------------------------------
// Name: scrollTo(unsigned int offset)
// Desc: scrolls view to given byte offset
//------------------------------------------------------------------------------
void QHexView::scrollTo(unsigned int offset) {

	const int bpr = bytesPerRow();
	origin_ = offset % bpr;
	address_t address = offset / bpr;

	updateScrollbars();

	if(origin_ != 0) {
		++address;
	}

	verticalScrollBar()->setValue(address);
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowAddress(bool show)
// Desc: sets if we are to display the address column
//------------------------------------------------------------------------------
void QHexView::setShowAddress(bool show) {
	show_address_ = show;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowHexDump(bool show)
// Desc: sets if we are to display the hex-dump column
//------------------------------------------------------------------------------
void QHexView::setShowHexDump(bool show) {
	show_hex_ = show;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowComments(bool show)
// Desc: sets if we are to display the comments column
//------------------------------------------------------------------------------
void QHexView::setShowComments(bool show) {
	show_comments_ = show;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowAsciiDump(bool show)
// Desc: sets if we are to display the ascii-dump column
//------------------------------------------------------------------------------
void QHexView::setShowAsciiDump(bool show) {
	show_ascii_ = show;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setRowWidth(int rowWidth)
// Desc: sets the row width (units is words)
//------------------------------------------------------------------------------
void QHexView::setRowWidth(int rowWidth) {
	row_width_ = rowWidth;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setWordWidth(int wordWidth)
// Desc: sets how many bytes represent a word
//------------------------------------------------------------------------------
void QHexView::setWordWidth(int wordWidth) {
	word_width_ = wordWidth;
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: bytesPerRow() const
//------------------------------------------------------------------------------
unsigned int QHexView::bytesPerRow() const {
	return row_width_ * word_width_;
}

//------------------------------------------------------------------------------
// Name: pixelToWord(int x, int y) const
//------------------------------------------------------------------------------
int QHexView::pixelToWord(int x, int y) const {
	int word = -1;

	switch(highlighting_) {
	case Highlighting_Data:
		// the right edge of a box is kinda quirky, so we pretend there is one
		// extra character there
		x = qBound(line1(), x, line2() + font_width_);

		// the selection is in the data view portion
		x -= line1();

		// scale x/y down to character from pixels
		x = x / font_width_ + (x % font_width_ >= font_width_ / 2 ? 1 : 0);
		y /= font_height_;

		// make x relative to rendering mode of the bytes
		x /= (charsPerWord() + 1);
		break;
	case Highlighting_Ascii:
		x = qBound(asciiDumpLeft(), x, line3());

		// the selection is in the ascii view portion
		x -= asciiDumpLeft();

		// scale x/y down to character from pixels
		x /= font_width_;
		y /= font_height_;

		// make x relative to rendering mode of the bytes
		x /= word_width_;
		break;
	default:
		Q_ASSERT(0);
		break;
	}

	// starting offset in bytes
	unsigned int start_offset = verticalScrollBar()->value() * bytesPerRow();

	// take into account the origin
	if(origin_ != 0) {
		if(start_offset > 0) {
			start_offset += origin_;
			start_offset -= bytesPerRow();
		}
	}

	// convert byte offset to word offset, rounding up
	start_offset /= word_width_;

	if((origin_ % word_width_) != 0) {
		start_offset += 1;
	}

	word = ((y * row_width_) + x) + start_offset;

	return word;
}

//------------------------------------------------------------------------------
// Name: mouseDoubleClickEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
void QHexView::mouseDoubleClickEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		const int x = event->x() + horizontalScrollBar()->value() * font_width_;
		const int y = event->y();
		if(x >= line1() && x < line2()) {

			highlighting_ = Highlighting_Data;

			const int offset = pixelToWord(x, y);
			int byte_offset = offset * word_width_;
			if(origin_) {
				if(origin_ % word_width_) {
					byte_offset -= word_width_ - (origin_ % word_width_);
				}
			}

			selection_start_ = byte_offset;
			selection_end_ = selection_start_ + word_width_;
			repaint();
		}
	}
}

//------------------------------------------------------------------------------
// Name: mousePressEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
void QHexView::mousePressEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		const int x = event->x() + horizontalScrollBar()->value() * font_width_;
		const int y = event->y();

		if(x < line2()) {
			highlighting_ = Highlighting_Data;
		} else if(x >= line2()) {
			highlighting_ = Highlighting_Ascii;
		}

		const int offset = pixelToWord(x, y);
		int byte_offset = offset * word_width_;
		if(origin_) {
			if(origin_ % word_width_) {
				byte_offset -= word_width_ - (origin_ % word_width_);
			}
		}

		if(offset < dataSize()) {
			selection_start_ = selection_end_ = byte_offset;
		} else {
			selection_start_ = selection_end_ = -1;
		}
		repaint();
	}
}

//------------------------------------------------------------------------------
// Name: mouseMoveEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
void QHexView::mouseMoveEvent(QMouseEvent *event) {
	if(highlighting_ != Highlighting_None) {
		const int x = event->x() + horizontalScrollBar()->value() * font_width_;
		const int y = event->y();

		const int offset = pixelToWord(x, y);

		if(selection_start_ != -1) {
			if(offset == -1) {
				selection_end_ = (row_width_ - selection_start_) + selection_start_;
			} else {

				int byte_offset = (offset * word_width_);

				if(origin_) {
					if(origin_ % word_width_) {
						byte_offset -= word_width_ - (origin_ % word_width_);
					}

				}
				selection_end_ = byte_offset;
			}

			if(selection_end_ < 0) {
				selection_end_ = 0;
			}

			if(!isInViewableArea(selection_end_)) {
				// TODO: scroll to an appropriate location
			}

		}
		repaint();
	}
}

//------------------------------------------------------------------------------
// Name: mouseReleaseEvent(QMouseEvent *event)
//------------------------------------------------------------------------------
void QHexView::mouseReleaseEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		highlighting_ = Highlighting_None;
	}
}

//------------------------------------------------------------------------------
// Name: setData(const QSharedPointer<QIODevice>& d)
//------------------------------------------------------------------------------
void QHexView::setData(const QSharedPointer<QIODevice>& d) {
	if (d->isSequential() || !d->size()) {
		QBuffer *b = new QBuffer;
		b->setData(d->readAll());
		b->open(QBuffer::ReadOnly);
		data_ = QSharedPointer<QIODevice>(b);
	} else {
		data_ = d;
	}

	deselect();
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: resizeEvent(QResizeEvent *)
//------------------------------------------------------------------------------
void QHexView::resizeEvent(QResizeEvent *) {
	updateScrollbars();
}

//------------------------------------------------------------------------------
// Name: setAddressOffset(address_t offset)
//------------------------------------------------------------------------------
void QHexView::setAddressOffset(address_t offset) {
	address_offset_ = offset;
}

//------------------------------------------------------------------------------
// Name: isSelected(int index) const
//------------------------------------------------------------------------------
bool QHexView::isSelected(int index) const {

	bool ret = false;
	if(index < static_cast<int>(dataSize())) {
		if(selection_start_ != selection_end_) {
			if(selection_start_ < selection_end_) {
				ret = (index >= selection_start_ && index < selection_end_);
			} else {
				ret = (index >= selection_end_ && index < selection_start_);
			}
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: drawComments(QPainter &painter, unsigned int offset, unsigned int row, int size) const
//------------------------------------------------------------------------------
void QHexView::drawComments(QPainter &painter, unsigned int offset, unsigned int row, int size) const {

	Q_UNUSED(size);

	painter.setPen(QPen(palette().text().color()));

	const address_t address = address_offset_ + offset;
	const QString comment   = comment_server_->comment(address, word_width_);

	painter.drawText(
		commentLeft(),
		row,
		comment.length() * font_width_,
		font_height_,
		Qt::AlignTop,
		comment
		);
}

//------------------------------------------------------------------------------
// Name: drawAsciiDumpToBuffer(QTextStream &stream, unsigned int offset, int size, const QByteArray &row_data) const
//------------------------------------------------------------------------------
void QHexView::drawAsciiDumpToBuffer(QTextStream &stream, unsigned int offset, int size, const QByteArray &row_data) const {
	// i is the byte index
	const int chars_per_row = bytesPerRow();
	for(int i = 0; i < chars_per_row; ++i) {

		const int index = offset + i;

		if(index < size) {

			if(isSelected(index)) {
				const unsigned char ch = row_data[i];
				const bool printable = is_printable(ch) && ch != '\f' && ch != '\t' && ch != '\r' && ch != '\n' && ch < 0x80;
				const char byteBuffer(printable ? ch : unprintable_char_);
				stream << byteBuffer;
			} else {
				stream << ' ';
			}
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: drawCommentsToBuffer(QTextStream &stream, unsigned int offset, int size) const
//------------------------------------------------------------------------------
void QHexView::drawCommentsToBuffer(QTextStream &stream, unsigned int offset, int size) const {
	Q_UNUSED(size);
	const address_t address = address_offset_ + offset;
	const QString comment   = comment_server_->comment(address, word_width_);
	stream << comment;
}

//------------------------------------------------------------------------------
// Name: format_bytes(const C &data_ref, int index) const
// Desc: formats bytes in a way that's suitable for rendering in a hexdump
//       having this as a separate function serves two purposes.
//       #1 no code duplication between the buffer and QPainter versions
//       #2 this encourages NRVO of the return value more than an integrated
//------------------------------------------------------------------------------
QString QHexView::format_bytes(const QByteArray &row_data, int index) const {
	union {
		quint64 q;
		quint32 d;
		quint16 w;
		quint8  b;
	} value = { 0 };

	char byte_buffer[32];

	switch(word_width_) {
	case 1:
		value.b |= (row_data[index + 0] & 0xff);
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%02x", value.b);
		break;
	case 2:
		value.w |= (row_data[index + 0] & 0xff);
		value.w |= (row_data[index + 1] & 0xff) << 8;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%04x", value.w);
		break;
	case 4:
		value.d |= (row_data[index + 0] & 0xff);
		value.d |= (row_data[index + 1] & 0xff) << 8;
		value.d |= (row_data[index + 2] & 0xff) << 16;
		value.d |= (row_data[index + 3] & 0xff) << 24;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%08x", value.d);
		break;
	case 8:
		// we need the cast to ensure that it won't assume 32-bit
		// and drop bits shifted more that 31
		value.q |= static_cast<quint64>(row_data[index + 0] & 0xff);
		value.q |= static_cast<quint64>(row_data[index + 1] & 0xff) << 8;
		value.q |= static_cast<quint64>(row_data[index + 2] & 0xff) << 16;
		value.q |= static_cast<quint64>(row_data[index + 3] & 0xff) << 24;
		value.q |= static_cast<quint64>(row_data[index + 4] & 0xff) << 32;
		value.q |= static_cast<quint64>(row_data[index + 5] & 0xff) << 40;
		value.q |= static_cast<quint64>(row_data[index + 6] & 0xff) << 48;
		value.q |= static_cast<quint64>(row_data[index + 7] & 0xff) << 56;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%016llx", value.q);
		break;
	}

	return byte_buffer;
}

//------------------------------------------------------------------------------
// Name: drawHexDumpToBuffer(QTextStream &stream, unsigned int offset, int size, const QByteArray &row_data) const
//------------------------------------------------------------------------------
void QHexView::drawHexDumpToBuffer(QTextStream &stream, unsigned int offset, int size, const QByteArray &row_data) const {

	Q_UNUSED(size);

	// i is the word we are currently rendering
	for(int i = 0; i < row_width_; ++i) {

		// index of first byte of current 'word'
		const int index = offset + (i * word_width_);

		// equal <=, not < because we want to test the END of the word we
		// about to render, not the start, it's allowed to end at the very last
		// byte
		if(index + word_width_ <= size) {
			const QString byteBuffer = format_bytes(row_data, i * word_width_);

			if(isSelected(index)) {
				stream << byteBuffer;
			} else {
				stream << QString(byteBuffer.length(), ' ');
			}

			if(i != (row_width_ - 1)) {
				stream << ' ';
			}
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: drawHexDump(QPainter &painter, unsigned int offset, unsigned int row, int size, int &word_count, const QByteArray &row_data) const
//------------------------------------------------------------------------------
void QHexView::drawHexDump(QPainter &painter, unsigned int offset, unsigned int row, int size, int &word_count, const QByteArray &row_data) const {
	const int hex_dump_left = hexDumpLeft();

	// i is the word we are currently rendering
	for(int i = 0; i < row_width_; ++i) {

		// index of first byte of current 'word'
		const int index = offset + (i * word_width_);

		// equal <=, not < because we want to test the END of the word we
		// about to render, not the start, it's allowed to end at the very last
		// byte
		if(index + word_width_ <= size) {

			const QString byteBuffer = format_bytes(row_data, i * word_width_);

			const int drawLeft = hex_dump_left + (i * (charsPerWord() + 1) * font_width_);

			if(isSelected(index)) {
				painter.fillRect(
					drawLeft,
					row,
					charsPerWord() * font_width_,
					font_height_,
					palette().highlight()
				);

				// should be highlight the space between us and the next word?
				if(i != (row_width_ - 1)) {
					if(isSelected(index + 1)) {
						painter.fillRect(
							drawLeft + font_width_,
							row,
							charsPerWord() * font_width_,
							font_height_,
							palette().highlight()
							);
					}
				}

				painter.setPen(QPen(palette().highlightedText().color()));
			} else {
				painter.setPen(QPen((word_count & 1) ? even_word_ : palette().text().color()));
			}

			painter.drawText(
				drawLeft,
				row,
				byteBuffer.length() * font_width_,
				font_height_,
				Qt::AlignTop,
				byteBuffer
				);

			++word_count;
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: drawAsciiDump(QPainter &painter, unsigned int offset, unsigned int row, int size, const QByteArray &row_data) const
//------------------------------------------------------------------------------
void QHexView::drawAsciiDump(QPainter &painter, unsigned int offset, unsigned int row, int size, const QByteArray &row_data) const {
	const int ascii_dump_left = asciiDumpLeft();

	// i is the byte index
	const int chars_per_row = bytesPerRow();
	for(int i = 0; i < chars_per_row; ++i) {

		const int index = offset + i;

		if(index < size) {
			const char ch        = row_data[i];
			const int drawLeft   = ascii_dump_left + i * font_width_;
			const bool printable = is_printable(ch);

			// drawing a selected character
			if(isSelected(index)) {

				painter.fillRect(
					drawLeft,
					row,
					font_width_,
					font_height_,
					palette().highlight()
					);

				painter.setPen(QPen(palette().highlightedText().color()));

			} else {
				painter.setPen(QPen(printable ? palette().text().color() : non_printable_text_));
			}

			const QString byteBuffer(printable ? ch : unprintable_char_);

			painter.drawText(
				drawLeft,
				row,
				font_width_,
				font_height_,
				Qt::AlignTop,
				byteBuffer
				);
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: paintEvent(QPaintEvent *)
//------------------------------------------------------------------------------
void QHexView::paintEvent(QPaintEvent *) {

	QPainter painter(viewport());
	painter.translate(-horizontalScrollBar()->value() * font_width_, 0);

	int word_count = 0;

	// pixel offset of this row
	unsigned int row = 0;

	const int chars_per_row = bytesPerRow();

	// current actual offset (in bytes)
	unsigned int offset = verticalScrollBar()->value() * chars_per_row;

	if(origin_ != 0) {
		if(offset > 0) {
			offset += origin_;
			offset -= chars_per_row;
		} else {
			origin_ = 0;
			updateScrollbars();
		}
	}

	const unsigned int data_size     = static_cast<unsigned int>(dataSize());
	const unsigned int widget_height = static_cast<unsigned int>(height());

	while(row + font_height_ < widget_height && offset < data_size) {

		data_->seek(offset);
		const QByteArray row_data = data_->read(chars_per_row);

		if(!row_data.isEmpty()) {
			if(show_address_) {
				const address_t address_rva = address_offset_ + offset;
				const QString addressBuffer = formatAddress(address_rva);
				painter.setPen(QPen(address_color_));
				painter.drawText(0, row, addressBuffer.length() * font_width_, font_height_, Qt::AlignTop, addressBuffer);
			}

			if(show_hex_) {
				drawHexDump(painter, offset, row, data_size, word_count, row_data);
			}

			if(show_ascii_) {
				drawAsciiDump(painter, offset, row, data_size, row_data);
			}

			if(show_comments_ && comment_server_) {
				drawComments(painter, offset, row, data_size);
			}
		}

		offset += chars_per_row;
		row += font_height_;
	}

	painter.setPen(QPen(palette().shadow().color()));

	if(show_address_ && show_line1_) {
		const int line1_x = line1();
		painter.drawLine(line1_x, 0, line1_x, widget_height);
	}

	if(show_hex_ && show_line2_) {
		const int line2_x = line2();
		painter.drawLine(line2_x, 0, line2_x, widget_height);
	}

	if(show_ascii_ && show_line3_) {
		const int line3_x = line3();
		painter.drawLine(line3_x, 0, line3_x, widget_height);
	}
}

//------------------------------------------------------------------------------
// Name: selectAll()
//------------------------------------------------------------------------------
void QHexView::selectAll() {
	selection_start_ = 0;
	selection_end_   = dataSize();
}

//------------------------------------------------------------------------------
// Name: deselect()
//------------------------------------------------------------------------------
void QHexView::deselect() {
	selection_start_ = -1;
	selection_end_   = -1;
}

//------------------------------------------------------------------------------
// Name: allBytes() const
//------------------------------------------------------------------------------
QByteArray QHexView::allBytes() const {
	data_->seek(0);
	return data_->readAll();
}

//------------------------------------------------------------------------------
// Name: selectedBytes() const
//------------------------------------------------------------------------------
QByteArray QHexView::selectedBytes() const {
	if(hasSelectedText()) {
		const int s = qMin(selection_start_, selection_end_);
		const int e = qMax(selection_start_, selection_end_);

		data_->seek(s);
		return data_->read(e - s);
	}

	return QByteArray();
}

//------------------------------------------------------------------------------
// Name: selectedBytesAddress() const
//------------------------------------------------------------------------------
QHexView::address_t QHexView::selectedBytesAddress() const {
	const address_t select_base = qMin(selection_start_, selection_end_);
	return select_base + address_offset_;
}

//------------------------------------------------------------------------------
// Name: selectedBytesSize() const
//------------------------------------------------------------------------------
unsigned int QHexView::selectedBytesSize() const {

	unsigned int ret;
	if(selection_end_ > selection_start_) {
		ret = selection_end_ - selection_start_;
	} else {
		ret = selection_start_ - selection_end_;
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: addressOffset() const
//------------------------------------------------------------------------------
QHexView::address_t QHexView::addressOffset() const {
	return address_offset_;
}

//------------------------------------------------------------------------------
// Name: setCommentServer(const CommentServerInterface::pointer &p)
//------------------------------------------------------------------------------
void QHexView::setCommentServer(const CommentServerInterface::pointer &p) {
	comment_server_ = p;
}

//------------------------------------------------------------------------------
// Name: commentServer() const
//------------------------------------------------------------------------------
QHexView::CommentServerInterface::pointer QHexView::commentServer() const {
	return comment_server_;
}

//------------------------------------------------------------------------------
// Name: showHexDump() const
//------------------------------------------------------------------------------
bool QHexView::showHexDump() const {
	return show_hex_;
}

//------------------------------------------------------------------------------
// Name: showAddress() const
//------------------------------------------------------------------------------
bool QHexView::showAddress() const {
	return show_address_;
}

//------------------------------------------------------------------------------
// Name: showAsciiDump() const
//------------------------------------------------------------------------------
bool QHexView::showAsciiDump() const {
	return show_ascii_;
}

//------------------------------------------------------------------------------
// Name: showComments() const
//------------------------------------------------------------------------------
bool QHexView::showComments() const {
	return show_comments_;
}

//------------------------------------------------------------------------------
// Name: wordWidth() const
//------------------------------------------------------------------------------
int QHexView::wordWidth() const {
	return word_width_;
}

//------------------------------------------------------------------------------
// Name: rowWidth() const
//------------------------------------------------------------------------------
int QHexView::rowWidth() const {
	return row_width_;
}

//------------------------------------------------------------------------------
// Name: firstVisibleAddress() const
//------------------------------------------------------------------------------
QHexView::address_t QHexView::firstVisibleAddress() const {
	// current actual offset (in bytes)
	const int chars_per_row = bytesPerRow();
	unsigned int offset = verticalScrollBar()->value() * chars_per_row;

	if(origin_ != 0) {
		if(offset > 0) {
			offset += origin_;
			offset -= chars_per_row;
		}
	}

	return offset + addressOffset();
}
