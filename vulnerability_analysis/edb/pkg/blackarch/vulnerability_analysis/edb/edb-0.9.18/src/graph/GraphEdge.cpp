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

#include "GraphEdge.h"
#include "GraphWidget.h"
#include <QPainter>

//------------------------------------------------------------------------------
// Name: GraphEdge(const GraphWidget *graph, const bezier &bz, const QColor &color)
// Desc:
//------------------------------------------------------------------------------
GraphEdge::GraphEdge(const GraphWidget *graph, const bezier &bz, const QColor &color) : QGraphicsPathItem(make_bezier(bz, graph)) {
	QPainter painter(&picture_);

	if(bz.sflag) {
		draw_arrow(QLineF(graph->gToQ(bz.list[0]), graph->gToQ(bz.sp)), color, &painter);
	}

	if(bz.eflag) {
		draw_arrow(QLineF(graph->gToQ(bz.list[bz.size-1]), graph->gToQ(bz.ep)), color, &painter);
	}
}

//------------------------------------------------------------------------------
// Name: boundingRect() const
// Desc:
//------------------------------------------------------------------------------
QRectF GraphEdge::boundingRect() const {
	return QGraphicsPathItem::boundingRect().united(picture_.boundingRect());
}

//------------------------------------------------------------------------------
// Name: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
// Desc:
//------------------------------------------------------------------------------
void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->save();
	QGraphicsPathItem::paint(painter, option, widget);
	painter->restore();
	picture_.play(painter);
}

//------------------------------------------------------------------------------
// Name: make_bezier(const bezier &bezier, const GraphWidget *graph) const
// Desc:
//------------------------------------------------------------------------------
QPainterPath GraphEdge::make_bezier(const bezier &bezier, const GraphWidget *graph) const {
	QPainterPath path;
	path.moveTo(graph->gToQ(bezier.list[0]));
	for (int i = 1; i < bezier.size - 1; i += 3)
		path.cubicTo(
			graph->gToQ(bezier.list[i+0]),
			graph->gToQ(bezier.list[i+1]),
			graph->gToQ(bezier.list[i+2]));

	return path;
}

//------------------------------------------------------------------------------
// Name: draw_arrow(const QLineF &line, const QColor &color, QPainter *painter) const
// Desc:
//------------------------------------------------------------------------------
void GraphEdge::draw_arrow(const QLineF &line, const QColor &color, QPainter *painter) const {
	QLineF n(line.normalVector());
	QPointF o(n.dx() / 3.0, n.dy() / 3.0);

	QPolygonF polygon;
	polygon.append(line.p1() + o);
	polygon.append(line.p2());
	polygon.append(line.p1() - o);

	QPen pen(color);
	pen.setWidthF(1.0);
	painter->setPen(pen);

	QBrush brush(color);
	painter->setBrush(brush);

	painter->drawPolygon(polygon);
}
