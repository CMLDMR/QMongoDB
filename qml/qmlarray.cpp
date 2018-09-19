#include "qmlarray.h"


QMLArray::QMLArray(QObject *parent) : QObject(parent)
{

}

QMLArray::QMLArray(const QMLArray &array)
{
    this->clear();
    for( auto element : array.getArray() )
    {
        this->append(element);
    }
}

QMLArray::QMLArray( QArray &array)
{
    this->clear();
    for( int i = 0 ; i < array.count() ; i++ )
    {
        this->append(array.value(i));
    }
}

QMLArray::QMLArray(const QArray &array)
{
    this->clear();
    for( int i = 0 ; i < array.count() ; i++ )
    {
        this->append(array.const_value(i));
    }
}

QMLArray &QMLArray::operator=(const QMLArray &array)
{
    this->clear();
    for( auto element : array.getArray() )
    {
        this->append(element);
    }
    return *this;
}

QMLArray &QMLArray::operator=(const QMLArray *array)
{
    this->clear();
    for( auto element : array->getArray() )
    {
        this->append(element);
    }
    return *this;
}



QMLArray*  QMLArray::newArray()
{
    QMLArray* array = new QMLArray();
    return array;
}

QMLArray::self &QMLArray::insertElement(QMLElement *element)
{
    this->append(element->getQElement());
    return *this;
}



QMLArray::self &QMLArray::insertInt( int value )
{
    this->append(QElement(QElementType::b_int32,value,"blank"));
    return *this;
}

QMLArray::self &QMLArray::insertDouble( double value )
{
    this->append(value);
    return *this;
}

QMLArray::self &QMLArray::insertBool( bool value )
{
    this->append(value);
    return *this;
}

QMLArray::self &QMLArray::insertString( QString value )
{
    this->append(value);
    return *this;
}

QMLArray::self &QMLArray::insertOid(QString oid)
{
    this->append(QOid(oid));
    return *this;
}

QMLArray::self &QMLArray::insertBson( QMLBSON* value )
{
    this->append(value->getQBSON());
    return *this;
}

QMLArray::self &QMLArray::insertArray(QMLArray *value)
{
    this->append(value->getArray());
    return *this;
}

QMLElement *QMLArray::getElement(const int &index)
{
    if( index >= this->count() || index < 0 )
    {
        return (new QMLElement());
    }else{
        return (new QMLElement(this->value(index)));
    }
}






QArray QMLArray::getArray() const
{
    return static_cast<QArray>(*this);
}

void QMLArray::setArray( QArray &array )
{
    this->clear();

    for( auto element : array )
    {
        this->append(element);
    }
}

