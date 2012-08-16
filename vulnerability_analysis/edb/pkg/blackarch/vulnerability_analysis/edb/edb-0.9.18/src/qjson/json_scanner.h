/* This file is part of QJson
 *
 * Copyright (C) 2008 Flavio Castelli <flavio.castelli@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _JSON_SCANNER
#define _JSON_SCANNER

#include <fstream>
#include <string>

#include <QtCore/QIODevice>
#include <QtCore/QVariant>

#define YYSTYPE QVariant

#include "parser_p.h"

namespace yy {
  class location;
  int yylex(YYSTYPE *yylval, yy::location *yylloc, QJson::ParserPrivate* driver);
}

class JSonScanner
{
    public:
        explicit JSonScanner(QIODevice* io);
        int yylex(YYSTYPE* yylval, yy::location *yylloc);
        
    protected:
        bool m_quotmarkClosed;
        unsigned int m_quotmarkCount;
        QIODevice* m_io;
};

#endif

