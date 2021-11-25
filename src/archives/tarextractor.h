#ifndef TAREXTRACTOR_H
#define TAREXTRACTOR_H

#include <QDir>
#include <QIODevice>

bool extractTar(QIODevice *in, QDir extractLoc);

#endif // TAREXTRACTOR_H
