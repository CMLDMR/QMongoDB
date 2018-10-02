#include "qmlelement.h"
#include "qmlbson.h"
#include "qmlarray.h"

QMLElement::QMLElement(QObject *parent)
    : QObject(parent)
{

}


QMLElement::QMLElement(const QMLElement& element)
    :QObject () , QElement ()
{
    QElement _element( this->getType(),element.getValue(),element.getKey() );
    this->setElement(_element);
    emit keyChanged();

    switch ( this->getType() ) {
    case QElementType::b_utf8:
        emit stringChanged();
        break;

    case QElementType::b_oid:
        emit oidChanged();
        break;

    case QElementType::b_document:
        emit bsonChanged();
        break;

    case QElementType::b_int64:
        emit int64Changed();
        break;

    case QElementType::b_int32:
        emit intChanged();
        break;

    case QElementType::b_double:
        emit doubleChanged();
        break;

    case QElementType::b_array:
        emit arrayChanged();
        break;

    default:
        break;
    }
}

QMLElement::QMLElement(const QElement &element)
    :QObject ()
{
    this->setElement(element);
    emit keyChanged();

    switch ( element.getType() ) {
    case QElementType::b_utf8:
        emit stringChanged();
        break;

    case QElementType::b_oid:
        emit oidChanged();
        break;
    case QElementType::b_document:
        emit bsonChanged();
        break;

    case QElementType::b_int64:
        emit int64Changed();
        break;

    case QElementType::b_int32:
        emit intChanged();
        break;

    case QElementType::b_double:
        emit doubleChanged();
        break;

    case QElementType::b_array:
        emit arrayChanged();
        break;

    default:
        break;
    }
}

QMLElement &QMLElement::operator=(const QMLElement &other)
{
    this->setElement(other.getQElement());
    emit keyChanged();

    switch ( other.getType() ) {
    case QElementType::b_utf8:
        emit stringChanged();
        break;
    case QElementType::b_oid:
        emit oidChanged();
        break;
    case QElementType::b_document:
        emit bsonChanged();
        break;

    case QElementType::b_int64:
        emit int64Changed();
        break;

    case QElementType::b_int32:
        emit intChanged();
        break;

    case QElementType::b_double:
        emit doubleChanged();
        break;

    case QElementType::b_array:
        emit arrayChanged();
        break;

    default:
        break;
    }
    return *this;
}
//TODO: other element type must be implemented
QMLElement::QMLElement(const QElementType &type, const QString &key, const QString &value)
{
    switch ( type ) {
    case QElementType::b_utf8:
    {
        QElement elemet(type,QVariant::fromValue(value),key);
        this->setElement(elemet);
        emit stringChanged();
    }
        break;
    case QElementType::b_oid:
    {
        QElement elemet(QOid(value),key);
        this->setElement(elemet);
        emit oidChanged();
    }
        break;

    default:
        break;
    }
    emit keyChanged();
}

QMLElement* QMLElement::newElement()
{
    QMLElement* element = new QMLElement();
    return ( element );
}

QMLElement* QMLElement::newElement(const QMLElement::Type &type, const QString &key, const QString &value)
{
    QMLElement* element = new QMLElement(QMLElement::ConvertType(type),key,value);
    return ( element );
}

void QMLElement::setStringData(const QString &key, const QString &value)
{
    this->setType(QElementType::b_utf8);
    this->setValue(QVariant::fromValue(value));
    this->setKey(key);
    emit keyChanged();
    emit stringChanged();
}

void QMLElement::setOidData(const QString &key, const QString &oid)
{
    this->setOid(QOid(oid),key);
    emit keyChanged();
    emit oidChanged();
}

void QMLElement::setInt32Data(const QString &key, const int &value)
{
    this->setType(QElementType::b_int32);
    this->setValue(QVariant::fromValue(value));
    this->setKey(key);
    emit intChanged();
    emit keyChanged();

}

void QMLElement::setInt64Data(const QString &key, const qint64 &value)
{
    this->setType(QElementType::b_int64);
    this->setValue(QVariant::fromValue(value));
    this->setKey(key);
    emit keyChanged();
    emit int64Changed();
}

void QMLElement::setDoubleData(const QString &key, const double &value)
{
    this->setType(QElementType::b_double);
    this->setValue(QVariant::fromValue(value));
    this->setKey(key);
    emit doubleChanged();
    emit keyChanged();
}

void QMLElement::setBoolData(const QString &key, const bool &value)
{
    this->setType(QElementType::b_bool);
    this->setValue(QVariant::fromValue(value));
    this->setKey(key);
    emit keyChanged();
}


void QMLElement::setData(const QString &key, const QString &value, const QMLElement::Type &type)
{
    QElement element(QElementType::b_null,QVariant::fromValue(value),key);

    switch ( type ) {
    case QMLElement::B_utf8:
        element.setType(QElementType::b_utf8);

        break;
    case QMLElement::B_oid:
        element.setType(QElementType::b_oid);
        emit oidChanged();
        break;
    default:
        break;
    }
    this->setElement(element);
    emit keyChanged();
}

void QMLElement::setData(const QString &key, const double &value)
{
    QElement element(QElementType::b_double,QVariant::fromValue(value),key);
    this->setElement(element);
    emit keyChanged();
}

void QMLElement::setData(const QString &key, const int &value )
{
    QElement element(QElementType::b_int32,QVariant::fromValue(value),key);
    this->setElement(element);
    emit intChanged();
    emit keyChanged();
}

void QMLElement::setData(const QString &key, const bool &value)
{
    QElement element(QElementType::b_bool,QVariant::fromValue(value),key);
    this->setElement(element);
    emit keyChanged();
}

void QMLElement::setData(const QVariant &element)
{
    QElement* element_ = element.value<QMLElement*>();

    this->setKey(element_->getKey());
    this->setType(element_->getType());
emit bsonChanged();
    switch (element_->getType()) {
    case QElementType::b_oid:
        this->setValue(QVariant::fromValue(element_->getOid().oid()));
        emit oidChanged();
        break;
    case QElementType::b_binary:
        this->setValue(QVariant::fromValue(element_->getBinary()));
        break;
    case QElementType::b_document:
        emit bsonChanged();
        break;
    case QElementType::b_int64:
        emit int64Changed();
        break;

    case QElementType::b_int32:
        emit intChanged();
        break;

    case QElementType::b_double:
        emit doubleChanged();
        break;
    case QElementType::b_array:
        emit arrayChanged();
        break;
    default:
        this->setValue(QVariant::fromValue(element_->getValue()));
        break;
    }
    emit keyChanged();
}



QElement QMLElement::getQElement() const
{
    QElement element = static_cast<QElement>(*this);
    return element;
}


QString QMLElement::Key() const
{
    return this->getKey();
}

double QMLElement::getDouble() const
{
    if( this->getType() == QElementType::b_double )
    {
        return this->getValue().toDouble();
    }else{
        return 0;
    }
}

QString QMLElement::getString() const
{
    if( this->getType() == QElementType::b_utf8 )
    {
        return this->getValue().toString();
    }else{
        return "";
    }
}

int QMLElement::getInt() const
{
    if( this->getType() == QElementType::b_int32 )
    {
        return this->getValue().toInt();
    }else{
        return 0;
    }
}

QVariant QMLElement::getInt64() const
{
    if( this->getType() == QElementType::b_int64 )
    {
        return this->getValue();
    }else{
        return 0;
    }
}

bool QMLElement::getBool() const
{
    if( this->getType() == QElementType::b_bool )
    {
        return this->getValue().toBool();
    }else{
        return false;
    }
}

QString QMLElement::Oid() const
{
    if( this->getType() == QElementType::b_oid )
    {
        try {
            return this->getOid().oid();
        } catch (QError &e) {
            return e.what();
        }
    }else{
        return "nullptr";
    }

}

QMLBSON* QMLElement::getBson() const
{
    if( this->getType() == QElementType::b_document )
    {
        QMLBSON* bson = new QMLBSON(this->toDocument());
        return bson;
    }else{
        return new QMLBSON();
    }
}

QMLArray *QMLElement::getArray() const
{
    if( this->getType() == QElementType::b_array )
    {
        auto array = new QMLArray(this->toArray());
        return array;
    }else{
        return new QMLArray();
    }
}



QString QMLElement::TypeName() const
{
    switch (this->getType()) {
    case QElementType::b_double:
        return "b_double";
        break;
    case QElementType::b_utf8:
        return "b_utf8";
        break;
    case QElementType::b_oid:
        return "b_oid";
        break;
    case QElementType::b_int64:
        return "b_int64";
        break;
    case QElementType::b_int32:
        return "b_int32";
        break;
    case QElementType::b_document:
        return "b_document";
        break;
    case QElementType::b_array:
        return "b_array";
        break;
    case QElementType::b_bool:
        return "b_bool";
        break;
    case QElementType::b_null:
        return "b_null";
        break;
    case QElementType::b_invalid:
        return "b_invalid";
        break;
    case QElementType::b_binary:
        return "b_binary";
        break;
    default:
        return "Unknown";
        break;
    }
}

QMLElement::Type QMLElement::getElementType() const
{
    switch (this->getType()) {
    case QElementType::b_double:
        return B_double;
        break;
    case QElementType::b_utf8:
        return B_utf8;
        break;
    case QElementType::b_oid:
        return B_oid;
        break;
    case QElementType::b_int64:
        return B_int64;
        break;
    case QElementType::b_int32:
        return B_int32;
        break;
    case QElementType::b_document:
        return B_document;
        break;
    case QElementType::b_array:
        return B_array;
        break;
    case QElementType::b_bool:
        return B_bool;
        break;
    case QElementType::b_null:
        return B_null;
        break;
    case QElementType::b_invalid:
        return B_invalid;
        break;
    case QElementType::b_binary:
        return B_binary;
        break;
    default:
        return B_invalid;
        break;
    }
}

QElementType QMLElement::ConvertType(QMLElement::Type type)
{
    switch ( type ) {
    case QMLElement::B_double:
        return QElementType::b_double;
        break;
    case QMLElement::B_utf8:
        return QElementType::b_utf8;
        break;
    case QMLElement::B_oid:
        return QElementType::b_oid;
        break;
    case QMLElement::B_int64:
        return QElementType::b_int64;
        break;
    case QMLElement::B_int32:
        return QElementType::b_int32;
        break;
    case QMLElement::B_document:
        return QElementType::b_document;
        break;
    case QMLElement::B_array:
        return QElementType::b_array;
        break;
    case QMLElement::B_bool:
        return QElementType::b_bool;
        break;
    case QMLElement::B_null:
        return QElementType::b_null;
        break;
    case QMLElement::B_invalid:
        return QElementType::b_invalid;
        break;
    case QMLElement::B_binary:
        return QElementType::b_binary;
        break;
    default:
        return QElementType::b_invalid;
        break;
    }
}





void QMLElement::setElement(const QElement &element)
{

    if( element.getType() == QElementType::b_oid )
    {

        this->setOid(QOid(element.getOid().oid()),element.getKey());
    }else{

        this->setKey(element.getKey());

        this->setType(element.getType());

        switch (element.getType()) {

        case QElementType::b_binary:
            this->setValue(QVariant::fromValue(element.getBinary()));
            break;

        default:
            this->setValue(QVariant::fromValue(element.getValue()));
            break;

        }
    }

    emit keyChanged();

    switch ( element.getType() ) {

    case QElementType::b_utf8:
        emit stringChanged();
        break;

    case QElementType::b_oid:
        emit oidChanged();
        break;

    case QElementType::b_document:
        emit bsonChanged();
        break;

    case QElementType::b_int64:
        emit int64Changed();
        break;

    case QElementType::b_int32:
        emit intChanged();
        break;

    case QElementType::b_double:
        emit doubleChanged();
        break;

    case QElementType::b_array:
        emit arrayChanged();
        break;

    default:
        break;

    }

}
