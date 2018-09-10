#include "qelement.h"
#include "qbson.h"

QElement::QElement(QElementType type_, QVariant value_, QString key_)
{
    this->key = key_;
    this->setValue( value_ );
    this->type = type_;
}

QElement::QElement(QElementType type_, QOid oid, QString key)
{
    this->key = key;
    this->setValue( QVariant::fromValue( oid ) );
    this->type = type_;
}

QElement::QElement(QOid oid, QString key)
{
    this->key = key;
    this->type = QElementType::b_oid;
    this->setValue( QVariant::fromValue( oid ) );
}

QElement::QElement(QByteArray binary, QString key)
{
    this->key = key;
    this->type = QElementType::b_binary;
    this->setValue( QVariant::fromValue(binary) );
}

QElement::QElement()
{
    this->key = "";
    this->type = QElementType::b_invalid;
    this->setValue( QVariant() );
}

QElement::QElement(const QElement &element)
{
    this->setKey( element.getKey() );
    this->setType( element.getType() );
    if( element.getType() == QElementType::b_oid )
    {
        this->setValue( QVariant::fromValue(element.getOid()) );
    }else{
        this->setValue( element.getValue() );
    }

}

QElement::QElement(QElement &element)
{
    this->setKey( element.getKey() );
    this->setType( element.getType() );
    if( element.getType() == QElementType::b_oid )
    {
        this->setValue( QVariant::fromValue(element.getOid()) );
    }else{
        this->setValue( element.getValue() );
    }
}

QElement::QElement(QElement &&element)
{
    this->setKey( element.getKey() );
    this->setType( element.getType() );
    if( element.getType() == QElementType::b_oid )
    {
        this->setValue( QVariant::fromValue(element.getOid()) );
    }else{
        this->setValue( element.getValue() );
    }
}

bool QElement::isValid() const
{
    if( this->key.isEmpty() )                   { qDebug() << "Key is Empty";  return false;}
    if( this->type == QElementType::b_invalid ) { qDebug() << "Type is inValid"; return false; }
    if( !this->val.isValid() )                  { qDebug() << "val is not Valid"; return false; }
    if( this->val.isNull() )                    { qDebug() << "val is null"; return false;}
    return true;
}

QBSON QElement::toDocument() const
{
    return this->getValue().value<QBSON>();
}

QArray QElement::toArray() const
{
    return this->getValue().value<QArray>();
}

QElement &QElement::operator=(const QElement &element)
{
    this->setKey( element.getKey() );
    this->setType( element.getType() );
    if( element.getType() == QElementType::b_oid )
    {
        this->setValue( QVariant::fromValue(element.getOid()) );
    }else{
        this->setValue( element.getValue() );
    }
    return *this;
}

QString QElement::getKey() const
{
    return key;
}

void QElement::setKey(const QString &value)
{
    key = value;
}

QVariant QElement::getValue() const
{
    if( this->type != QElementType::b_oid )
    {
        return val;
    }else{
        throw QError("expected QVariant but QOid");
    }

}

void QElement::setValue(const QVariant &value)
{
    val = value;
}

QElementType QElement::getType() const
{
    return type;
}

void QElement::setType(const QElementType &value)
{
    type = value;
}

QOid QElement::getOid() const
{
    if( this->getType() == QElementType::b_oid )
    {
        return this->val.value<QOid>();
    }else{
        throw QError(QString("element is not b_oid: ") + QBSON::TypeToString(this->getType()));
    }
}

QByteArray QElement::getBinary() const
{
    if( this->getType() == QElementType::b_binary )
    {
        return this->val.value<QByteArray>();
    }else{
        throw QError(QString("element is not b_binary: ") + QBSON::TypeToString(this->getType()));
    }
}






QString QOid::oid() const
{
    return mOid;
}

QOid &QOid::operator=(const QOid &oid)
{
    this->mOid = oid.oid();
    return *this;
}

QOid::QOid()
{

}

QOid::QOid( QString oid )
{
    this->mOid = oid;
}

QOid::QOid( const QOid &oid )
{
    this->mOid = oid.oid();
}

QOid::QOid( QOid &oid )
{
    this->mOid = oid.oid();
}

QOid::QOid( QOid &&oid )
{
    this->mOid = oid.oid();
}




