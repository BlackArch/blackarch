/* This file is part of qjson
  *
  * Copyright (C) 2009 Till Adam <adam@kde.org>
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

#include "serializer.h"

#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

using namespace QJson;

class Serializer::SerializerPrivate {
};

Serializer::Serializer() : d( new SerializerPrivate ) {
}

Serializer::~Serializer() {
  delete d;
}

void Serializer::serialize( const QVariant& v, QIODevice* io, bool* ok )
{
  Q_ASSERT( io );
  if (!io->isOpen()) {
    if (!io->open(QIODevice::WriteOnly)) {
      if ( ok != 0 )
        *ok = false;
      qCritical ("Error opening device");
      return;
    }
  }

  if (!io->isWritable()) {
    if (ok != 0)
      *ok = false;
    qCritical ("Device is not readable");
    io->close();
    return;
  }

  const QByteArray str = serialize( v );
  if ( !str.isNull() ) {
    QDataStream stream( io );
    stream << str;
  } else {
    if ( ok )
      *ok = false;
  }
}

static QString sanitizeString( QString str )
{
  str.replace( QLatin1String( "\\" ), QLatin1String( "\\\\" ) );
  str.replace( QLatin1String( "\"" ), QLatin1String( "\\\"" ) );
  str.replace( QLatin1String( "\b" ), QLatin1String( "\\b" ) );
  str.replace( QLatin1String( "\f" ), QLatin1String( "\\f" ) );
  str.replace( QLatin1String( "\n" ), QLatin1String( "\\n" ) );
  str.replace( QLatin1String( "\r" ), QLatin1String( "\\r" ) );
  str.replace( QLatin1String( "\t" ), QLatin1String( "\\t" ) );
  return QString( QLatin1String( "\"%1\"" ) ).arg( str );
}

static QByteArray join( const QList<QByteArray>& list, const QByteArray& sep ) {
  QByteArray res;
  Q_FOREACH( const QByteArray& i, list ) {
    if ( !res.isEmpty() )
      res += sep;
    res += i;
  }
  return res;
}

QByteArray Serializer::serialize( const QVariant &v )
{
  QByteArray str;
  bool error = false;

  if ( ! v.isValid() ) { // invalid or null?
    str = "null";
  } else if ( v.type() == QVariant::List ) { // variant is a list?
    const QVariantList list = v.toList();
    QList<QByteArray> values;
    Q_FOREACH( const QVariant& v, list )
    {
      QByteArray serializedValue = serialize( v );
      if ( serializedValue.isNull() ) {
        error = true;
        break;
      }
      values << serializedValue;
    }
    str = "[ " + join( values, ", " ) + " ]";
  } else if ( v.type() == QVariant::Map ) { // variant is a map?
    const QVariantMap vmap = v.toMap();
    QMapIterator<QString, QVariant> it( vmap );
    str = "{ ";
    QList<QByteArray> pairs;
    while ( it.hasNext() ) {
      it.next();
      QByteArray serializedValue = serialize( it.value() );
      if ( serializedValue.isNull() ) {
        error = true;
        break;
      }
      pairs << sanitizeString( it.key() ).toUtf8() + " : " + serializedValue;
    }
    str += join( pairs, ", " );
    str += " }";
  } else if (( v.type() == QVariant::String ) ||  ( v.type() == QVariant::ByteArray )) { // a string or a byte array?
    str = sanitizeString( v.toString() ).toUtf8();
  } else if ( v.type() == QVariant::Double ) { // a double?
    str = QByteArray::number( v.toDouble() );
    if( ! str.contains( "." ) && ! str.contains( "e" ) ) {
      str += ".0";
    }
  } else if ( v.type() == QVariant::Bool ) { // boolean value?
    str = ( v.toBool() ? "true" : "false" );
  } else if ( v.type() == QVariant::ULongLong ) { // large unsigned number?
    str = QByteArray::number( v.value<qulonglong>() );
  } else if ( v.canConvert<qlonglong>() ) { // any signed number?
    str = QByteArray::number( v.value<qlonglong>() );
  } else if ( v.canConvert<QString>() ){ // can value be converted to string?
    // this will catch QDate, QDateTime, QUrl, ...
    str = sanitizeString( v.toString() ).toUtf8();
    //TODO: catch other values like QImage, QRect, ...
  } else {
    error = true;
  }
  if ( !error )
    return str;
  else
    return QByteArray();
}
