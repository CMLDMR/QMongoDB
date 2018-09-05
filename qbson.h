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

#include "qerror.h"




class QBSON;
class QArray;

enum class QElementType
{
    b_double = 0,
    b_utf8,
    b_oid,
    b_int64,
    b_int32,
    b_document,
    b_array,
    b_bool,
    b_null,
    b_invalid,
    b_binary
};


class QMONGODBSHARED_EXPORT QOid
{
    QString mOid;
public:

    explicit QOid();
    QOid(QString oid);
    QOid(QOid const &oid);
    QOid(QOid& oid);
    QOid(QOid&& oid);
    ~QOid() {}

    QString oid() const;

    QOid& operator=(const QOid& oid);
};

Q_DECLARE_METATYPE(QOid);
//Q_DECLARE_METATYPE(int64_t);
//Q_DECLARE_METATYPE(int32_t);



class QMONGODBSHARED_EXPORT QElement{
public:
    explicit QElement(QElementType type_ , QVariant value_ = QVariant() , QString key_ = "" );
    QElement(QElementType type_ , QOid oid , QString key );
    QElement( QOid oid , QString key );
    QElement();
    QElement(QElement const &element);
    QElement(QElement& element);
    QElement(QElement&& element);
    ~QElement() {}

    QBSON toDocument() const;
    QArray toArray() const;

    QElement& operator=(const QElement& element);

    QString getKey() const;
    void setKey(const QString &value);

    QVariant getValue() const;
    void setValue(const QVariant &value);

    QElementType getType() const;
    void setType(const QElementType &value);

    ///
    /// \brief getOid : return Oid if it's type oid else throw QError
    /// \return
    ///
    QOid getOid() const;

    QByteArray getBinary() const;

private:
    QString key;
    QVariant val;
    QElementType type;

};

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

    std::string tojson();

private:
    QVector<QElement> maplist;

};




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
    void setLimit(int limit);


    QBSON getBson() const;

private:
    QBSON bson;
};

class QMONGODBSHARED_EXPORT QArray
{
public:
    QArray() {}

    int count() const;

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


    using container = QVector<QElement>;

    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    iterator begin() { return mapData.begin(); }
    iterator end() { return mapData.end(); }
    const_iterator cbegin() const { return mapData.cbegin(); }
    const_iterator cend() const { return mapData.cend(); }

private:
    QVector<QElement> mapData;
};


Q_DECLARE_METATYPE(QElement);
Q_DECLARE_METATYPE(QArray);
Q_DECLARE_METATYPE(QBSON);


#endif // QBSON_H
