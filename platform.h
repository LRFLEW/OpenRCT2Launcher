#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGlobal>
#include <QDir>

#ifndef Q_PROCESSOR_X86
#error Needs x86 to run OpenRCT2
#endif

#if defined(Q_OS_WIN)

#define OPENRCT2_FLAVOR "1"
// Qt uses forward slashes for all OS's and converts to backslashes internally
#define OPENRCT2_BASE "OpenRCT2/"
#define OPENRCT2_EXEC_NAME "openrct2.exe"

// Workaround for QStringLiteral concatenation issue
#define OPENRCT2_BIN "OpenRCT2/bin/"
#define OPENRCT2_EXEC_LOCATION "OpenRCT2/bin/openrct2.exe"
#define OPENRCT2_THEMES_LOCATION "OpenRCT2/themes"
#define OPENRCT2_TITLE_LOCATION "OpenRCT2/title sequences"

#elif defined(Q_OS_OSX)

#define OPENRCT2_FLAVOR "3"
#define OPENRCT2_BASE "Library/Application Support/OpenRCT2/"
#define OPENRCT2_EXEC_NAME "OpenRCT2.app/Contents/MacOS/OpenRCT2"

#elif defined(Q_OS_LINUX)

#define OPENRCT2_FLAVOR "4"
#define OPENRCT2_BASE ".config/OpenRCT2/"
#define OPENRCT2_EXEC_NAME "openrct2"

#else

#error Unsupported Platform

#endif

#ifndef Q_OS_WIN
#define OPENRCT2_BIN OPENRCT2_BASE "bin/"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN OPENRCT2_EXEC_NAME
#define OPENRCT2_THEMES_LOCATION OPENRCT2_BIN "themes"
#define OPENRCT2_TITLE_LOCATION "title sequences"
#endif

#ifdef Q_OS_WIN
#include <QStandardPaths>
#define OPENRCT2_HOMEPATH (QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
#define OPENRCT2_HOMEDIR (QDir(OPENRCT2_HOMEPATH))
#else
#define OPENRCT2_HOMEPATH (QDir::homePath())
#define OPENRCT2_HOMEDIR (QDir::home())
#endif

#endif // PLATFORM_H
