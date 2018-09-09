#ifndef QERROR_H
#define QERROR_H


#if defined(QMONGODB_LIBRARY)
#  define QMONGODBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QMONGODBSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/qglobal.h>
#include <stdexcept>
#include <iostream>
#include <QString>
#include <QDebug>




class QMONGODBSHARED_EXPORT QError : public std::exception
{
public:
    QError(const char* err);
    QError(std::string err);
    QError(QString err);

    virtual const char *what() const noexcept;


private:
    QString error;
};

#endif // QERROR_H
