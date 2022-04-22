#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>

class FileServer : public QObject
{
    Q_OBJECT
public:
    FileServer(QString filename, QObject* parent = nullptr);
    ~FileServer();
    int port;
};

#endif // FILESERVER_H
