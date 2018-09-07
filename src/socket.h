#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SOCKET_H