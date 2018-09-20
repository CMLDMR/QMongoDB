#ifndef QMLARRAY_H
#define QMLARRAY_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qbson.h"
#include "qmlelement.h"
#include "qmlbson.h"


class QMLArray : public QObject , public QArray
{
    Q_OBJECT

    /// \brief count
    /// \return
    /// Return count of Element in Array
    Q_PROPERTY(int count READ count )

    /// \brief
    /// \return
    /// Using with class name within self
    using self = QMLArray;
public:
    explicit QMLArray(QObject *parent = nullptr);
    QMLArray(const QMLArray& array);
    QMLArray(QArray &array);
    QMLArray(const QArray &array);
    QMLArray& operator=( const QMLArray& array );
    QMLArray& operator=( const QMLArray* array );

    ///
    /// \brief newArray
    /// \return
    /// Return new Instance of QMLArray
    Q_INVOKABLE static QMLArray* newArray();






    ///
    /// \brief insertElement
    /// \param element
    /// \return
    /// Add QMLElement to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertElement( QMLElement *element );

    ///
    /// \brief insertInt
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertInt( int value );

    ///
    /// \brief insertDouble
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertDouble( double value );

    ///
    /// \brief insertBool
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertBool( bool value );

    ///
    /// \brief insertString
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertString( QString value );

    ///
    /// \brief insertOid
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertOid( QString oid );

    ///
    /// \brief insertBson
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertBson(QMLBSON *value );

    ///
    /// \brief insertArray
    /// \param value
    /// \return
    /// Add value to Array and Return self. This is Fluent builder Pattern.
    Q_INVOKABLE self& insertArray( QMLArray* value );


    ///
    /// \brief getElement
    /// \param index
    /// \return
    /// Return QMLElement at index position
    Q_INVOKABLE QMLElement* getElement(const int& index);



    ///
    /// \brief getArray
    /// \return
    /// Return Base Class Object
    QArray getArray() const;

    ///
    /// \brief setArray
    /// \param array
    /// Set Base Object to this.
    void setArray( QArray &array );


};


Q_DECLARE_METATYPE(QMLArray)

#endif // QMLARRAY_H
