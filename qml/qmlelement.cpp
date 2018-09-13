#include "qmlelement.h"
#include "qmlbson.h"

QMLElement::QMLElement(QObject *parent)
    : QObject(parent)
{

}


QMLElement::QMLElement(const QMLElement& element)
{
    QElement _element( this->getType(),element.getValue(),element.getKey() );
    this->setElement(_element);
}

QMLElement::QMLElement(const QElement &element)
{
    this->setElement(element);
}

QMLElement::QMLElement(const QElementType &type, const QString &key, const QString &value)
{
    qDebug() << static_cast<int>(type) << key << value;

    switch ( type ) {
    case QElementType::b_utf8:
    {
        QElement elemet(type,QVariant::fromValue(value),key);
        this->setElement(elemet);
    }
        break;
    case QElementType::b_oid:
    {
        QElement elemet(QOid(value),key);
        this->setElement(elemet);
    }
        break;
    default:
        break;
    }
//    this->setKey( key );
//    this->setType( type );
//    this->setValue( value );
}

QVariant QMLElement::newElement()
{
    QMLElement* element = new QMLElement();
    return QVariant::fromValue(element);
}

QVariant QMLElement::newElement(const QMLElement::Type &type, const QString &key, const QString &value)
{
    qDebug() << type << key << value;

    QMLElement* element = new QMLElement(QMLElement::ConvertType(type),key,value);
    return QVariant::fromValue( element );

}

void QMLElement::setData(const QString &key, const QString &value, const QMLElement::Type &type)
{
    qDebug() << "setData Call QString";
    QElement element(QElementType::b_null,QVariant::fromValue(value),key);

    switch ( type ) {
    case QMLElement::B_utf8:
        element.setType(QElementType::b_utf8);
        break;
    case QMLElement::B_oid:
        element.setType(QElementType::b_oid);
        break;
    default:
        break;
    }

    this->setElement(element);
}

void QMLElement::setData(const QString &key, const double &value)
{
    qDebug() << "setData Call double";
    QElement element(QElementType::b_double,QVariant::fromValue(value),key);
    this->setElement(element);
}

void QMLElement::setData(const QString &key, const int &value , const Type& type)
{
    qDebug() << "setData Call int";
    QElement element(QElementType::b_int32,QVariant::fromValue(value),key);
    this->setElement(element);
}

void QMLElement::setData(const QString &key, const bool &value)
{
    qDebug() << "setData Call bool";
    QElement element(QElementType::b_bool,QVariant::fromValue(value),key);
    this->setElement(element);
}

void QMLElement::setData(const QVariant &element)
{
    QElement* element_ = element.value<QMLElement*>();

    this->setKey(element_->getKey());
    this->setType(element_->getType());

    switch (element_->getType()) {
    case QElementType::b_oid:
        this->setValue(QVariant::fromValue(element_->getOid().oid()));
        break;
    case QElementType::b_binary:
        this->setValue(QVariant::fromValue(element_->getBinary()));
        break;
    default:
        this->setValue(QVariant::fromValue(element_->getValue()));
        break;
    }
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
//        throw QError(QString("this element is not double type, Key is ")+this->Key());
        qDebug() << QString("this element is not double type, Key is ")+this->Key();
        return 0;
    }
}

QString QMLElement::getString() const
{
    if( this->getType() == QElementType::b_utf8 )
    {
        return this->getValue().toString();
    }else{
        qDebug() << "this element is not String type, Key is " << this->Key();
//        throw QError(QString("this element is not String type, Key is ")+this->Key() );
        return "";
    }
}

int QMLElement::getInt() const
{
    if( this->getType() == QElementType::b_int32 )
    {
        return this->getValue().toInt();
    }else{
//        throw QError(QString("this element is not int32 type, Key is ")+this->Key());
        qDebug() << QString("this element is not int32 type, Key is ")+this->Key();
        return 0;
    }
}

bool QMLElement::getBool() const
{
    if( this->getType() == QElementType::b_bool )
    {
        return this->getValue().toBool();
    }else{
//        throw QError(QString("this element is not Bool type, Key is ")+this->Key());
        qDebug() << QString("this element is not Bool type, Key is ")+this->Key();
        return false;
    }
}

QString QMLElement::Oid() const
{
    qDebug() << static_cast<int>(this->getType());
//    qDebug() << this->getValue();
    if( this->getType() == QElementType::b_oid )
    {
        qDebug() << "type is oid";
        try {
//            qDebug() << this->getOid().oid();
            return this->getOid().oid();
        } catch (QError &e) {
            return e.what();
        }


    }else{
//        throw QError(QString("this element is not Oid type, Key is ")+this->Key());
        qDebug() << QString("this element is not Oid type, Key is ")+this->Key();
        return "nullptr";
    }

}

QMLBSON QMLElement::getBson() const
{
    if( this->getType() == QElementType::b_document )
    {
        try {
            return this->toDocument();
        } catch (QError &e) {
            return QMLBSON();
        }
    }else{
        qDebug() << QString("this element is not b_document type, Key is ")+this->Key();
        return QMLBSON();
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

}
