#ifndef QMONGODB_H
#define QMONGODB_H


#include <QtCore/qglobal.h>

//#if defined(QMONGODB_LIBRARY)
//#  define QMONGODBSHARED_EXPORT Q_DECL_EXPORT
//#else
//#  define QMONGODBSHARED_EXPORT Q_DECL_IMPORT
//#endif


//#include "qmongodb_global.h"
#include <QString>
#include <QMap>
#include <QVariantMap>
#include "qbson.h"
#include <QObject>
#include <QtCore>



class QMONGODBSHARED_EXPORT QMongoDB : public QObject
{
    Q_OBJECT
public:

    QMongoDB(QString mongodburl , QString database , QObject* parent = nullptr);

    ~QMongoDB();

    QVector<QBSON> find(QString collection , QBSON filter , QOption option = QOption() );

    QBSON find_one( QString collection , QBSON filter , QOption option = QOption() );

    bool insert_one(QString collection , QBSON document );

    bool update_one(QString collection , QBSON filter , QBSON updateDocument );

    bool Delete(QString collection , QBSON filter);

    QElement uploadfile(QString filename, QString key);

    QString downloadfile(QElement fileoid);

    QByteArray downloadByteArray(QElement fileoid);

    ///
    /// \brief getfilesize if QElement type is not oid throw error
    /// \param fileoid
    /// \return
    ///
    qlonglong getfilesize(QElement fileoid);

    QString getfilename(QElement fileoid);

Q_SIGNALS:
    void gridfsbytereceived(int);

private:
    QString mUrl;
    QString db;
};

#endif // QMONGODB_H
