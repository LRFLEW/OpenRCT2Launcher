#include "tarextractor.h"

#define READ_BUFFER_SIZE 16384

bool extractTar(QIODevice *in, QDir extractLoc) {
    in->open(QIODevice::ReadOnly);
    QByteArray buffer, headbuf;
    buffer.resize(READ_BUFFER_SIZE);
    headbuf.resize(512);

    while (true) {
        char type;
        bool good;

        in->read(headbuf.data(), 512);

        if (headbuf[0] == '\0') return true; // Consider empty file name as end (close enough)

        if (qstrncmp(headbuf.data() + 257, "ustar", 5) != 0) return false; // Should be either POSIX or GNU tar

        // Terrible, terrible checksum
        // Will fail under odd edge-cases (fingers crossed it won't happen)
        quint16 checksum = headbuf.mid(148, 8).toUShort(&good, 8);
        if (!good) return false;
        headbuf.replace(148, 8, "        ", 8);
        quint16 sum = 0;
        for (quint8 c : headbuf) sum += c;
        if (sum != checksum) return false;

        QString name = QString::fromUtf8(headbuf.data(), qstrnlen(headbuf.data(), 100));
        if (name.startsWith("OpenRCT2/")) name.remove(0, 9);

        quint16 mode = headbuf.mid(100, 8).toUShort(&good, 16); // Use Base-16 for efficiency with QFile
        if (!good) return false;

        quint32 size = headbuf.mid(124, 12).toUInt(&good, 8);
        if (!good) return false;

        type = headbuf[156];

        if (type == '5') {
            if (size != 0) return false;
            if (name.isEmpty()) continue;
            if (!name.endsWith("/")) return false;
            extractLoc.mkdir(name);
        } else if (type == '0' || type == '\0') {
            if (name.endsWith("/")) return false;

            int length = size;
            QFile file(extractLoc.filePath(name));
            file.open(QFile::WriteOnly);

            while (length > 0) {
                int read = in->read(buffer.data(), qMin(length, (int) READ_BUFFER_SIZE));
                file.write(buffer.data(), read);
                length -= read;
            }

            file.close();

            file.setPermissions(file.permissions() | static_cast<QFile::Permissions>((mode & 0x0FFF) | ((mode & 0x0F00) << 4)));
        } else return false; // Unsupported type

        int padding = (0x200 - (size & 0x1FF)) & 0x1FF;
        if (padding > 0) {
            in->read(headbuf.data(), padding);
        }
    }
}
