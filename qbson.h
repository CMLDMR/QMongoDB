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
    b_invalid
};

class QMONGODBSHARED_EXPORT QElement{
public:
    explicit QElement(QElementType type_ , QVariant value_ = QVariant() , QString key_ = "" );
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

private:
    QString key;
    QVariant val;
    QElementType type;

};

class QMONGODBSHARED_EXPORT QBSON
{

public:
    explicit QBSON();
    QBSON(QBSON const &obj);
    QBSON(QBSON& obj);
    QBSON(QBSON&& obj);
    virtual ~QBSON() {}

    void append(QString key , QVariant value , QElementType type);

    ///
    /// \brief append only bool double string int qlonglong
    /// \param key
    /// \param value
    ///
    void append(QString key , QVariant value );

    virtual void append(QString key , QElement element );
    void append(QString key , QBSON value );
    void append(QString key , QArray value );

    QVector<QElement> getMaplist() const;

    QBSON& operator=(const QBSON& obj);

    QElement operator[](const QString key);

    const QStringList Keys();

    using container = QVector<QElement>;

    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    iterator begin() { return maplist.begin(); }
    iterator end() { return maplist.end(); }
    const_iterator cbegin() const { return maplist.cbegin(); }
    const_iterator cend() const { return maplist.cend(); }


    void clear();
    void append(QVector<QElement> mlist);

    static QString TypeToString(QElementType type);
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
