#include "zipextractor.h"

#include <QByteArray>
#include <QDataStream>
#include <zlib.h>

#define READ_BUFFER_SIZE 16384
#define NAME_BUFFER_SIZE 64

static const quint32 ZipLocalMagic = 0x04034B50;
static const quint32 ZipCentralMagic = 0x02014B50;
static const quint32 ZipEndMagic = 0x06054B50;
#define qcrc32(crc, buf, len) ((quint32) crc32(crc, (const Bytef *) buf, len))

static inline QFile::Permissions getPermissionsFromUnix(quint16 type) {
    return static_cast<QFile::Permissions>(((type & 0700) << 6) | ((type & 0700) << 2) | ((type & 0070) << 1) | (type & 0007));
}

bool extractZip(QIODevice *in, QDir extractLoc) {
    in->open(QIODevice::ReadOnly);
    QDataStream stream(in);
    stream.setByteOrder(QDataStream::LittleEndian); // Just to be sure

    QByteArray buffer, cbuffer, namebuf;
    buffer.resize(READ_BUFFER_SIZE);
    cbuffer.resize(READ_BUFFER_SIZE);
    namebuf.reserve(NAME_BUFFER_SIZE);

    while (true) {
        quint32 magic;
        stream >> magic;

        if (magic == ZipLocalMagic) {

            quint8 zver, ztype;
            stream >> zver >> ztype;
            if (ztype != 0 || zver > 20) return false; // Unsupported Zip Version

            quint16 flags;
            stream >> flags;
            if (flags & 0xF7F9) return false; // Unsupported Flags

            quint16 comp;
            stream >> comp;
            if (comp != 0 && comp != 8) return false; // Unsupported Compression

            stream.skipRawData(4); // Ignore Modification Date and Time

            quint32 crc;
            stream >> crc;

            quint32 csize, esize;
            stream >> csize >> esize;

            quint16 namelen, extralen;
            stream >> namelen >> extralen;

            namebuf.resize(namelen);
            stream.readRawData(namebuf.data(), namelen);
            QString name = QString::fromUtf8(namebuf, namelen);

            stream.skipRawData(extralen); // Ignore Extra Field

            if (csize == 0 && esize == 0 && name.endsWith("/")) { // Is a Directory (Even on Windows)
                extractLoc.mkdir(name);

            } else if (comp == 0) { // No Compression (I Don't expect to see it)
                quint32 length = esize;
                quint32 rcrc = qcrc32(0, Z_NULL, 0);
                QFile file(extractLoc.filePath(name));
                file.open(QFile::WriteOnly);

                while (length > 0) {
                    int read = stream.readRawData(buffer.data(), qMin(length, (quint32) READ_BUFFER_SIZE));
                    file.write(buffer.data(), read);
                    rcrc = qcrc32(rcrc, buffer.data(), read);
                    length -= read;
                }

                file.close();
                if (rcrc != crc) return false; // CRC-32 Mismatch

            } else { // Standard DEFLATE

                z_stream strm;
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
                strm.opaque = Z_NULL;
                strm.next_in = Z_NULL;
                strm.avail_in = 0;
                int ret = inflateInit2(&strm, -MAX_WBITS);
                if (ret != Z_OK)
                    return ret;

                quint32 length = csize;
                quint32 written = 0;
                quint32 rcrc = qcrc32(0, Z_NULL, 0);
                QFile file(extractLoc.filePath(name));
                file.open(QFile::WriteOnly);

                while (length > 0 && ret == Z_OK) {
                    int read = stream.readRawData(cbuffer.data(), qMin(length, (quint32) READ_BUFFER_SIZE));
                    strm.next_in = (Bytef *) cbuffer.data();
                    strm.avail_in = read;

                    do {
                        strm.next_out = (Bytef *) buffer.data();
                        strm.avail_out = READ_BUFFER_SIZE;
                        ret = inflate(&strm, Z_NO_FLUSH);
                        if (ret >= 0) {
                            quint32 towrite = READ_BUFFER_SIZE - strm.avail_out;
                            file.write(buffer.data(), towrite);
                            rcrc = qcrc32(rcrc, buffer.data(), towrite);
                            written += towrite;
                        }
                    } while (ret == Z_OK && strm.avail_in > 0);
                    length -= read;
                }

                file.close();
                ret ^= 1;
                ret |= inflateEnd(&strm);
                if (ret != Z_OK) return false;
                if (written != esize) return false; // Unexpected File Size
                if (length != 0) return false; // Unexpected End Of Stream
                if (rcrc != crc) return false; // CRC-32 Mismatch
            }

        } else if (magic == ZipCentralMagic) {
            stream.skipRawData(1); // Skip Writer Version

            quint8 ztype;
            stream >> ztype;

            stream.skipRawData(22);

            quint16 namelen, extralen, commentlen;
            stream >> namelen >> extralen >> commentlen;

            if (ztype == 3) {
                stream.skipRawData(6);

                quint16 type;
                stream >> type;

                stream.skipRawData(4);

                namebuf.resize(namelen);
                stream.readRawData(namebuf.data(), namelen);
                QString name = QString::fromUtf8(namebuf, namelen);

                if ((type >> 12) == 8 || (type >> 12) == 4) {
                    if (name.endsWith("/") != ((type >> 12) == 4)) return false; // Type != Written
                    QFile file(extractLoc.filePath(name));
                    file.setPermissions(file.permissions() | getPermissionsFromUnix(type));
                }

                stream.skipRawData(extralen + commentlen);
            } else {
                stream.skipRawData(12 + namelen + extralen + commentlen);
            }
        } else if (magic == ZipEndMagic) {
            // End of File
            return true;
        } else {
            return false; // Unknown Magic
        }
    }
}
