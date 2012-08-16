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

#include "LineEdit.h"
#include <QToolButton>
#include <QStyle>

//------------------------------------------------------------------------------
// Name: LineEdit(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent), clear_button_(new QToolButton(this)) {

	QPixmap pixmap(":/debugger/images/edb16-edit-clean.png");

	clear_button_->setIcon(QIcon(pixmap));
	clear_button_->setIconSize(pixmap.size());
	clear_button_->setCursor(Qt::ArrowCursor);
	clear_button_->setStyleSheet("QToolButton { border: none; padding: 0px; }");
	clear_button_->hide();

	connect(clear_button_, SIGNAL(clicked()), this, SLOT(clear()));
	connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(updateCloseButton(const QString &)));

	const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

	setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clear_button_->sizeHint().width() + frameWidth + 1));
	QSize msz = minimumSizeHint();
	setMinimumSize(qMax(msz.width(), clear_button_->sizeHint().height() + frameWidth * 2 + 2), qMax(msz.height(), clear_button_->sizeHint().height() + frameWidth * 2 + 2));
}

//------------------------------------------------------------------------------
// Name: resizeEvent(QResizeEvent *)
// Desc:
//------------------------------------------------------------------------------
void LineEdit::resizeEvent(QResizeEvent *) {
    QSize sz = clear_button_->sizeHint();
    const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    clear_button_->move(rect().right() - frameWidth - sz.width(), (rect().bottom() + 1 - sz.height())/2);
}

//------------------------------------------------------------------------------
// Name: updateCloseButton(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void LineEdit::updateCloseButton(const QString &text) {
    clear_button_->setVisible(!text.isEmpty());
}


