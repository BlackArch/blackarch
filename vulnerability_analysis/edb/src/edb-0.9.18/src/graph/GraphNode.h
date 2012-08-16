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


#ifndef QY2NODE_20090903_H_
#define QY2NODE_20090903_H_

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPicture>

#include <graphviz/gvc.h>

class GraphWidget;

class GraphNode : public QGraphicsPathItem {
public:
	GraphNode(const GraphWidget *graph, node_t *node);

public:
    enum { Type = UserType + 2 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPainterPath make_shape(node_t *node) const;
    void make_polygon_helper(node_t *node, QPainterPath &path) const;
	void make_ellipse_helper(node_t *node, QPainterPath &path) const;
	void draw_label(const textlabel_t *textlabel);

public:
	QString name;

private:
	QPicture                 picture_;
	const GraphWidget *const graph_;
};

#endif
