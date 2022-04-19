#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>

class FileServer : public QObject
{
    Q_OBJECT
public:
    FileServer(QString port,QObject* parent = nullptr);
    ~FileServer();
};

#endif // FILESERVER_H
