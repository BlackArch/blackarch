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


#include "GraphNode.h"
#include "GraphWidget.h"
#include <QPainter>

//------------------------------------------------------------------------------
// Name: GraphNode(const GraphWidget *graph, node_t *node)
// Desc:
//------------------------------------------------------------------------------
GraphNode::GraphNode(const GraphWidget *graph, node_t *node) :
	QGraphicsPathItem(make_shape(node)),
	name(node->name),
	graph_(graph) {

	draw_label(ND_label(node));
}

//------------------------------------------------------------------------------
// Name: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
// Desc:
//------------------------------------------------------------------------------
void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->save();
	QGraphicsPathItem::paint(painter, option, widget);
	painter->restore();
	picture_.play(painter);
}

//------------------------------------------------------------------------------
// Name: draw_label(const textlabel_t *textlabel)
// Desc:
//------------------------------------------------------------------------------
void GraphNode::draw_label(const textlabel_t *textlabel) {
	QPainter painter(&picture_);

	painter.setPen(textlabel->fontcolor);

	// Since I always just take the points from graph_ and pass them to Qt
	// as pixel I also have to set the pixel size of the font.
	QFont font(textlabel->fontname, textlabel->fontsize);
	font.setPixelSize(textlabel->fontsize);

	if(!font.exactMatch()) {
		QFontInfo fontinfo(font);
		qWarning("replacing font \"%s\" by font \"%s\"", qPrintable(font.family()), qPrintable(fontinfo.family()));
	}

	painter.setFont(font);

	QString text(QString::fromUtf8(textlabel->text));
	QFontMetricsF fm(painter.fontMetrics());
	QRectF rect(fm.boundingRect(text));

#ifdef ND_coord_i
	rect.moveCenter(graph_->gToQ(textlabel->p, false));
#else
	rect.moveCenter(graph_->gToQ(textlabel->pos, false));
#endif
	painter.drawText(rect.adjusted(-2, -2, +2, +2), Qt::AlignCenter, text);
}

//------------------------------------------------------------------------------
// Name: make_polygon_helper(node_t *node, QPainterPath &path) const
// Desc:
//------------------------------------------------------------------------------
void GraphNode::make_polygon_helper(node_t *node, QPainterPath &path) const {
	const polygon_t *const poly = static_cast<polygon_t *>(ND_shape_info(node));

	if(poly->peripheries != 1) {
		qWarning("unsupported number of peripheries %d", poly->peripheries);
	}

	const int sides = poly->sides;
	const pointf* vertices = poly->vertices;

	QPolygonF polygon;
	for (int side = 0; side < sides; side++) {
		polygon.append(graph_->gToQ(vertices[side], false));
	}
	polygon.append(polygon[0]);

	path.addPolygon(polygon);
}

//------------------------------------------------------------------------------
// Name: make_ellipse_helper(node_t *node, QPainterPath &path) const
// Desc:
//------------------------------------------------------------------------------
void GraphNode::make_ellipse_helper(node_t *node, QPainterPath &path) const {
	const polygon_t *const poly = static_cast<polygon_t *>(ND_shape_info(node));

	if(poly->peripheries != 1) {
		qWarning("unsupported number of peripheries %d", poly->peripheries);
	}

	const int sides = poly->sides;
	const pointf* vertices = poly->vertices;

	QPolygonF polygon;
	for (int side = 0; side < sides; side++) {
		polygon.append(graph_->gToQ(vertices[side], false));
	}

	QRectF ellipse_bounds(polygon[0], polygon[1]);

	for (int i = 0; i < poly->peripheries; ++i) {
		path.addEllipse(ellipse_bounds.adjusted(-2 * i, 2 * i, 2 * i, -2 * i));
	}
}

//------------------------------------------------------------------------------
// Name: make_shape(node_t *node) const
// Desc:
//------------------------------------------------------------------------------
QPainterPath GraphNode::make_shape(node_t *node) const {
	QPainterPath path;

	const QString name = ND_shape(node)->name;

	// TODO: point, egg, doublecircle, doubleoctagon, tripleoctagon,
	// Mdiamond, MSquare, Mcircle, note, tab, folder, box3d, component

	// handle all of the "regular" polygons
	if(name == "invhouse" ||
			name == "invtrapezium" ||
			name == "invtriangle" ||
			name == "box" ||
			name == "polygon" ||
			name == "triangle" ||
			name == "diamond" ||
			name == "trapezium" ||
			name == "parallelogram" ||
			name == "house" ||
			name == "pentagon" ||
			name == "hexagon" ||
			name == "septagon" ||
			name == "octagon" ||
			name == "rect" ||
			name == "rectangle"
			) {

		make_polygon_helper(node, path);
	} else if(name == "ellipse" || name == "circle" || name == "point") {
		make_ellipse_helper(node, path);
	} else if(name == "plaintext" || name == "none") {
		// NO-OP
	} else if(name == "record") {

	} else {
		qWarning("unsupported shape %s", qPrintable(name));
	}

	return path;
}
