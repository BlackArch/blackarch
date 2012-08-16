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

#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QList>
#include <QRegExp>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class SyntaxHighlighter : public QSyntaxHighlighter {
	Q_OBJECT

public:
	SyntaxHighlighter(QObject *parent);
	SyntaxHighlighter(QTextDocument *parent);
	SyntaxHighlighter(QTextEdit *parent);

private:
	void create_rules();

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule {
		HighlightingRule();
		HighlightingRule(const QString &regex, const QBrush &foreground, const QBrush &background, int weight, bool italic, bool underline);

		QRegExp         pattern;
		QTextCharFormat format;
	};

	QList<HighlightingRule> rules_;
};

#endif
