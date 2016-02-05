#ifndef GZIPFILTER_H
#define GZIPFILTER_H

#include <QIODevice>
#include <zlib.h>

class GZipReadFilter : public QIODevice
{
public:
    GZipReadFilter(QIODevice *in);
    GZipReadFilter(QIODevice *in, QObject *parent);

    virtual bool open(OpenMode mode);
    bool isSequential() const;
    bool atEnd() const;

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    QIODevice *device;
    z_stream strm;
    bool done = false;
    QByteArray buffer;
};

#endif // GZIPFILTER_H
