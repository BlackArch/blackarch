/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu

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

#include "QDisassemblyView.h"
#include "AnalyzerInterface.h"
#include "ArchProcessorInterface.h"
#include "Configuration.h"
#include "Debugger.h"
#include "DebuggerCoreInterface.h"
#include "Instruction.h"
#include "SymbolManager.h"
#include "SyntaxHighlighter.h"
#include "Util.h"

#include <QApplication>
#include <QAbstractItemDelegate>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QScrollBar>
#include <QTextDocument>
#include <QTextDocument>
#include <QTextLayout>
#include <QToolTip>
#include <QtGlobal>
#include <climits>

namespace {

	const int default_byte_width   = 8;
	const QColor filling_dis_color = Qt::gray;
	const QColor default_dis_color = Qt::blue;
	const QColor invalid_dis_color = Qt::blue;
	const QColor data_dis_color    = Qt::blue;

	void draw_rich_text(QPainter *painter, int x, int y, const QTextDocument &text) {
		painter->save();
		painter->translate(x, y);
		QAbstractTextDocumentLayout::PaintContext context;
		context.palette.setColor(QPalette::Text, painter->pen().color());
		text.documentLayout()->draw(painter, context);
		painter->restore();
	}

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
	
	bool near_line(int x, int linex) {
		return qAbs(x - linex) < 3;
	}
	
	int instruction_size(quint8 *buffer, std::size_t size) {
		edb::Instruction insn(buffer, size, 0, std::nothrow);
		return insn.size();
	}
}

//------------------------------------------------------------------------------
// Name: QDisassemblyView(QWidget * parent)
// Desc: constructor
//------------------------------------------------------------------------------
QDisassemblyView::QDisassemblyView(QWidget * parent) : QAbstractScrollArea(parent),
		breakpoint_icon_(":/debugger/images/edb14-breakpoint.png"),
		current_address_icon_(":/debugger/images/edb14-arrow.png"),
		highlighter_(new SyntaxHighlighter(this)),
		address_offset_(0),
		selected_instruction_address_(0),
		current_address_(0),
		line1_(0),
		line2_(0),
		line3_(0),
		selected_instruction_size_(0),
		moving_line1_(false),
		moving_line2_(false),
		moving_line3_(false) {


	setShowAddressSeparator(true);

	setFont(QFont("Monospace", 8));
	setMouseTracking(true);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(scrollbar_action_triggered(int)));
}

//------------------------------------------------------------------------------
// Name: ~QDisassemblyView()
// Desc:
//------------------------------------------------------------------------------
QDisassemblyView::~QDisassemblyView() {
}

//------------------------------------------------------------------------------
// Name: length_disasm_back(const quint8 *buf, size_t size) const
// Desc:
//------------------------------------------------------------------------------
size_t QDisassemblyView::length_disasm_back(const quint8 *buf, size_t size) const {


	quint8 tmp[edb::Instruction::MAX_SIZE * 2];

	Q_ASSERT(size <= sizeof(tmp));

	
	int offs = 0;

	memcpy(tmp, buf, size);

	while(offs < edb::Instruction::MAX_SIZE) {
	
		const edb::Instruction insn(tmp + offs, edb::Instruction::MAX_SIZE, 0, std::nothrow);
		if(!insn.valid()) {
			return 0;
		}
		const size_t cmdsize = insn.size();
		offs += cmdsize;

		if(offs == edb::Instruction::MAX_SIZE) {
			return cmdsize;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// Name: previous_instructions(edb::address_t current_address, int count)
// Desc:
//------------------------------------------------------------------------------
edb::address_t QDisassemblyView::previous_instructions(edb::address_t current_address, int count) {

	for(int i = 0; i < count; ++i) {
		

		quint8 buf[edb::Instruction::MAX_SIZE];

		int buf_size = qMin<edb::address_t>((current_address - region_.base), sizeof(buf));

		if(!edb::v1::get_instruction_bytes(address_offset_ + current_address - buf_size, buf, buf_size)) {
			current_address -= 1;
			break;
		}

		const size_t size = length_disasm_back(buf, buf_size);
		if(!size) {
			current_address -= 1;
			break;
		}

		current_address -= size;
	}
	return current_address;
}

//------------------------------------------------------------------------------
// Name: following_instructions(edb::address_t current_address, int count)
// Desc:
//------------------------------------------------------------------------------
edb::address_t QDisassemblyView::following_instructions(edb::address_t current_address, int count) {

	for(int i = 0; i < count; ++i) {
		
		quint8 buf[edb::Instruction::MAX_SIZE + 1];

		// do the longest read we can while still not passing the region end
		int buf_size = qMin<edb::address_t>((region_.end - current_address), sizeof(buf));

		// read in the bytes...
		if(!edb::v1::get_instruction_bytes(address_offset_ + current_address, buf, buf_size)) {
			current_address += 1;
			break;
		} else {
			const edb::Instruction insn(buf, buf_size, current_address, std::nothrow);
			if(insn) {
				current_address += insn.size();
			} else {
				current_address += 1;
				break;
			}
		}
	}
		
	return current_address;
}

//------------------------------------------------------------------------------
// Name: wheelEvent(QWheelEvent *e)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::wheelEvent(QWheelEvent *e) {

	if(e->modifiers() & Qt::ControlModifier) {
		e->accept();
		return;
	}

	const int dy = qAbs(e->delta());
	const int scroll_count = dy / 120;

	if(e->delta() > 0) {
		// scroll up
		edb::address_t address = verticalScrollBar()->value();
		address = previous_instructions(address, scroll_count);
		verticalScrollBar()->setValue(address);
	} else {
		// scroll down
		edb::address_t address = verticalScrollBar()->value();
		address = following_instructions(address, scroll_count);
		verticalScrollBar()->setValue(address);
	}
}

//------------------------------------------------------------------------------
// Name: scrollbar_action_triggered(int action)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::scrollbar_action_triggered(int action) {

	if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
		return;
	}

	switch(action) {
	case QAbstractSlider::SliderSingleStepSub:
		{
			edb::address_t address = verticalScrollBar()->value();
			address = previous_instructions(address, 1);
			verticalScrollBar()->setSliderPosition(address);
		}
		break;
	case QAbstractSlider::SliderPageStepSub:
		{
			edb::address_t address = verticalScrollBar()->value();
			address = previous_instructions(address, verticalScrollBar()->pageStep());
			verticalScrollBar()->setSliderPosition(address);
		}
		break;
	case QAbstractSlider::SliderSingleStepAdd:
		{
			edb::address_t address = verticalScrollBar()->value();
			address = following_instructions(address, 1);
			verticalScrollBar()->setSliderPosition(address);
		}
		break;
	case QAbstractSlider::SliderPageStepAdd:
		{
			edb::address_t address = verticalScrollBar()->value();
			address = following_instructions(address, verticalScrollBar()->pageStep());
			verticalScrollBar()->setSliderPosition(address);
		}
		break;
	
	case QAbstractSlider::SliderToMinimum:
	case QAbstractSlider::SliderToMaximum:
	case QAbstractSlider::SliderMove:
	case QAbstractSlider::SliderNoAction:
	default:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: setShowAddressSeparator(bool value)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::setShowAddressSeparator(bool value) {
	show_address_separator_ = value;
}

//------------------------------------------------------------------------------
// Name: formatAddress(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
QString QDisassemblyView::formatAddress(edb::address_t address) const {
	return format_address(address, show_address_separator_);
}

//------------------------------------------------------------------------------
// Name: repaint()
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::repaint() {
	viewport()->repaint();
}

//------------------------------------------------------------------------------
// Name: addressShown(edb::address_t address) const
// Desc: returns true if a given address is in the visible range
//------------------------------------------------------------------------------
bool QDisassemblyView::addressShown(edb::address_t address) const {
	return show_addresses_.contains(address);
}

//------------------------------------------------------------------------------
// Name: setCurrentAddress(edb::address_t address)
// Desc: sets the 'current address' (where EIP is usually)
//------------------------------------------------------------------------------
void QDisassemblyView::setCurrentAddress(edb::address_t address) {
	current_address_ = address;
}

//------------------------------------------------------------------------------
// Name: setRegion(const MemRegion &r)
// Desc: sets the memory region we are viewing
//------------------------------------------------------------------------------
void QDisassemblyView::setRegion(const MemRegion &r) {
	if(region_ != r) {
		region_ = r;
		updateScrollbars();		
		emit regionChanged();
	}
	repaint();
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc: clears the display
//------------------------------------------------------------------------------
void QDisassemblyView::clear() {
	setRegion(MemRegion());
}

//------------------------------------------------------------------------------
// Name: setAddressOffset(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::setAddressOffset(edb::address_t address) {
	address_offset_ = address;
}

//------------------------------------------------------------------------------
// Name: scrollTo(edb::address_t address)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::scrollTo(edb::address_t address) {
	verticalScrollBar()->setValue(address - address_offset_);
}

//------------------------------------------------------------------------------
// Name: format_instruction_bytes(const edb::Instruction &insn, int maxStringPx, const QFontMetrics &metrics) const
// Desc:
//------------------------------------------------------------------------------
QString QDisassemblyView::format_instruction_bytes(const edb::Instruction &insn, int maxStringPx, const QFontMetrics &metrics) const {
	const QString byte_buffer = edb::v1::format_bytes(QByteArray::fromRawData(reinterpret_cast<const char *>(insn.buffer()), insn.size()));
	return metrics.elidedText(byte_buffer, Qt::ElideRight, maxStringPx);
}

//------------------------------------------------------------------------------
// Name: format_instruction_bytes(const edb::Instruction &insn) const
// Desc:
//------------------------------------------------------------------------------
QString QDisassemblyView::format_instruction_bytes(const edb::Instruction &insn) const {
	return edb::v1::format_bytes(QByteArray::fromRawData(reinterpret_cast<const char *>(insn.buffer()), insn.size()));
}

//------------------------------------------------------------------------------
// Name: draw_instruction(QPainter &painter, const edb::Instruction &insn, bool upper, int y, int line_height, int l2, int l3) const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::draw_instruction(QPainter &painter, const edb::Instruction &insn, bool upper, int y, int line_height, int l2, int l3) const {

	const bool is_filling = edb::v1::arch_processor().is_filling(insn);
	int x                 = font_width_ + font_width_ + l2 + (font_width_ / 2);
	const int ret         = insn.size();

	if(insn.valid()) {
		QString opcode = QString::fromStdString(upper ? edisassm::to_string(insn, edisassm::syntax_intel_ucase()) : edisassm::to_string(insn, edisassm::syntax_intel_lcase()));
		
		opcode = painter.fontMetrics().elidedText(opcode, Qt::ElideRight, (l3 - l2) - font_width_ * 2);


		//return metrics.elidedText(byte_buffer, Qt::ElideRight, maxStringPx);


		if(is_filling) {
			painter.setPen(filling_dis_color);
			painter.drawText(
				x,
				y,
				opcode.length() * font_width_,
				line_height,
				Qt::AlignTop,
				opcode);
		} else {

			switch(insn.type()) {
			case edb::Instruction::OP_JCC:
			case edb::Instruction::OP_JMP:
			case edb::Instruction::OP_LOOP:
			case edb::Instruction::OP_LOOPE:
			case edb::Instruction::OP_LOOPNE:
			case edb::Instruction::OP_CALL:
				if(insn.operand_count() != 0) {
					const edb::Operand &oper = insn.operand(0);
					if(oper.general_type() == edb::Operand::TYPE_REL) {
						const edb::Instruction::address_t target = oper.relative_target();
						if(const Symbol::pointer sym = edb::v1::symbol_manager().find(target)) {
							opcode.append(QString(" <%2>").arg(sym->name));
						}
					}
				}
				break;
			default:
				break;
			}

			painter.setPen(default_dis_color);
			QTextDocument doc;
			doc.setDefaultFont(font());
			doc.setDocumentMargin(0);
			doc.setPlainText(opcode);
			highlighter_->setDocument(&doc);
			draw_rich_text(&painter, x, y, doc);
		}

	} else {
		const QString asm_buffer = format_invalid_instruction_bytes(insn, painter);

		painter.drawText(
			x,
			y,
			asm_buffer.length() * font_width_,
			line_height,
			Qt::AlignTop,
			asm_buffer);
	}

	return ret;
}

//------------------------------------------------------------------------------
// Name: format_invalid_instruction_bytes(const edb::Instruction &insn, QPainter &painter) const
// Desc:
//------------------------------------------------------------------------------
QString QDisassemblyView::format_invalid_instruction_bytes(const edb::Instruction &insn, QPainter &painter) const {
	char byte_buffer[32];
	const quint8 *const buf = insn.buffer();

	switch(insn.size()) {
	case 1:
		painter.setPen(data_dis_color);
		qsnprintf(byte_buffer, sizeof(byte_buffer), "db 0x%02x", *buf & 0xff);
		break;
	case 2:
		painter.setPen(data_dis_color);
		qsnprintf(byte_buffer, sizeof(byte_buffer), "dw 0x%04x", *reinterpret_cast<const quint16 *>(buf) & 0xffff);
		break;
	case 4:
		painter.setPen(data_dis_color);
		qsnprintf(byte_buffer, sizeof(byte_buffer), "dd 0x%08x", *reinterpret_cast<const quint32 *>(buf) & 0xffffffff);
		break;
	case 8:
		painter.setPen(data_dis_color);
		qsnprintf(byte_buffer, sizeof(byte_buffer), "dq 0x%016llx", *reinterpret_cast<const quint64 *>(buf));
		break;
	default:
		// we tried...didn't we?
		painter.setPen(invalid_dis_color);
		return tr("invalid");
	}
	return byte_buffer;
}

//------------------------------------------------------------------------------
// Name: draw_function_markers(QPainter &painter, edb::address_t address, int l2, int y, int insn_size, AnalyzerInterface *analyzer)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::draw_function_markers(QPainter &painter, edb::address_t address, int l2, int y, int insn_size, AnalyzerInterface *analyzer) {
	Q_CHECK_PTR(analyzer);
	painter.setPen(QPen(palette().shadow().color(), 2));

	const AnalyzerInterface::AddressCategory cat = analyzer->category(address);
	const int line_height = this->line_height();
	const int x = l2 + font_width_;

	switch(cat) {
	case AnalyzerInterface::ADDRESS_FUNC_START:

		// half of a horizontal
		painter.drawLine(
			x,
			y + line_height / 2,
			l2 + (font_width_ / 2) + font_width_,
			y + line_height / 2
			);

		// half of a vertical
		painter.drawLine(
			x,
			y + line_height / 2,
			x,
			y + line_height
			);

		break;
	case AnalyzerInterface::ADDRESS_FUNC_BODY:
		if(analyzer->category(address + insn_size - 1) == AnalyzerInterface::ADDRESS_FUNC_END) {
			goto do_end;
		} else {

			// vertical line
			painter.drawLine(
				x,
				y,
				x,
				y + line_height);

		}
		break;
	case AnalyzerInterface::ADDRESS_FUNC_END:
	do_end:

		// half of a vertical
		painter.drawLine(
			x,
			y,
			x,
			y + line_height / 2
			);

		// half of a horizontal
		painter.drawLine(
			x,
			y + line_height / 2,
			l2 + (font_width_ / 2) + font_width_,
			y + line_height / 2
			);
	default:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: paintEvent(QPaintEvent *)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::paintEvent(QPaintEvent *) {

	QPainter painter(viewport());

	const bool uppercase  = edb::v1::config().uppercase_disassembly;
	int viewable_lines    = viewport()->height() / line_height();
	int current_line      = verticalScrollBar()->value();
	int row_index         = 0;
	int y                 = 0;
	const int l1          = line1();
	const int l2          = line2();
	const int l3          = line3();
	const int line_height = this->line_height();

	// TODO: reimplement me
	// const Configuration::Syntax syntax = edb::v1::config().syntax;
	const int region_size = region_.size();

	if(region_size == 0) {
		return;
	}

	show_addresses_.clear();
	show_addresses_.insert(address_offset_ + current_line);

	const int bytes_width = l2 - l1;

	const QBrush alternated_base_color = palette().alternateBase();
	const QBrush bytes_color           = palette().text();
	const QBrush divider_color         = palette().shadow();
	const QPen bytes_pen               = bytes_color.color();
	const QPen divider_pen             = divider_color.color();

	AnalyzerInterface *const analyzer = edb::v1::analyzer();

	edb::address_t last_address = 0;

	while(viewable_lines >= 0 && current_line < region_size) {
		const edb::address_t address = address_offset_ + current_line;

		quint8 buf[edb::Instruction::MAX_SIZE + 1];

		// do the longest read we can while still not passing the region end
		int buf_size = qMin<edb::address_t>((region_.end - address), sizeof(buf));

		// read in the bytes...
		if(!edb::v1::get_instruction_bytes(address, buf, buf_size)) {
			// if the read failed, let's pretend that we were able to read a
			// single 0xff byte so that we have _something_ to display.
			buf_size = 1;
			*buf = 0xff;
		}

		// disassemble the instruction, if it happens that the next byte is the start of a known function
		// then we should treat this like a one byte instruction
		edb::Instruction insn(buf, buf_size, address, std::nothrow);
		if((analyzer != 0) && (analyzer->category(address + 1) == AnalyzerInterface::ADDRESS_FUNC_START)) {
			edb::Instruction(buf, 1, address, std::nothrow).swap(insn);
		}

		const int insn_size = insn.size();

		if(insn_size == 0) {
			return;
		}
		
		/*
		if(selectedAddress() == address) {
			painter.fillRect(0, y, width(), line_height, palette().highlight());
		} else 
		*/
		if(row_index & 1) {
			// draw alternating line backgrounds
			painter.fillRect(0, y, width(), line_height, alternated_base_color);
		}

		if(analyzer) {
			draw_function_markers(painter, address, l2, y, insn_size, analyzer);
		}

		// draw breakpoint icon or eip indicator
		if(address == current_address_) {
			painter.drawPixmap(1, y + 1, current_address_icon_);
		} else if(edb::v1::find_breakpoint(address) != 0) {
			painter.drawPixmap(1, y + 1, breakpoint_icon_);
		}

		// format the different components
		const QString byte_buffer    = format_instruction_bytes(insn, bytes_width, painter.fontMetrics());
		const QString address_buffer = formatAddress(address);

		// draw the address
		painter.setPen(QPen(Qt::red));
		painter.drawText(
			breakpoint_icon_.width() + 1,
			y,
			address_buffer.length() * font_width_,
			line_height,
			Qt::AlignTop,
			address_buffer);

		// draw the data bytes
		painter.setPen(bytes_pen);
		painter.drawText(
			l1 + (font_width_ / 2),
			y,
			byte_buffer.length() * font_width_,
			line_height,
			Qt::AlignTop,
			byte_buffer);


		//optionally draw the symbol name
		const Symbol::pointer sym = edb::v1::symbol_manager().find(address);
		if(sym) {

			const int maxStringPx = l1 - (breakpoint_icon_.width() + 1 + ((address_buffer.length() + 1) * font_width_));

			if(maxStringPx >= font_width_) {
				const QString symbol_buffer = painter.fontMetrics().elidedText(sym->name, Qt::ElideRight, maxStringPx);

				painter.drawText(
					breakpoint_icon_.width() + 1 + ((address_buffer.length() + 1) * font_width_),
					y,
					symbol_buffer.length() * font_width_,
					line_height,
					Qt::AlignTop,
					symbol_buffer);
			}
		}

		// for relative jumps draw the jump direction indicators
		switch(insn.type()) {
		case edb::Instruction::OP_JCC:
		case edb::Instruction::OP_JMP:
		case edb::Instruction::OP_LOOP:
		case edb::Instruction::OP_LOOPE:
		case edb::Instruction::OP_LOOPNE:
			if(insn.operand(0).general_type() == edb::Operand::TYPE_REL) {
				const edb::Instruction::address_t target = insn.operand(0).relative_target();

				painter.drawText(
					l2 + font_width_ + (font_width_ / 2),
					y,
					font_width_,
					line_height,
					Qt::AlignTop,
					QString((target > address) ? QChar(0x02C7) : QChar(0x02C6))
					);
			}
			break;
		default:
			break;
		}

		// draw the disassembly
		current_line += draw_instruction(painter, insn, uppercase, y, line_height, l2, l3);
		show_addresses_.insert(address);
		last_address = address;

		y += line_height;
		++row_index;
		--viewable_lines;
	}

	show_addresses_.remove(last_address);

	painter.setPen(divider_pen);
	painter.drawLine(l1, 0, l1, height());
	painter.drawLine(l2, 0, l2, height());
	painter.drawLine(l3, 0, l3, height());
}

//------------------------------------------------------------------------------
// Name: setFont(const QFont &f)
// Desc: overloaded version of setFont, calculates font metrics for later
//------------------------------------------------------------------------------
void QDisassemblyView::setFont(const QFont &f) {

	// TODO: assert that we are using a fixed font & find out if we care?
	QAbstractScrollArea::setFont(f);

	// recalculate all of our metrics/offsets
	const QFontMetrics metrics(f);
	font_width_  = metrics.width('X');
	font_height_ = metrics.height();

	updateScrollbars();
}

//------------------------------------------------------------------------------
// Name: resizeEvent(QResizeEvent *)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::resizeEvent(QResizeEvent *) {
	updateScrollbars();
}

//------------------------------------------------------------------------------
// Name: line_height() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::line_height() const {
	return qMax(font_height_, current_address_icon_.height());
}

//------------------------------------------------------------------------------
// Name: updateScrollbars()
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::updateScrollbars() {
	const unsigned int total_lines    = region_.size();
	const unsigned int viewable_lines = viewport()->height() / line_height();
	const unsigned int scroll_max     = (total_lines > viewable_lines) ? total_lines - 1 : 0;

	verticalScrollBar()->setMaximum(scroll_max);
}

//------------------------------------------------------------------------------
// Name: auto_line1() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::auto_line1() const {
	const unsigned int elements = address_length();
	return (elements * font_width_) + (font_width_ / 2) + breakpoint_icon_.width() + 1;
}

//------------------------------------------------------------------------------
// Name: line1() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::line1() const {
	if(line1_ == 0) {
		return auto_line1();
	} else {
		return line1_;
	}
}

//------------------------------------------------------------------------------
// Name: line2() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::line2() const {
	if(line2_ == 0) {
		return line1() + (default_byte_width * 3) * font_width_;
	} else {
		return line2_;
	}
}

//------------------------------------------------------------------------------
// Name: line3() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::line3() const {
	if(line3_ == 0) {
		return line2() + 50 * font_width_;
	} else {
		return line3_;
	}
}

//------------------------------------------------------------------------------
// Name: address_length() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::address_length() const {
	const unsigned int address_len = (sizeof(edb::address_t) * CHAR_BIT) / 4;
	return address_len + (show_address_separator_ ? 1 : 0);
}

//------------------------------------------------------------------------------
// Name: addressFromPoint(const QPoint &pos) const
// Desc:
//------------------------------------------------------------------------------
edb::address_t QDisassemblyView::addressFromPoint(const QPoint &pos) const {
	const edb::address_t address = address_from_coord(pos.x(), pos.y()) + address_offset_;
	if(address >= region_.end) {
		return 0;
	}
	return address;
}

//------------------------------------------------------------------------------
// Name: get_instruction_size(edb::address_t address, bool &ok, quint8 *buf, int &size) const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::get_instruction_size(edb::address_t address, bool &ok, quint8 *buf, int &size) const {
	int ret = 0;

	if(size < 0) {
		ok = false;
	} else {
		ok = edb::v1::get_instruction_bytes(address, buf, size);

		if(ok) {
			ret = instruction_size(buf, size);
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: get_instruction_size(edb::address_t address, bool &ok) const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::get_instruction_size(edb::address_t address, bool &ok) const {
	quint8 buf[edb::Instruction::MAX_SIZE];

	// do the longest read we can while still not crossing region end
	int buf_size = sizeof(buf);
	if(region_.end != 0 && address + buf_size > region_.end) {

		if(address <= region_.end) {
			buf_size = region_.end - address;
		} else {
			buf_size = 0;
		}
	}

	return get_instruction_size(address, ok, buf, buf_size);
}

//------------------------------------------------------------------------------
// Name: address_from_coord(int x, int y) const
// Desc:
//------------------------------------------------------------------------------
edb::address_t QDisassemblyView::address_from_coord(int x, int y) const {
	Q_UNUSED(x);

	const int line = y / line_height();
	edb::address_t address = verticalScrollBar()->value();

	// add up all the instructions sizes up to the line we want
	for(int i = 0; i < line; ++i) {
		bool ok;

		int size = get_instruction_size(address_offset_ + address, ok);
		if(ok) {
			address += (size != 0) ? size : 1;
		} else {
			address += 1;
		}
	}

	return address;
}

//------------------------------------------------------------------------------
// Name: mouseDoubleClickEvent(QMouseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::mouseDoubleClickEvent(QMouseEvent *event) {
	if(region_ != MemRegion()) {
		if(event->button() == Qt::LeftButton) {
			if(event->x() < line1()) {
				const edb::address_t address = addressFromPoint(event->pos());

				if(region_.contains(address)) {
					emit breakPointToggled(address);
					repaint();
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: event(QEvent *event)
// Desc:
//------------------------------------------------------------------------------
bool QDisassemblyView::event(QEvent *event) {

	if(region_ != MemRegion()) {
		if(event->type() == QEvent::ToolTip) {
			bool show = false;

			const QHelpEvent *const helpEvent = static_cast<QHelpEvent *>(event);

			if(helpEvent->x() >= line1() && helpEvent->x() < line2()) {

				const edb::address_t address = addressFromPoint(helpEvent->pos());

				quint8 buf[edb::Instruction::MAX_SIZE];

				// do the longest read we can while still not passing the region end
				int buf_size = qMin<edb::address_t>((region_.end - address), sizeof(buf));
				if(edb::v1::get_instruction_bytes(address, buf, buf_size)) {
					const edb::Instruction insn(buf, buf_size, address, std::nothrow);

					if((line1() + (static_cast<int>(insn.size()) * 3) * font_width_) > line2()) {
						const QString byte_buffer = format_instruction_bytes(insn);
						QToolTip::showText(helpEvent->globalPos(), byte_buffer);
						show = true;
					}
				}
			}

			if(!show) {
				QToolTip::showText(helpEvent->globalPos(), QString());
			}
		}
	}

	return QAbstractScrollArea::event(event);
}

//------------------------------------------------------------------------------
// Name: mouseReleaseEvent(QMouseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::mouseReleaseEvent(QMouseEvent *event) {

	Q_UNUSED(event);

	moving_line1_ = false;
	moving_line2_ = false;
	moving_line3_ = false;

	setCursor(Qt::ArrowCursor);
	repaint();
}

//------------------------------------------------------------------------------
// Name: updateSelectedAddress(QMouseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::updateSelectedAddress(QMouseEvent *event) {
	
	if(region_ != MemRegion()) {
		bool ok;
		const edb::address_t address = addressFromPoint(event->pos());
		const int size               = get_instruction_size(address, ok);
	
		if(ok) {
			selected_instruction_address_ = address;
			selected_instruction_size_    = size;
		} else {
			selected_instruction_address_ = 0;
			selected_instruction_size_    = 0;
		}
	}
}

//------------------------------------------------------------------------------
// Name: mousePressEvent(QMouseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::mousePressEvent(QMouseEvent *event) {

	if(region_ != MemRegion()) {
		if(event->button() == Qt::LeftButton) {
			if(near_line(event->x(), line1())) {
				moving_line1_ = true;
			} else if(near_line(event->x(), line2())) {
				moving_line2_ = true;
			} else if(near_line(event->x(), line3())) {
				moving_line3_ = true;
			} else {
				updateSelectedAddress(event);
			}
		} else {
			updateSelectedAddress(event);
		}
	}
}

//------------------------------------------------------------------------------
// Name: mouseMoveEvent(QMouseEvent *event)
// Desc:
//------------------------------------------------------------------------------
void QDisassemblyView::mouseMoveEvent(QMouseEvent *event) {

	if(region_ != MemRegion()) {
		const int x_pos = event->x();

		if(moving_line1_) {
			if(x_pos >= auto_line1() && x_pos + font_width_ < line2()) {
				if(line2_ == 0) {
					line2_ = line2();
				}
				line1_ = x_pos;
			}
			repaint();
		} else if(moving_line2_) {
			if(x_pos > line1() + font_width_ && x_pos + 1 < line3()) {
				if(line3_ == 0) {
					line3_ = line3();
				}
				line2_ = x_pos;
			}
			repaint();
		} else if(moving_line3_) {
			if(x_pos > line2() + font_width_ && x_pos + 1 < width() - (verticalScrollBar()->width() + 3)) {
				line3_ = x_pos;
			}
			repaint();
		} else {
			if(near_line(x_pos, line1()) || near_line(x_pos, line2()) || near_line(x_pos, line3())) {
				setCursor(Qt::SplitHCursor);
			} else {
				setCursor(Qt::ArrowCursor);
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: selectedAddress() const
// Desc:
//------------------------------------------------------------------------------
edb::address_t QDisassemblyView::selectedAddress() const {
	return selected_instruction_address_;
}

//------------------------------------------------------------------------------
// Name: selectedSize() const
// Desc:
//------------------------------------------------------------------------------
int QDisassemblyView::selectedSize() const {
	return selected_instruction_size_;
}

//------------------------------------------------------------------------------
// Name: region() const
// Desc:
//------------------------------------------------------------------------------
MemRegion QDisassemblyView::region() const {
	return region_;
}
