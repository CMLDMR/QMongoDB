﻿#include "qmlbson.h"
#include "qml/qmlarray.h"
#include <memory>



QMLBSON::QMLBSON(QObject *parent) : QObject(parent)
{

}

QMLBSON::QMLBSON(const QMLBSON &bson)
{
    this->clear();
    this->append(bson.getMaplist());
}

QMLBSON::QMLBSON(const QBSON &bson)
{
    this->clear();
    this->append(bson.getMaplist());
}

QMLBSON *QMLBSON::operator=(QMLBSON *bson)
{
    this->clear();
    this->append(bson->getMaplist());
}

QMLBSON* QMLBSON::newBSON()
{
    QMLBSON* bson = new QMLBSON();
    return (bson);
}

void QMLBSON::addString( const QString &key, const QString &value )
{
    this->append(key,value);
}

void QMLBSON::addOid(const QString &key, const QString &oid)
{
    this->append(key,QOid(oid));
}

void QMLBSON::addInt(const QString &key, const int &value)
{
    this->append(key,value);
}

void QMLBSON::addInt64(const QString &key, const qint64 &value)
{
    this->append(key,value);
}

void QMLBSON::addDouble(const QString &key, const double &value)
{
    this->append( key , value );
}

void QMLBSON::addBool(const QString &key, const bool &value)
{
    this->append( key , value );
}

void QMLBSON::addBson(const QString &key, QMLBSON* value)
{
    this->append(key,value->getQBSON());
}

void QMLBSON::addArray(const QString &key, QMLArray *value)
{
    this->append(key,value->getArray());
}



QMLBSON &QMLBSON::add(QString key, QString value, const QMLElement::Type &type)
{
    if( type == QMLElement::B_oid )
    {
        this->append(key,QOid(value));
    }else{
        this->append(key,value);
    }
}

QMLBSON &QMLBSON::add(QString key, qreal value, const QMLElement::Type &type)
{
    switch ( type ) {
    case QMLElement::B_int32:
        this->append(key,static_cast<int>(value));
        break;
    case QMLElement::B_double:
        this->append(key,static_cast<double>(value));
        break;
    default:
        break;
    }
}

QMLBSON &QMLBSON::add(QString key, bool value)
{
    this->append(key,value);
}

QMLBSON &QMLBSON::add(QString key, QMLBSON* value)
{
    this->append(key,*value);
}

bool QMLBSON::containsKey(const QString &key)
{
    if( !this->Keys().contains(key) )
    {
        return false;
    }else{
        return true;
    }
}

QMLElement *QMLBSON::getElement(const QString &key)
{
    if( !this->Keys().contains(key) )
    {
        QString err = "bson document has no key: "+key;
        qDebug() << err;
        return new QMLElement();
    }else{
        QElement element = this->value(key);
        return new QMLElement(this->value(key));
    }
}


QMLElement::Type QMLBSON::getElementType(const QString &key)
{
    if( !this->Keys().contains(key) )
    {
        QString err = "bson document has no key: "+key;
        qDebug() << err;
        return QMLElement::B_invalid;
    }else{
        auto e = std::make_unique<QMLElement>(this->value(key));
        auto type = e->getElementType();
        return type;
    }
}

QJsonArray QMLBSON::getKeys()
{
    QJsonArray keylist;
    for( auto key : this->Keys() ){
        keylist.append(key);
    }
    return keylist;
}

QBSON QMLBSON::getQBSON()
{
    return static_cast<QBSON>(*this);
}


