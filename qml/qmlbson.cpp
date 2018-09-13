#include "qmlbson.h"

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

QVariant QMLBSON::newBSON()
{
    QMLBSON* bson = new QMLBSON();
    return QVariant::fromValue(bson);
}

QMLBSON &QMLBSON::add(QString key, QString value, const QMLElement::Type &type)
{

    if( type == QMLElement::B_oid )
    {
        qDebug() << "insert Oid";
        this->append(key,QOid(value));
    }else{
        qDebug() << "insert QString Value";
        this->append(key,value);
        qDebug() << this->value(key).getValue() << value;
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
    qDebug() << "Call add bool";
    this->append(key,value);
}

QMLBSON &QMLBSON::add(QString key, QMLBSON* value)
{
    qDebug() << "Call add QMLBSON" << key << value->tojson().c_str();
    this->append(key,*value);
    qDebug() << "View added QMLBSON" << this->tojson().c_str()<<"\n";
}


QVariant QMLBSON::getElement(const QString &key)
{
    if( !this->Keys().contains(key) )
    {
        QString err = "bson document has no key: "+key;
        qDebug() << err;
        return QVariant();
    }else{
        QElement element = this->value(key);

        switch (element.getType()) {
        case QElementType::b_document:
        {
            qDebug() << "This is Bson Object. Use getBson function ("+key+")";
            return QVariant();
        }
            break;
        default:
        {
            QMLElement *_element = new QMLElement(element);
            return QVariant::fromValue(_element);
        }
            break;
        }
    }
}

QVariant QMLBSON::getBson(const QString &key)
{
    if( !this->Keys().contains(key) )
    {
        QString err = "bson document has no key: "+key;
        qDebug() << err;
        return QVariant();
    }else{
        QElement element = this->value(key);

        switch (element.getType()) {
        case QElementType::b_document:
        {
            qDebug() << element.toDocument().tojson().c_str();
            QMLBSON *_bson = new QMLBSON(element.toDocument());
            return QVariant::fromValue(_bson);
        }
            break;
        default:
        {
            qDebug() << "This is Not BSON object within Key: " + key ;
            return QVariant();
        }
            break;
        }
    }
}

