#ifndef QBSON_H
#define QBSON_H

#if defined(QMONGODB_LIBRARY)
#  define QMONGODBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QMONGODBSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QVariant>
#include <QVector>
#include <type_traits>
#include <iostream>
#include <QDataStream>

#include "qerror.h"
#include "qelement.h"



class QBSON;

class QMONGODBSHARED_EXPORT QArray
{
    using container = QVector<QElement>;
public:
    QArray() {}

    int count() const;

    void clear();

    void append(QString str);
    void append(double val);
    void append(bool val);
    void append(qint32 val);
    void append(qint64 val);
    void append(QBSON obj);
    void append(QArray array);
    void append(QElement element);
    void append(QOid oid);
    void append(QByteArray binary);

    QElement operator[](const int index);

    QElement value(const int index);
    const QElement const_value(const int index) const;



    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    iterator begin() { return mapData.begin(); }
    iterator end() { return mapData.end(); }
    const_iterator cbegin() const { return mapData.cbegin(); }
    const_iterator cend() const { return mapData.cend(); }

private:
    QVector<QElement> mapData;
};


static void inStream(QDataStream& in , const QElement &element);
static void outStream(QDataStream& out , QArray &array);



class QMONGODBSHARED_EXPORT QBSON
{

    using container = QVector<QElement>;

    using self = QBSON;


public:
    explicit QBSON();
    QBSON(QBSON const &obj);
    QBSON(QBSON& obj);
    QBSON(QBSON&& obj);
    virtual ~QBSON() {}

    void append(QString key , QVariant value , QElementType type);

    // COMPOSITE BUILDER
    self& insert( QString key , QOid oid );
    self& insert( QString key , QString value );
    self& insert( QString key , double value );
    self& insert( QString key , qint64 value );
    self& insert( QString key , bool value );
    self& insert( QString key , int value );
    self& insert( QString key , QBSON value );
    self& insert( QString key , QArray value );
    self& insert( QString key , QByteArray binary );

    void append(QString key , QOid oid );
    void append(std::string key , QOid oid );
    void append(const char* key , QOid oid );




    ///
    /// \brief isEmpty : if QBSON Object is Empty return true else false
    /// \return
    ///
    bool isEmpty() const;

    ///
    /// \brief append only bool double string int qlonglong
    ///  throw std::exception if QElement can not initilazed
    /// \param key
    /// \param value
    ///
    void append(QString key , QVariant value );


    void append(std::string key , std::string str);
    void append(std::string key , QString str);
    void append(std::string key , const char* str);

    void append(QString key , std::string str);
    void append(QString key , QString str);
    void append(QString key , const char* str);

    void append(const char* key , std::string str);
    void append(const char* key , QString str);
    void append(const char* key , const char* str);

    void append(std::string key , double value);
    void append(QString key , double value);
    void append(const char* key , double value);

    void append(std::string key , qint64 value);
    void append(QString key , qint64 value);
    void append(const char* key , qint64 value);

    void append(std::string key , bool value);
    void append(QString key , bool value);
    void append(const char* key , bool value);

    void append(std::string key , int value);
    void append(QString key , int value);
    void append(const char* key , int value);

    void append(std::string key , QByteArray binary);
    void append(QString key , QByteArray binary);
    void append(const char* key , QByteArray binary);



    void append(QElement element );

    ///
    /// \brief append : throw std::exception if key does not exist
    /// \param key
    /// \param value
    ///

    void append(QString key , QBSON value );
    void append(std::string key , QBSON value);
    void append(const char* key , QBSON value);


    void append(QString key , QArray value );
    void append(std::string key , QArray value );
    void append(const char* key , QArray value );

    QVector<QElement> getMaplist() const;

    QBSON& operator=(const QBSON& obj);

    ///
    /// \brief operator [] : throw QError if key is not exist
    /// \param key
    /// \return
    ///
    QElement operator[](const QString key);

    ///
    /// \brief operator [] : throw QError if key is not exist
    /// \param key
    /// \return
    ///
    QElement operator[](std::string key);

    ///
    /// \brief operator [] : throw QError if key is not exist
    /// \param key
    /// \return
    ///
    QElement operator[](const char* key);

    ///
    /// \brief value: throw QError if key is not exist
    /// \param key
    /// \return
    ///
    QElement value( const QString& key );

    ///
    /// \brief removeElement
    /// \param key
    /// \return
    /// Remove First Element that's key = "key"
    bool removeElement( const QString& key );



    const QStringList Keys();


    using iterator = typename container::iterator;

    using const_iterator = typename container::const_iterator;

    iterator begin() { return maplist.begin(); }

    iterator end() { return maplist.end(); }

    const_iterator cbegin() const { return maplist.cbegin(); }

    const_iterator cend() const { return maplist.cend(); }

    void clear();

    void append(QVector<QElement> mlist);

    static QString TypeToString(QElementType type);

    std::string tojson() const;




    friend QDataStream& operator<<(QDataStream& in , const QBSON& bson)
    {
        in.setVersion(QDataStream::Version::Qt_5_10);
        in << bson.getMaplist().count();
        for( int i = 0 ; i < bson.getMaplist().count() ; i++ )
        {

            auto element = bson.getMaplist().at(i);
            in << static_cast<int>(element.getType());
            if( element.getType() == QElementType::b_document )
            {
                auto _bson = element.toDocument();
                in << element.getKey();
                in << _bson;
            }else if( element.getType() == QElementType::b_array ) {

                inStream(in,element);

            } else {
                in << element;
            }

        }
        return in;
    }

    friend QBSON& operator>>(QDataStream& out,QBSON& bson)
    {
        out.setVersion(QDataStream::Version::Qt_5_10);
        int count;
        out >> count;
        bson.clear();
        for( int i = 0 ; i < count ; i++ )
        {
            int type;
            out >> type;
            QElementType _type = static_cast<QElementType>(type);
            QElement element;


            if( _type == QElementType::b_document )
            {
                QString key;
                QBSON _bson;
                out >> key;
                out >> _bson;
                bson.append( key , _bson );
            }else if(_type == QElementType::b_array ){
                QString key;
                out >> key;
                QArray array;
                outStream(out,array);
                bson.append( key , array );
            }else{
                out >> element;
                bson.append( element );
            }


        }
        return bson;
    }

private:
    QVector<QElement> maplist;

};


static void inStream(QDataStream& in , const QElement &element)
{

    auto array = element.toArray();
    in << element.getKey();
    in << array.count();
    for( auto _element : array )
    {
        in << static_cast<int>(_element.getType());
        if( _element.getType() == QElementType::b_document )
        {
            auto _bson = _element.toDocument();
            in << _element.getKey();
            in << _bson;
        }else if( _element.getType() == QElementType::b_array ) {

            inStream(in,_element);

        } else {
            in << _element;
        }
    }

}


static void outStream(QDataStream& out , QArray &array)
{
    out.setVersion(QDataStream::Version::Qt_5_10);
    int count;
    out >> count;
    QArray _array;
    for( int i = 0 ; i < count ; i++ )
    {
        int _type;
        out >> _type;
        QElementType type = static_cast<QElementType>(_type);

        if( type == QElementType::b_document )
        {
            QString key;
            QBSON _bson;
            out >> key;
            out >> _bson;
            array.append( _bson );
        }else if (type == QElementType::b_array ) {
            QString key;
            out >> key;
            QArray array;
            outStream(out,array);
            array.append( array );
        } else {
            QElement element;
            out >> element;
            array.append( element );
        }

    }
}



class QMONGODBSHARED_EXPORT QProjection
{
public:
    QProjection();

    void set( QString key , bool visible );

    QBSON getBson() const;

private:
    QBSON bson;
};


class QMONGODBSHARED_EXPORT QSort
{
public:
    QSort();

    void sortByAscending(QString key);

    void sortByDescending(QString key);

    QBSON getBson() const;

private:
    QBSON bson;
};

class QMONGODBSHARED_EXPORT QOption
{
public:
    explicit QOption();
    QOption(QOption const &option);
    QOption(QOption& option);
    QOption(QOption&& option);
    virtual ~QOption() {}

    void setProjection(const QProjection &projection);

    void setSort(const QSort &sort);

    void setSkip(int skip);

    //TODO: setlimit 2 defa çağrılınca iki tane "limit":x oluşuyor
    void setLimit(int limit);


    QBSON getBson() const;

    void setBson(const QBSON &value);

private:
    QBSON bson;
};










Q_DECLARE_METATYPE(QElement);
Q_DECLARE_METATYPE(QArray);
Q_DECLARE_METATYPE(QBSON);


#endif // QBSON_H
