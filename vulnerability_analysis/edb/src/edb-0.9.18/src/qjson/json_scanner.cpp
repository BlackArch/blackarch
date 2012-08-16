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

#include "qjson_debug.h"
#include "json_scanner.h"
#include "json_parser.hh"

#include <ctype.h>

#include <QtCore/QDebug>
#include <QtCore/QRegExp>

#include <cassert>

bool ishexnstring(const QString& string) {
  for (int i = 0; i < string.length(); i++) {
    if (isxdigit(string[i] == 0))
      return false;
  }
  return true;
}

JSonScanner::JSonScanner(QIODevice* io)
  : m_io (io)
{
  m_quotmarkClosed = true;
  m_quotmarkCount = 0;
}

static QString unescape( const QByteArray& ba, bool* ok ) {
  assert( ok );
  *ok = false;
  QString res;
  QByteArray seg;
  bool bs = false;
  for ( int i = 0, size = ba.size(); i < size; ++i ) {
    const char ch = ba[i];
    if ( !bs ) {
      if ( ch == '\\' )
        bs = true;
      else
        seg += ch;
    } else {
      bs = false;
      switch ( ch ) {
        case 'b':
          seg += '\b';
          break;
        case 'f':
          seg += '\f';
          break;
        case 'n':
          seg += '\n';
          break;
        case 'r':
          seg += '\r';
          break;
        case 't':
          seg += '\t';
          break;
        case 'u':
        {
          res += QString::fromUtf8( seg );
          seg.clear();

          if ( i > size - 5 ) {
            //error
            return QString();
          }

          const QString hex_digit1 = QString::fromUtf8( ba.mid( i + 1, 2 ) );
          const QString hex_digit2 = QString::fromUtf8( ba.mid( i + 3, 2 ) );
          i += 4;

          if ( !ishexnstring( hex_digit1 ) || !ishexnstring( hex_digit2 ) ) {
            qCritical() << "Not an hex string:" << hex_digit1 << hex_digit2;
            return QString();
          }
          bool hexOk;
          const ushort hex_code1 = hex_digit1.toShort( &hexOk, 16 );
          if (!hexOk) {
            qCritical() << "error converting hex value to short:" << hex_digit1;
            return QString();
          }
          const ushort hex_code2 = hex_digit2.toShort( &hexOk, 16 );
          if (!hexOk) {
            qCritical() << "error converting hex value to short:" << hex_digit2;
            return QString();
          }

          res += QChar(hex_code2, hex_code1);
          break;
        }
        case '\\':
          seg  += '\\';
          break;
        default:
          seg += ch;
          break;
      }
    }
  }
  res += QString::fromUtf8( seg );
  *ok = true;
  return res;
}

int JSonScanner::yylex(YYSTYPE* yylval, yy::location *yylloc)
{
  char ch;
  
  if (!m_io->isOpen()) {
    qCritical() << "JSonScanner::yylex - io device is not open";
    return -1;
  }

  yylloc->step();

  do {
    bool ret;
    if (m_io->atEnd()) {
      qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::END";
      return yy::json_parser::token::END;
    }
    else
      ret = m_io->getChar(&ch);

    if (!ret) {
      qCritical() << "JSonScanner::yylex - error reading from io device";
      return -1;
    }

    qjsonDebug() << "JSonScanner::yylex - got |" << ch << "|";
    
    yylloc->columns();
    
    if (ch == '\n' || ch == '\r')
      yylloc->lines();
      
  } while (m_quotmarkClosed && (isspace(ch) != 0));

  if (m_quotmarkClosed && ((ch == 't') || (ch == 'T')
      || (ch == 'n') || (ch == 'N'))) {
    // check true & null value
    const QByteArray buf = m_io->peek(3).toLower();

    if (buf.length() == 3) {
      if (buf == "rue") {
        m_io->read (3);
        yylloc->columns(3);
        qjsonDebug() << "JSonScanner::yylex - TRUE_VAL";
        return yy::json_parser::token::TRUE_VAL;
      }
      else if (buf == "ull") {
        m_io->read (3);
        yylloc->columns(3);
        qjsonDebug() << "JSonScanner::yylex - NULL_VAL";
        return yy::json_parser::token::NULL_VAL;
      }
    }
  }
  else if (m_quotmarkClosed && ((ch == 'f') || (ch == 'F'))) {
    // check false value
    const QByteArray buf = m_io->peek(4).toLower();
    if (buf.length() == 4) {
      if (buf == "alse") {
        m_io->read (4);
        yylloc->columns(4);
        qjsonDebug() << "JSonScanner::yylex - FALSE_VAL";
        return yy::json_parser::token::FALSE_VAL;
      }
    }
  }
  else if (m_quotmarkClosed && ((ch == 'e') || (ch == 'E'))) {
    QByteArray ret(1, ch);
    const QByteArray buf = m_io->peek(1);
    if (!buf.isEmpty()) {
      if ((buf[0] == '+' ) || (buf[0] == '-' )) {
        ret += m_io->read (1);  
        yylloc->columns();
      }
    }
    *yylval = QVariant(QString::fromUtf8(ret));
    return yy::json_parser::token::E;
  }
  
  if (ch != '"' && !m_quotmarkClosed) {
    // we're inside a " " block
    QByteArray raw;
    raw += ch;
    char prevCh = ch;
    bool escape_on = (ch == '\\') ? true : false;

    while ( true ) {
      char nextCh;
      qint64 ret = m_io->peek(&nextCh, 1);
      if (ret != 1) {
        if (m_io->atEnd())
          return yy::json_parser::token::END;
        else
          return -1;
      } else if ( !escape_on && nextCh == '\"' ) {
        bool ok;
        const QString str = unescape( raw, &ok );
        *yylval = ok ? str : QString();
        return ok ? yy::json_parser::token::STRING : -1;
      }
#if 0
      if ( prevCh == '\\' && nextCh != '"' && nextCh != '\\' && nextCh != '/' &&
           nextCh != 'b' && nextCh != 'f' && nextCh != 'n' &&
           nextCh != 'r' && nextCh != 't' && nextCh != 'u') {
        qjsonDebug() << "Just read" << nextCh;
        qjsonDebug() << "JSonScanner::yylex - error decoding escaped sequence";
        return -1;
       }
#endif
      m_io->read(1); // consume
      raw += nextCh;
      prevCh = nextCh;
      if (escape_on)
        escape_on = false;
      else
        escape_on = (prevCh == '\\') ? true : false;
#if 0
      if (nextCh == '\\') {
        char buf;
        if (m_io->getChar (&buf)) {
          yylloc->columns();
          if (((buf != '"') && (buf != '\\') && (buf != '/') &&
              (buf != 'b') && (buf != 'f') && (buf != 'n') &&
              (buf != 'r') && (buf != 't') && (buf != 'u'))) {
                qjsonDebug() << "Just read" << buf;
                qjsonDebug() << "JSonScanner::yylex - error decoding escaped sequence";
                return -1;
          }
        } else {
          qCritical() << "JSonScanner::yylex - error decoding escaped sequence : io error";
          return -1;
        }
      }
#endif
    }
  }
  else if (isdigit(ch) != 0 && m_quotmarkClosed) {
    *yylval = QVariant(QString::fromLatin1(QByteArray(&ch,1)));
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::DIGIT";
    return yy::json_parser::token::DIGIT;
  }
  else if (isalnum(ch) != 0) {
    *yylval = QVariant(QString(QChar::fromLatin1(ch)));
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::WORD ("
             << ch << ")";
    return yy::json_parser::token::STRING;
  }
  else if (ch == ':') {
    // set yylval
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::COLON";
    return yy::json_parser::token::COLON;
  }
  else if (ch == '"') {
    // yy::json_parser::token::QUOTMARK (")

    // set yylval
    m_quotmarkCount++;
    if (m_quotmarkCount %2 == 0) {
      m_quotmarkClosed = true;
      m_quotmarkCount = 0;
      qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::QUOTMARKCLOSE";
      return yy::json_parser::token::QUOTMARKCLOSE;
    }
    else {
      m_quotmarkClosed = false;
      qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::QUOTMARKOPEN";
      return yy::json_parser::token::QUOTMARKOPEN;
    }
  }
  else if (ch == ',') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::COMMA";
    return yy::json_parser::token::COMMA;
  }
  else if (ch == '.') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::DOT";
    return yy::json_parser::token::DOT;
  }
  else if (ch == '-') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::MINUS";
    return yy::json_parser::token::MINUS;
  }
  else if (ch == '[') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::SQUARE_BRACKET_OPEN";
    return yy::json_parser::token::SQUARE_BRACKET_OPEN;
  }
  else if (ch == ']') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::SQUARE_BRACKET_CLOSE";
    return yy::json_parser::token::SQUARE_BRACKET_CLOSE;
  }
  else if (ch == '{') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::CURLY_BRACKET_OPEN";
    return yy::json_parser::token::CURLY_BRACKET_OPEN;
  }
  else if (ch == '}') {
    qjsonDebug() << "JSonScanner::yylex - yy::json_parser::token::CURLY_BRACKET_CLOSE";
    return yy::json_parser::token::CURLY_BRACKET_CLOSE;
  }

  //unknown char!
  //TODO yyerror?
  qCritical() << "JSonScanner::yylex - unknown char, returning -1";
  return -1;
}


