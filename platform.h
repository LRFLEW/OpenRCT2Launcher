#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGlobal>
#include <QDir>

#ifndef Q_PROCESSOR_X86
#error Needs x86 to run OpenRCT2
#endif

#if defined(Q_OS_WIN)

#ifdef Q_PROCESSOR_X86_64
#define OPENRCT2_FLAVOR "6"
#define OPENRCT2_FLAVOR_FALLBACK "1"
#else
#define OPENRCT2_FLAVOR "1"
#endif

// Qt uses forward slashes for all OS's and converts to backslashes internally
#define OPENRCT2_BASE "OpenRCT2/"
#define OPENRCT2_EXEC_NAME "openrct2.exe"

#elif defined(Q_OS_OSX)

// Only one flavor for macOS
#define OPENRCT2_FLAVOR "3"

#define OPENRCT2_BASE "Library/Application Support/OpenRCT2/"
#define OPENRCT2_EXEC_NAME "OpenRCT2.app/Contents/MacOS/OpenRCT2"

#elif defined(Q_OS_LINUX)

#ifdef Q_PROCESSOR_X86_64
#define OPENRCT2_FLAVOR "9"
#define OPENRCT2_FLAVOR_FALLBACK "4"
#else
#define OPENRCT2_FLAVOR "4"
#endif

#define OPENRCT2_BASE ".config/OpenRCT2/"
#define OPENRCT2_EXEC_NAME "openrct2"

#else

#error Unsupported Platform

#endif

#if defined(Q_OS_OSX) // macOS doesn't differenciate bit-ness
#define OPENRCT2_BIT_SUFFIX
#elif defined(Q_PROCESSOR_X86_64)
#define OPENRCT2_BIT_SUFFIX " x64"
#else
#define OPENRCT2_BIT_SUFFIX " x86"
#endif

#define OPENRCT2_BIN OPENRCT2_BASE "bin/"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN OPENRCT2_EXEC_NAME

#ifdef Q_OS_WIN
#include <QStandardPaths>
#define OPENRCT2_HOMEPATH (QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
#define OPENRCT2_HOMEDIR (QDir(OPENRCT2_HOMEPATH))
#else
#define OPENRCT2_HOMEPATH (QDir::homePath())
#define OPENRCT2_HOMEDIR (QDir::home())
#endif

#endif // PLATFORM_H
