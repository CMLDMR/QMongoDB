#include "qerror.h"

QError::QError(const char *err)
    :error(err)
{

}

QError::QError(std::string err)
    :error(err.c_str())
{

}

QError::QError(QString err)
    :error(err)
{

}

const char *QError::what() const noexcept
{
    return error.toStdString().c_str();
}
