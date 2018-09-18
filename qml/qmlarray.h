#ifndef QMLARRAY_H
#define QMLARRAY_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qbson.h"
#include "qml/qmlelement.h"
#include "qmlbson.h"


class QMLArray : public QObject , public QArray
{
    Q_OBJECT
    Q_PROPERTY(int count READ count )
public:
    explicit QMLArray(QObject *parent = nullptr);
    QMLArray(const QMLArray& array);
    QMLArray(QArray &array);
    QMLArray(const QArray &array);
    QMLArray& operator=( const QMLArray& array );
    QMLArray& operator=( const QMLArray* array );

    Q_INVOKABLE static QMLArray* newArray();





    Q_INVOKABLE void insertElement( QMLElement *element );
    Q_INVOKABLE void insertInt( int value );
    Q_INVOKABLE void insertDouble( double value );
    Q_INVOKABLE void insertBool( bool value );
    Q_INVOKABLE void insertString( QString value );
    Q_INVOKABLE void insertOid( QString oid );
    Q_INVOKABLE void insertBson(QMLBSON *value );
    Q_INVOKABLE void insertArray( QMLArray* value );
    Q_INVOKABLE QMLElement* getElement(const int& index);



    QArray getArray() const;
    void setArray( QArray &array );


signals:

public slots:

private:




};


Q_DECLARE_METATYPE(QMLArray)

#endif // QMLARRAY_H
