#ifndef QMLELEMENT_H
#define QMLELEMENT_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qelement.h"


class QMLBSON;
class QMLArray;

class QMLElement : public QObject , public QElement
{
    Q_OBJECT
    Q_PROPERTY( QString Key READ Key NOTIFY keyChanged )
    Q_PROPERTY( double Double READ getDouble )
    Q_PROPERTY( QString String READ getString NOTIFY stringChanged )
    Q_PROPERTY( int Int READ getInt )
    Q_PROPERTY( QVariant Int64 READ getInt64 NOTIFY int64Changed )
    Q_PROPERTY( bool Bool READ getBool )
    Q_PROPERTY( QString TypeName READ TypeName )
    Q_PROPERTY( Type Type READ getElementType )
    Q_PROPERTY( QString Oid READ Oid NOTIFY oidChanged )
    Q_PROPERTY( QMLBSON* Bson READ getBson NOTIFY bsonChanged )
    Q_PROPERTY( QMLArray* Array READ getArray )
public:
    explicit QMLElement( QObject *parent = nullptr );
    QMLElement( const QMLElement& element );
    QMLElement( const QElement& element );
    virtual ~QMLElement(){ qDebug() << "QMLElement Desturctor"; }

    QMLElement& operator=(const QMLElement& other);

    enum Type{
        B_double = 0,
        B_utf8,
        B_oid,
        B_int64,
        B_int32,
        B_document,
        B_array,
        B_bool,
        B_null,
        B_invalid,
        B_binary
    };
    Q_ENUM(Type)


    Q_INVOKABLE static QMLElement *newElement();
    Q_INVOKABLE static QMLElement *newElement( const QMLElement::Type& type , const QString& key , const QString& value );

    Q_INVOKABLE void setStringData( const QString& key , const QString& value );
    Q_INVOKABLE void setOidData( const QString& key , const QString& oid );
    Q_INVOKABLE void setInt32Data( const QString& key , const int& value );
    Q_INVOKABLE void setInt64Data( const QString& key , const qint64& value );
    Q_INVOKABLE void setDoubleData( const QString& key , const double& value );
    Q_INVOKABLE void setBoolData( const QString& key , const bool& value );

    Q_INVOKABLE void setData( const QString& key , const QString& value  , const Type& type = B_utf8 );
    Q_INVOKABLE void setData( const QString& key , const double& value);
    Q_INVOKABLE void setData( const QString& key , const int& value , const Type& type = B_int32 );
    Q_INVOKABLE void setData( const QString& key , const bool& value );
    Q_INVOKABLE void setData( const QVariant &element );



    Q_INVOKABLE QElement getQElement() const;

    Type getElementType() const;

private:
    QString Key() const;

    double getDouble() const;

    QString getString() const;

    int getInt() const;

    QVariant getInt64() const;

    bool getBool() const;

    QString Oid() const;

    QMLBSON *getBson() const;

    QMLArray* getArray() const;

    QString TypeName() const;

    static QElementType ConvertType(QMLElement::Type type);


signals:
    void keyChanged();
    void stringChanged();
    void oidChanged();
    void bsonChanged();
    void int64Changed();

public slots:


private:
    void setElement(const QElement& element);
    QMLElement(const QElementType &type , const QString& key , const QString& value);

};

Q_DECLARE_METATYPE(QMLElement);


#endif // QMLELEMENT_H
