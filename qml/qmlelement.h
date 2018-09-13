#ifndef QMLELEMENT_H
#define QMLELEMENT_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class QMLElement : public QObject
{
    Q_OBJECT
public:
    explicit QMLElement(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QMLELEMENT_H