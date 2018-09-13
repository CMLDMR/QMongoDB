#ifndef QMLELEMENT_H
#define QMLELEMENT_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qelement.h"

class QMLElement : public QObject , public QElement
{
    Q_OBJECT
    Q_PROPERTY(QString Key READ Key )
    Q_PROPERTY(double Double READ getDouble )
    Q_PROPERTY(QString String READ getString )
    Q_PROPERTY(int Int READ getInt )
    Q_PROPERTY(bool Bool READ getBool )
    Q_PROPERTY(QString TypeName READ TypeName )
    Q_PROPERTY(QString Type READ getElementType )
    Q_PROPERTY(QString oid READ Oid )
public:
    explicit QMLElement(QObject *parent = nullptr);
    QMLElement(const QMLElement& element);
    QMLElement(const QElement& element);

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


    Q_INVOKABLE QVariant newElement();
    Q_INVOKABLE QVariant newElement(const QMLElement::Type& type ,const QString& key , const QString& value );

    Q_INVOKABLE void setData( const QString& key , const QString& value  , const Type& type = B_utf8 );
    Q_INVOKABLE void setData( const QString& key , const double& value);
    Q_INVOKABLE void setData( const QString& key , const int& value , const Type& type = B_int32 );
    Q_INVOKABLE void setData( const QString& key , const bool& value );
    Q_INVOKABLE void setData( const QVariant &element );

private:
    QString Key() const;

    double getDouble() const;

    QString getString() const;

    int getInt() const;

    bool getBool() const;

    QString Oid() const;

    QString TypeName() const;

    Type getElementType() const;

    static QElementType ConvertType(QMLElement::Type type);


signals:

public slots:


private:
    void setElement(const QElement& element);
    QMLElement(const QElementType &type , const QString& key , const QString& value);

};

Q_DECLARE_METATYPE(QMLElement);


#endif // QMLELEMENT_H
