/* This file is part of qjson
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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

#include "parserrunnable.h"

#include "parser.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>

using namespace QJson;

class QJson::ParserRunnable::Private
{
  public:
    QByteArray m_data;
};

ParserRunnable::ParserRunnable(QObject* parent)
    : QObject(parent),
      QRunnable(),
      d(new Private)
{
  qRegisterMetaType<QVariant>("QVariant");
}

ParserRunnable::~ParserRunnable()
{
  delete d;
}

void ParserRunnable::setData( const QByteArray& data ) {
  d->m_data = data;
}

void ParserRunnable::run()
{
  qDebug() << Q_FUNC_INFO;

  bool ok;
  Parser parser;
  QVariant result = parser.parse (d->m_data, &ok);
  if (ok) {
    qDebug() << "successfully converted json item to QVariant object";
    emit parsingFinished(result, true, QString());
  } else {
    const QString errorText = tr("An error occured while parsing json: %1").arg(parser.errorString());
    qCritical() << errorText;
    emit parsingFinished(QVariant(), false, errorText);
  }
}
