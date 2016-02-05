#ifndef ZIPEXTRACTOR_H
#define ZIPEXTRACTOR_H

#include <QDir>
#include <QIODevice>

bool extractZip(QIODevice *in, QDir extractLoc);

#endif // ZIPEXTRACTOR_H
