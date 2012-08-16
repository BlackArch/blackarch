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

#ifndef GRAPHWIDGET_20090903_H_
#define GRAPHWIDGET_20090903_H_

#include "GraphWidgetBase.h"
#include "API.h"
#include <QRectF>

class QContextMenuEvent;
class QMouseEvent;

#include <graphviz/gvc.h>

class EDB_EXPORT GraphWidget : public GraphWidgetBase {
	Q_OBJECT

private:
	friend class GraphNode;
	friend class GraphEdge;

public:
	GraphWidget(const QString& filename, const QString& layout, QWidget* parent = 0);
	GraphWidget(GVC_t *gvc, graph_t *graph, const QString& layout, QWidget* parent = 0);

	virtual ~GraphWidget();

public:
	void render_graph(const QString& filename, const QString& layout);
	void render_graph(GVC_t *gvc, graph_t *graph, const QString& layout);
	void clear_graph();

Q_SIGNALS:
	void backgroundContextMenuEvent(QContextMenuEvent* event);
	void nodeContextMenuEvent(QContextMenuEvent* event, const QString& name);
	void nodeDoubleClickEvent(QMouseEvent* event, const QString& name);

protected:
	void keyPressEvent(QKeyEvent* event);
	void wheelEvent(QWheelEvent* event);
	void contextMenuEvent(QContextMenuEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);

private:
	void render_node(graph_t *graph, node_t *node);
	void render_edge(edge_t *edge);
	void render_sub_graph(GVC_t *gvc, graph_t *graph);

private:
	void drawGraphLabel(graph_t *graph);

private:
	void scale_view(qreal scaleFactor);

	QPointF gToQ(const pointf& p, bool upside_down = true) const;
	QPointF gToQ(const point& p, bool upside_down = true) const;

	QColor aggetToQColor(void *obj, const char *name, const QColor& fallback) const;
	Qt::PenStyle aggetToQPenStyle(void *obj, const char *name, const Qt::PenStyle fallback) const;

private:
	QRectF graph_rect_;
};

#endif
