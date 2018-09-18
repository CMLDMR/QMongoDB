#ifndef QELEMENT_H
#define QELEMENT_H

#include <QtCore/qglobal.h>

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


class QBSON;
class QArray;

enum class QElementType : int
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
    bool valid;
public:

    ///
    /// \brief QOid
    /// no throw an exception
    explicit QOid();

    ///
    /// \brief QOid
    /// \param oid
    /// throw exception if QOid is not valid
    QOid(QString oid);
    ///
    /// \brief QOid
    /// \param oid
    /// throw exception if QOid is not valid
    QOid(QOid const &oid);
    ///
    /// \brief QOid
    /// \param oid
    /// throw exception if QOid is not valid
    QOid(QOid& oid);
    ///
    /// \brief QOid
    /// \param oid
    /// throw exception if QOid is not valid
    QOid(QOid&& oid);
    ~QOid() {}

    ///
    /// \brief oid
    /// \return
    /// throw exception if QOid is not valid
    QString oid() const;


    ///
    /// \brief operator =
    /// \param oid
    /// \return
    /// throw exception if QOid is not valid
    QOid& operator=(const QOid& oid);

    ///
    /// \brief isValid
    /// \return
    /// return true if QOid is valid else false
    bool isValid() const;
};

Q_DECLARE_METATYPE(QOid);



class QMONGODBSHARED_EXPORT QElement{


public:
    explicit QElement(QElementType type_ , QVariant value_ = QVariant() , QString key_ = "_" );
    QElement(QElementType type_ , QOid oid , QString key );
    QElement( QOid oid , QString key );
    QElement( QByteArray binary , QString key );
    QElement();
    QElement(QElement const &element);
    QElement(QElement& element);
    QElement(QElement&& element);
    ~QElement() {}

    ///
    /// \brief isValid
    /// \return
    /// if key is not null,val is not null or invalid , type is not equal QElementType==b_invalid return false else return true
    bool isValid() const;

    ///
    /// \brief setOid
    /// \param oid
    /// \param key
    ///
    void setOid(QOid oid , QString key);

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





    friend QDataStream& operator<<( QDataStream& in , const QElement& element )
    {
        in.setVersion(QDataStream::Version::Qt_5_10);
        in << static_cast<int>(element.getType());
        in << element.getKey();
        if( element.getType() == QElementType::b_oid )
        {
            in << element.getOid().oid();

        }else if ( element.getType() == QElementType::b_binary ) {
            in << element.getBinary();

        }else {
            in << element.getValue();
        }
        return in;
    }


    friend QElement& operator>>(QDataStream& out,QElement& element)
    {
        out.setVersion(QDataStream::Version::Qt_5_10);
        int type;
        QString key;
        QVariant value;
        out >> type;
        out >> key;

        element.setType(static_cast<QElementType>(type));
        element.setKey(key);

        if( type == static_cast<int>(QElementType::b_oid) )
        {
            QString oid;
            out >> oid;
            element = QElement(QOid(oid),key);
        }else if ( type == static_cast<int>(QElementType::b_binary) ) {
            QByteArray ar;
            out >> ar;
            element = QElement(QElementType::b_binary , ar , key );
        } else {
            out >> value;
            element.setValue(value);
        }


        return element;

    }

private:
    QString key;
    QVariant val;
    QElementType type;


};




#endif // QELEMENT_H
