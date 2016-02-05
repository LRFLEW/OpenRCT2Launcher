#include "gzipreadfilter.h"

#define FILTER_BUFFER_SIZE 16384

GZipReadFilter::GZipReadFilter(QIODevice *in) :
    device(in)
{
    buffer.resize(FILTER_BUFFER_SIZE);
}

GZipReadFilter::GZipReadFilter(QIODevice *in, QObject *parent) :
    QIODevice(parent), device(in)
{  }

bool GZipReadFilter::open(OpenMode mode) {
    if (mode & OpenModeFlag::WriteOnly)
        return false;
    if (!device->open(mode))
        return false;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    int ret = inflateInit2(&strm, 16 + MAX_WBITS);
    if (ret != Z_OK)
        return false;

    return QIODevice::open(mode);
}

bool GZipReadFilter::isSequential() const {
    return true;
}

bool GZipReadFilter::atEnd() const {
    if (done) return QIODevice::atEnd();
    return false;
}

qint64 GZipReadFilter::writeData(const char *data, qint64 len) {
    // Do Nothing, as it won't happen
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

qint64 GZipReadFilter::readData(char *data, qint64 maxlen) {
    int ret = Z_OK;
    strm.next_out = (Bytef *) data;
    strm.avail_out = maxlen;
    while (ret == Z_OK && strm.avail_out > 0) {
        if (strm.avail_in == 0) {
            strm.avail_in = device->read(buffer.data(), FILTER_BUFFER_SIZE);
            strm.next_in = (Bytef *) buffer.data();
        }
        ret = inflate(&strm, Z_NO_FLUSH);
    }
    if (ret == Z_STREAM_END) done = true;
    return maxlen - strm.avail_out;
}
