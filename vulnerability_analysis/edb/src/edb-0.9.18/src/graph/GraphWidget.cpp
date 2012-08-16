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


#include <cmath>
#include <climits>

#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>

#include "GraphWidget.h"
#include "GraphNode.h"
#include "GraphEdge.h"

#include <graphviz/gvc.h>

//------------------------------------------------------------------------------
// Name: GraphWidget(const QString& filename, const QString& layout, QWidget* parent)
// Desc:
//------------------------------------------------------------------------------
GraphWidget::GraphWidget(const QString& filename, const QString& layout, QWidget* parent) : GraphWidgetBase(parent) {
	render_graph(filename, layout);
}

//------------------------------------------------------------------------------
// Name: GraphWidget(GVC_t *gvc, graph_t *graph, const QString& layout, QWidget* parent)
// Desc:
//------------------------------------------------------------------------------
GraphWidget::GraphWidget(GVC_t *gvc, graph_t *graph, const QString& layout, QWidget* parent) : GraphWidgetBase(parent) {
	render_graph(gvc, graph, layout);
}

//------------------------------------------------------------------------------
// Name: ~GraphWidget()
// Desc:
//------------------------------------------------------------------------------
GraphWidget::~GraphWidget() {
}

//------------------------------------------------------------------------------
// Name: keyPressEvent(QKeyEvent* event)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::keyPressEvent(QKeyEvent* event) {
	switch(event->key()) {
	case Qt::Key_Plus:
		scale_view(1.2);
		break;
	case Qt::Key_Minus:
		scale_view(1.0 / 1.2);
		break;
	case Qt::Key_Asterisk:
		rotate(10.0);
		break;
	case Qt::Key_Slash:
		rotate(-10.0);
		break;
	default:
		QGraphicsView::keyPressEvent(event);
	}
}

//------------------------------------------------------------------------------
// Name: wheelEvent(QWheelEvent* event)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::wheelEvent(QWheelEvent* event) {
	scale_view(std::pow(2.0, +event->delta() / 240.0));
}

//------------------------------------------------------------------------------
// Name: scale_view(qreal scaleFactor)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::scale_view(qreal scaleFactor) {
	const qreal f = ::sqrt(matrix().det());

	scaleFactor = qBound(0.1 / f, scaleFactor, 8.0 / f);

	scale(scaleFactor, scaleFactor);
}

//------------------------------------------------------------------------------
// Name: contextMenuEvent(QContextMenuEvent* event)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::contextMenuEvent(QContextMenuEvent* event) {

	GraphNode *node = qgraphicsitem_cast<GraphNode*>(itemAt(event->pos()));

	if(node) {
		emit nodeContextMenuEvent(event, node->name);
	} else {
		emit backgroundContextMenuEvent(event);
	}
}

//------------------------------------------------------------------------------
// Name: mouseDoubleClickEvent(QMouseEvent* event)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::mouseDoubleClickEvent(QMouseEvent* event) {
	GraphNode *node = qgraphicsitem_cast<GraphNode*>(itemAt(event->pos()));

	if(node) {
		emit nodeDoubleClickEvent(event, node->name);
	}
}

//------------------------------------------------------------------------------
// Name: gToQ(const pointf& p, bool upside_down) const
// Desc:
//------------------------------------------------------------------------------
QPointF GraphWidget::gToQ(const pointf& p, bool upside_down) const {
	return upside_down ? QPointF(p.x, graph_rect_.height() - p.y) : QPointF(p.x, -p.y);
}

//------------------------------------------------------------------------------
// Name: gToQ(const point& p, bool upside_down) const
// Desc:
//------------------------------------------------------------------------------
QPointF GraphWidget::gToQ(const point& p, bool upside_down) const {
	return upside_down ? QPointF(p.x, graph_rect_.height() - p.y) : QPointF(p.x, -p.y);
}

//------------------------------------------------------------------------------
// Name: aggetToQColor(void *obj, const char *name, const QColor& fallback) const
// Desc:
//------------------------------------------------------------------------------
QColor GraphWidget::aggetToQColor(void *obj, const char *name, const QColor& fallback) const {
	const char *tmp = agget(obj, const_cast<char*>(name));
	if(tmp == 0 || strlen(tmp) == 0)
		return fallback;

	return QColor(tmp);
}

//------------------------------------------------------------------------------
// Name: aggetToQPenStyle(void *obj, const char *name, const Qt::PenStyle fallback) const
// Desc:
//------------------------------------------------------------------------------
Qt::PenStyle GraphWidget::aggetToQPenStyle(void *obj, const char *name, const Qt::PenStyle fallback) const {
	const char *tmp = agget(obj, const_cast<char*>(name));
	if(tmp == 0 || strlen(tmp) == 0)
		return fallback;

	if(strcmp(tmp, "dashed") == 0)
		return Qt::DashLine;

	if(strcmp(tmp, "dotted") == 0)
		return Qt::DotLine;

	if(strcmp(tmp, "solid") == 0)
		return Qt::SolidLine;

	return fallback;
}

//------------------------------------------------------------------------------
// Name: render_graph(const QString& filename, const QString& layout)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::render_graph(const QString& filename, const QString& layout) {
	if(FILE *const fp = fopen(qPrintable(filename), "r")) {
		if(GVC_t *gvc = gvContext()) {
			if(graph_t *graph = agread(fp)) {
				render_graph(gvc, graph, layout);
				agclose(graph);
			} else {
				qCritical("agread() failed");
			}
			gvFreeContext(gvc);
		} else {
			qCritical("gvContext() failed");
		}
		fclose(fp);
	} else {
		qCritical("failed to open %s", qPrintable(filename));
	}
}

//------------------------------------------------------------------------------
// Name: render_sub_graph(GVC_t *gvc, graph_t *graph)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::render_sub_graph(GVC_t *gvc, graph_t *graph) {

	if(graph) {
		for(int i = 1; i <= graph->u.n_cluster; ++i) {
			graph_t * sub_graph = graph->u.clust[i];
			render_sub_graph(gvc, sub_graph);
		}

		drawGraphLabel(graph);
	}

	const QRectF subGraphRect(
		gToQ(GD_bb(graph).LL),
		gToQ(GD_bb(graph).UR)
		);

	QPen graphPen(aggetToQColor(graph, "color", Qt::white));

	const QString style = QString::fromUtf8(agget(graph, const_cast<char *>("style")));
	const QBrush graphBrush(
		style.contains("filled") ? aggetToQColor(graph, "color", Qt::white) : QBrush()
		);

	QGraphicsRectItem *item = scene_->addRect(subGraphRect, graphPen, graphBrush);
	item->setZValue(INT_MIN);
}

//------------------------------------------------------------------------------
// Name: render_edge(edge_t *edge)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::render_edge(edge_t *edge) {
	const splines* spl = ED_spl(edge);

	if(spl == 0)
		return;

	for (int i = 0; i < spl->size; ++i) {
		const bezier& bz = spl->list[i];
		const QColor color(aggetToQColor(edge, "color", Qt::black));

		GraphEdge *const item = new GraphEdge(this, bz, color);

		QPen pen(color);
		pen.setStyle(aggetToQPenStyle(edge, "style", Qt::SolidLine));
		pen.setWidthF(1.0);
		item->setPen(pen);

		item->setZValue(-1.0);
		scene_->addItem(item);
	}
}

//------------------------------------------------------------------------------
// Name: render_node(graph_t *graph, node_t *node)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::render_node(graph_t *graph, node_t *node) {
	const QString node_style = QString::fromUtf8(agget(node, const_cast<char *>("style")));
	const QStringList node_styles = node_style.split(",");

	if(!node_styles.contains("invisible")) {

		GraphNode* item = new GraphNode(this, node);
		item->setZValue(1.0);
#ifdef ND_coord_i
		item->setPos(gToQ(ND_coord_i(node)));
#else
		item->setPos(gToQ(ND_coord(node)));
#endif

		QPen pen(aggetToQColor(node, "color", Qt::black));
		pen.setWidthF(1.0);
		item->setPen(pen);

		if(node_styles.contains("filled")) {
			QString fill_color = QString::fromUtf8(agget(node, const_cast<char *>("fillcolor")));
			if(fill_color.isEmpty()) {
				fill_color = QString::fromUtf8(agget(node, const_cast<char *>("color")));
			}

			QColor color(fill_color);
			if(color.isValid()) {
				item->setBrush(QBrush(color));
			} else {
				item->setBrush(QBrush(Qt::gray));
			}
		}

		QString tooltip = QString::fromUtf8(agget(node, const_cast<char *>("tooltip")));
		if(!tooltip.isEmpty()) {
			tooltip.replace("\\n", "\n");
			item->setToolTip(tooltip);
		}

		scene_->addItem(item);

		for(edge_t *e = agfstout(graph, node); e != 0; e = agnxtout(graph, e)) {
			render_edge(e);
		}
	}
}

//------------------------------------------------------------------------------
// Name: render_graph(GVC_t *gvc, graph_t *graph, const QString& layout)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::render_graph(GVC_t *gvc, graph_t *graph, const QString& layout) {

	if(gvLayout(gvc, graph, const_cast<char *>(qPrintable(layout))) == 0) {
		clear_graph();
		if(GD_charset(graph) != 0) {
			qWarning("unsupported charset");
		}

		// don't use gToQ here since it adjusts the values
		graph_rect_ = QRectF(
			GD_bb(graph).LL.x,
			GD_bb(graph).LL.y,
			GD_bb(graph).UR.x,
			GD_bb(graph).UR.y);

		scene_->setSceneRect(graph_rect_.adjusted(-5, -5, +5, +5));
		scene_->setBackgroundBrush(aggetToQColor(graph, "bgcolor", Qt::white));

		render_sub_graph(gvc, graph);

		for(node_t *n = agfstnode(graph); n != 0; n = agnxtnode(graph, n)) {
			render_node(graph, n);
		}
	} else {
		qCritical("gvLayout() failed");
	}
}

//------------------------------------------------------------------------------
// Name: drawGraphLabel(graph_t *graph)
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::drawGraphLabel(graph_t *graph) {
	const textlabel_t *const textlabel = GD_label(graph);
	if(textlabel) {
#if 0
		// Since I always just take the points from graph and pass them to Qt
		// as pixel I also have to set the pixel size of the font.
		QFont font(textlabel->fontname, textlabel->fontsize);
		font.setPixelSize(textlabel->fontsize);

		if(!font.exactMatch()) {
			QFontInfo fontinfo(font);
			qWarning("replacing font \"%s\" by font \"%s\"", qPrintable(font.family()), qPrintable(fontinfo.family()));
		}

		QGraphicsTextItem *item = scene_->addText(QString::fromUtf8(textlabel->text), font);

		const QRectF subGraphRect(
			gToQ(GD_bb(graph).LL),
			gToQ(GD_bb(graph).UR)
			);

		const pointf p = textlabel->pos;
		QPointF pos = true ? QPointF(p.x, subGraphRect.height() - p.y) : QPointF(p.x, -p.y);
		item->setPos(pos);
#endif
	}
}

//------------------------------------------------------------------------------
// Name: clear_graph()
// Desc:
//------------------------------------------------------------------------------
void GraphWidget::clear_graph() {
	qDeleteAll(scene_->items());
}
