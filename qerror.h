#ifndef QERROR_H
#define QERROR_H

#include <QtCore/qglobal.h>

class QError : public std::exception
{
public:
    QError();
};

#endif // QERROR_H