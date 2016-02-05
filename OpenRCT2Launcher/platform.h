#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGlobal>

#define LAUNCHER_VERSION_STRING "v0.0.2"

#ifndef Q_PROCESSOR_X86
#error Needs x86 to run OpenRCT2
#endif

#if defined(Q_OS_WIN)

#define OPENRCT2_FLAVOR "1"
// Qt uses forward slashes for all OS's and converts to backslashes internally
#define OPENRCT2_BASE "Documents/OpenRCT2/"
#define OPENRCT2_EXEC_NAME "openrct2.exe"

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

#define OPENRCT2_BIN OPENRCT2_BASE "bin/"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN OPENRCT2_EXEC_NAME

#endif // PLATFORM_H
