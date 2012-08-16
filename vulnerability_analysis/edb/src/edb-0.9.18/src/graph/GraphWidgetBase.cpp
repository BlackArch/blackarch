/*
Copyright (C) 2009 - 2011 Evan Teran
                          eteran@alum.rit.edu

Copyright (C) 2009        Arvin Schnell
                          aschnell@suse.de

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

#include "GraphWidgetBase.h"

//------------------------------------------------------------------------------
// Name: GraphWidgetBase(QWidget *parent)
//------------------------------------------------------------------------------
GraphWidgetBase::GraphWidgetBase(QWidget *parent) : QGraphicsView(parent) {

	setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::TextAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	setResizeAnchor(AnchorUnderMouse);

	scene_ = new QGraphicsScene(this);
	scene_->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
	setScene(scene_);
}

