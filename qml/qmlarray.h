#ifndef QMLARRAY_H
#define QMLARRAY_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class QMLArray : public QObject
{
    Q_OBJECT
public:
    explicit QMLArray(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QMLARRAY_H