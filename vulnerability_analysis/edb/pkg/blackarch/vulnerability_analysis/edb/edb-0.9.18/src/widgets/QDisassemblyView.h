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

#ifndef QDISASSEMBLYVIEW_20061101_H_
#define QDISASSEMBLYVIEW_20061101_H_

#include <QAbstractScrollArea>
#include <QAbstractSlider>
#include <QPixmap>
#include <QSet>

#include "MemRegion.h"
#include "Types.h"

class AnalyzerInterface;
class QPainter;
class QTextDocument;
class SyntaxHighlighter;

class QDisassemblyView : public QAbstractScrollArea {
	Q_OBJECT

public:
	QDisassemblyView(QWidget *parent = 0);
	~QDisassemblyView();

protected:
	virtual bool event(QEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void wheelEvent(QWheelEvent *e);

public:
	MemRegion region() const;
	bool addressShown(edb::address_t address) const;
	edb::address_t addressFromPoint(const QPoint &pos) const;
	edb::address_t selectedAddress() const;
	int selectedSize() const;

public Q_SLOTS:
	void setFont(const QFont &f);
	void resizeEvent(QResizeEvent *event);
	void scrollTo(edb::address_t address);
	void setAddressOffset(edb::address_t address);
	void setRegion(const MemRegion &r);
	void setCurrentAddress(edb::address_t address);
	void clear();
	void repaint();
	void setShowAddressSeparator(bool value);

private Q_SLOTS:
	void scrollbar_action_triggered(int action);

signals:
	void breakPointToggled(edb::address_t address);
	void regionChanged();

private:
	QString formatAddress(edb::address_t address) const;
	QString format_instruction_bytes(const edb::Instruction &insn) const;
	QString format_instruction_bytes(const edb::Instruction &insn, int maxStringPx, const QFontMetrics &metrics) const;
	QString format_invalid_instruction_bytes(const edb::Instruction &insn, QPainter &painter) const;
	edb::address_t address_from_coord(int x, int y) const;
	size_t length_disasm_back(const quint8 *buf, size_t size) const;
	edb::address_t previous_instructions(edb::address_t current_address, int count);
	edb::address_t following_instructions(edb::address_t current_address, int count);
	int address_length() const;
	int auto_line1() const;
	int draw_instruction(QPainter &painter, const edb::Instruction &insn, bool upper, int y, int line_height, int l2, int l3) const;
	int get_instruction_size(edb::address_t address, bool &ok) const;
	int get_instruction_size(edb::address_t address, bool &ok, quint8 *buf, int &size) const;
	int line1() const;
	int line2() const;
	int line3() const;
	int line_height() const;
	void draw_function_markers(QPainter &painter, edb::address_t address, int l2, int y, int insn_size, AnalyzerInterface *analyzer);
	void updateScrollbars();
	void updateSelectedAddress(QMouseEvent *event);

private:
	MemRegion                region_;
	QPixmap                  breakpoint_icon_;
	QPixmap                  current_address_icon_;
	QSet<edb::address_t>     show_addresses_;
	SyntaxHighlighter *const highlighter_;
	edb::address_t           address_offset_;
	edb::address_t           selected_instruction_address_;
	edb::address_t           current_address_;
	int                      font_height_; // height of a character in this font
	int                      font_width_;  // width of a character in this font
	int                      line1_;
	int                      line2_;
	int                      line3_;
	int                      selected_instruction_size_;
	bool                     moving_line1_;
	bool                     moving_line2_;
	bool                     moving_line3_;
	bool                     show_address_separator_;
};

#endif
