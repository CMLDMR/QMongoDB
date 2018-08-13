#include "qbson.h"

#include <QDebug>


void consoleLog( std::string &stream , QBSON obj );
void consoleLog( std::string &stream , QArray array  );


QBSON::QBSON()
{

}

QBSON::QBSON(const QBSON &obj)
{
    this->maplist.clear();
    this->maplist.append(obj.getMaplist());
}



QBSON::QBSON(QBSON &obj)
{
    this->maplist.clear();
    this->maplist.append(obj.getMaplist());
}

QBSON::QBSON(QBSON &&obj)
{
    this->maplist.clear();
    this->maplist.append(obj.getMaplist());
}

void QBSON::append(QString key, QVariant value, QElementType type)
{


    switch ( type ) {
    case QElementType::b_oid:
    {
        QElement var( type , QOid( value.toString() ) , key );
        maplist.push_back(var);
    }
        break;

    case QElementType::b_int32:
    {
        QElement var( type , std::int32_t( value.toInt() ) , key );
        maplist.push_back(var);
    }
        break;

    case QElementType::b_int64:
    {
        QElement var( type , std::int64_t( value.toLongLong() ) , key );
        maplist.push_back(var);
    }
        break;

    default:
    {
        QElement var( type , value , key );
        maplist.push_back(var);
    }
        break;
    }

}

void QBSON::append(QString key, QOid oid)
{
    QElement var( QElementType::b_oid , oid , key );
    maplist.push_back(var);
}

void QBSON::append(std::string key, QOid oid)
{
    QElement var( QElementType::b_oid , oid , key.c_str() );
    maplist.push_back(var);
}

void QBSON::append(const char *key, QOid oid)
{
    QElement var( oid , key );
    maplist.append(var);
}

bool QBSON::isEmpty() const
{
    if( this->maplist.isEmpty() )
    {
        return true;
    }else{
        return false;
    }
}

void QBSON::append(QString key, QVariant value)
{
    QElement var(QElementType::b_null);

    switch ( value.type() ) {
    case QVariant::Double:
        var.setType( QElementType::b_double );
        var.setKey( key );
        var.setValue( value.toDouble() );
        break;
    case QVariant::String:
        var.setType( QElementType::b_utf8 );
        var.setKey( key );
        var.setValue( value.toString() );
        break;
    case QVariant::Int:
        var.setType( QElementType::b_int32 );
        var.setKey( key );
        var.setValue( value.toInt() );
        break;
    case QVariant::LongLong:
        var.setType( QElementType::b_int64 );
        var.setKey( key );
        var.setValue( value.toLongLong() );
        break;
    case QVariant::Bool:
        var.setType( QElementType::b_bool );
        var.setKey( key );
        var.setValue( value.toBool() );
        break;
    case QVariant::Invalid:
        if( this->Keys().contains(key) ){
            throw QError(QString("invalid type does not accept"));
        }
        break;
    default:
        var.setType( QElementType::b_invalid );
        var.setKey( key );
        var.setValue( value );
        break;
    }
    maplist.push_back(var);

}

void QBSON::append(std::string key, std::string str)
{
    QElement var(QElementType::b_utf8,str.c_str(),key.c_str());
    maplist.push_back(var);
}

void QBSON::append(std::string key, QString str)
{
    QElement var(QElementType::b_utf8,str.toStdString().c_str(),key.c_str());
    maplist.push_back(var);
}

void QBSON::append(std::string key, const char *str)
{
    QElement var(QElementType::b_utf8,str,key.c_str());
    maplist.push_back(var);
}

void QBSON::append(QString key, std::string str)
{
    QElement var(QElementType::b_utf8,str.c_str(),key);
    maplist.push_back(var);
}

void QBSON::append(QString key, QString str)
{
    QElement var(QElementType::b_utf8,str.toStdString().c_str(),key);
    maplist.push_back(var);
}

void QBSON::append(QString key, const char *str)
{
    QElement var(QElementType::b_utf8,str,key);
    maplist.push_back(var);
}

void QBSON::append(const char *key, std::string str)
{
    QElement var(QElementType::b_utf8,str.c_str(),key);
    maplist.push_back(var);
}

void QBSON::append(const char *key, QString str)
{
    QElement var(QElementType::b_utf8,str.toStdString().c_str(),key);
    maplist.push_back(var);
}

void QBSON::append(const char *key, const char *str)
{
    QElement var(QElementType::b_utf8,str,key);
    maplist.push_back(var);
}

void QBSON::append(std::string key, double value)
{
    QElement var(QElementType::b_double,value,key.c_str());
    maplist.push_back(var);
}

void QBSON::append(QString key, double value)
{
    QElement var(QElementType::b_double,value,key);
    maplist.push_back(var);
}

void QBSON::append(const char *key, double value)
{
    QElement var(QElementType::b_double,value,key);
    maplist.push_back(var);
}

void QBSON::append(QElement element)
{
    maplist.push_back(element);
}

void QBSON::append(QString key, QBSON value )
{
    QElement var(QElementType::b_document);
    var.setKey( key );
    var.setValue( QVariant::fromValue(value) );
    maplist.push_back(var);
}

void QBSON::append(std::string key, QBSON value)
{
    QElement var(QElementType::b_document);
    var.setKey( key.c_str() );
    var.setValue( QVariant::fromValue(value) );
    maplist.push_back(var);
}

void QBSON::append(const char *key, QBSON value)
{
    QElement var(QElementType::b_document);
    var.setKey( key );
    var.setValue( QVariant::fromValue(value) );
    maplist.push_back(var);
}

void QBSON::append(QString key, QArray value)
{
    QElement var(QElementType::b_array);
    var.setKey( key );
    var.setValue( QVariant::fromValue(value) );
    maplist.push_back(var);
}

QVector<QElement> QBSON::getMaplist() const
{
    return maplist;
}

QBSON& QBSON::operator=(const QBSON& obj)
{
    this->clear();
    this->append(obj.getMaplist());
    return *this;
}


QElement QBSON::operator[](const QString key)
{
    if( !this->Keys().contains(key) ){
        throw QError(QString("%1 is not exist!").arg(key));
    }

    QElement str(QElementType::b_null);
    for( auto doc : this->maplist )
    {
        if( key == doc.getKey() )
        {
            str = doc;
            break;
        }
    }
    return str;
}

QElement QBSON::operator[](std::string key)
{
    if( !this->Keys().contains(key.c_str()) ){
        throw QError(QString("%1 is not exist!").arg(key.c_str()));
    }
    QElement str(QElementType::b_null);
    for( auto doc : this->maplist )
    {
        if( key.c_str() == doc.getKey() )
        {
            str = doc;
            break;
        }
    }
    return str;
}

QElement QBSON::operator[](const char *key)
{

    if( !this->Keys().contains(key) ){
        throw QError(QString("%1 is not exist!").arg(key));
    }

    QElement str(QElementType::b_null);
    for( auto doc : this->maplist )
    {
        if( key == doc.getKey() )
        {
            str = doc;
            break;
        }
    }
    return str;
}

const QStringList QBSON::Keys()
{
    QStringList strList;
    for( auto doc : this->maplist)
    {
        strList.append(doc.getKey());
    }
    return strList;
}






void QBSON::clear()
{
    this->maplist.clear();
}

void QBSON::append(QVector<QElement> mlist)
{
    this->maplist.append(mlist);
}



QString QBSON::TypeToString(QElementType type)
{

    switch (type) {
    case QElementType::b_double:
        return "double";
        break;
    case QElementType::b_utf8:
        return "utf8";
        break;
    case QElementType::b_oid:
        return "oid";
        break;
    case QElementType::b_array:
        return "array";
        break;
    case QElementType::b_int64:
        return "int64";
        break;
    case QElementType::b_int32:
        return "int32";
        break;
    case QElementType::b_document:
        return "document";
        break;
    case QElementType::b_bool:
        return "bool";
        break;
    case QElementType::b_null:
        return "null";
        break;
    case QElementType::b_invalid:
        return "invalid";
        break;
    default:
        break;
    }



}

std::string QBSON::tojson()
{
    std::string str;
    str += "{ ";
    consoleLog(str,*this);
    str += " }";
    return str;
}








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
    this->setValue( element.getValue() );
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
        throw QError("element is not b_oid");
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
        throw QError("element is not b_oid");
    }
}



int QArray::count() const
{
    return mapData.count();
}

void QArray::append(QString str)
{
    QElement element(QElementType::b_utf8);
    element.setValue( str );
    this->mapData.append(element);
}

void QArray::append(double val)
{
    QElement element(QElementType::b_double);
    element.setValue( val );
    this->mapData.append(element);}

void QArray::append(bool val)
{
    QElement element(QElementType::b_bool);
    element.setValue( val );;
    this->mapData.append(element);
}

void QArray::append(qint32 val)
{
    QElement element(QElementType::b_int32);
    element.setValue( val );
    this->mapData.append(element);
}

void QArray::append(qint64 val)
{
    QElement element(QElementType::b_int64);
    element.setValue( val );
    this->mapData.append(element);
}

void QArray::append(QBSON obj)
{
    QElement element(QElementType::b_document);
    element.setValue( QVariant::fromValue( obj ) );
    this->mapData.append(element);
}

void QArray::append(QArray array)
{
    QElement element(QElementType::b_array);
    element.setValue( QVariant::fromValue(array) );
    this->mapData.append(element);
}

void QArray::append(QElement element)
{
    if( element.getType() == QElementType::b_invalid )
    {
        throw "Element is Invalid";
        return;
    }

    this->mapData.append(element);
}

void QArray::append(QOid oid)
{
    QElement element(QOid(oid),"");
    this->mapData.append(element);
}

QProjection::QProjection()
{



}

void QProjection::set(QString key, bool visible)
{
    this->bson.append(key,visible);
}

QBSON QProjection::getBson() const
{
    QBSON bson;
    bson.append(this->bson.getMaplist());
    return bson;
}

QSort::QSort()
{

}

void QSort::sortByAscending(QString key)
{
    this->bson.append(key,1,QElementType::b_int32);
}

void QSort::sortByDescending(QString key)
{
    this->bson.append(key,-1,QElementType::b_int32);
}

QBSON QSort::getBson() const
{
    return bson;
}

QOption::QOption()
{

}

QOption::QOption(const QOption &option)
{
    this->bson.clear();
    this->bson.append(option.getBson().getMaplist());
}

QOption::QOption(QOption &option)
{
    this->bson.clear();
    this->bson.append(option.getBson().getMaplist());
}

QOption::QOption(QOption &&option)
{
    this->bson.clear();
    this->bson.append(option.getBson().getMaplist());
}

void QOption::setProjection(const QProjection &projection)
{
    this->bson.append("projection",projection.getBson());
}

void QOption::setSort(const QSort &sort)
{
    this->bson.append("sort",sort.getBson());
}

void QOption::setSkip(int skip)
{
    this->bson.append("skip",skip);
}

void QOption::setLimit(int limit)
{
    this->bson.append("limit",limit);
}

QBSON QOption::getBson() const
{
    return bson;
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



void consoleLog( std::string &stream , QBSON obj ){


    for( auto element : obj )

    {
        switch ( element.getType() ) {
        case QElementType::b_bool:
        {
            if( element.getValue().toBool() )
            {
                stream += "\""+element.getKey().toStdString() + "\" : true , ";

            }else{
                stream += "\""+element.getKey().toStdString() + "\" : false , ";

            }
        }
            break;
        case QElementType::b_double:
            stream += "\""+element.getKey().toStdString() + "\" : " +  std::to_string(element.getValue().toDouble())+ " , ";
            break;
        case QElementType::b_int32:
            stream += "\""+element.getKey().toStdString() +"\" : "+ std::to_string( element.getValue().toInt() )+ " , ";
            break;
        case QElementType::b_int64:
            stream += "\""+element.getKey().toStdString() +"\" : "+ std::to_string(element.getValue().toLongLong() ) + " , ";
            break;
        case QElementType::b_oid:
            stream += "\""+element.getKey().toStdString() +"\" : { \"QOid\" : \""+ element.getOid().oid().toStdString()+ "\" } , ";
            break;
        case QElementType::b_utf8:
            stream += "\""+element.getKey().toStdString() +"\" : \"" + element.getValue().toString().toStdString() + "\"" + " , ";
            break;
        case QElementType::b_document:
            stream += "\""+element.getKey().toStdString() +"\" : "+ "{ ";
            consoleLog(stream,element.toDocument());
            stream += "} , ";
            break;
        case QElementType::b_array:
            stream += "\""+element.getKey().toStdString() +"\" : "+ "[ ";
            consoleLog(stream,element.toArray());
            stream += "] , ";
            break;
        default:
            break;
        }
    }
    return;
}


void consoleLog(std::string &stream , QArray array ){


    for( auto element : array ){

        switch ( element.getType() ) {
        case QElementType::b_bool:
        {
            if( element.getValue().toBool() )
            {
                stream += "true, ";
            }else{
                stream += "false, ";
            }
        }
            break;
        case QElementType::b_double:
            stream += std::to_string(element.getValue().toDouble())+ " , ";
            break;
        case QElementType::b_int32:
            stream += std::to_string( element.getValue().toInt() )+ " , ";
            break;
        case QElementType::b_int64:
            stream += std::to_string(element.getValue().toLongLong() ) + " , ";
            break;
        case QElementType::b_oid:
            stream += "{ \"QOid\" : \""+ element.getOid().oid().toStdString()+ "\" } , ";
            break;
        case QElementType::b_utf8:
            stream += "\"" + element.getValue().toString().toStdString() + "\"" + " , ";
            break;
        case QElementType::b_document:
            stream += "{";
            consoleLog(stream,element.toDocument());
            stream += "} , ";
            break;
        case QElementType::b_array:
            stream += "[";
            consoleLog(stream,element.toArray());
            stream += "] , ";
            break;
        default:
            break;
        }
    }

    return;
}

