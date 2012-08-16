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


#ifndef QY2EDGE_20090903_H_
#define QY2EDGE_20090903_H_

#include <QGraphicsPathItem>
#include <QPicture>
#include <graphviz/gvc.h>

class GraphWidget;

class GraphEdge : public QGraphicsPathItem {
public:
	GraphEdge(const GraphWidget *graph, const bezier &bz, const QColor &color);

public:
    enum { Type = UserType + 1 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
	QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QPainterPath make_bezier(const bezier &bezier, const GraphWidget *graph) const;
	void draw_arrow(const QLineF &line, const QColor &color, QPainter *painter) const;

private:
	QPicture picture_;
};

#endif
