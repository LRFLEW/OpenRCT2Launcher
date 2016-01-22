#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGlobal>

#ifndef Q_PROCESSOR_X86
#error Needs x86 to run OpenRCT2
#endif

#if defined(Q_OS_WIN)

#define OPENRCT2_FLAVOR "1"
#define OPENRCT2_BIN "Documents\\OpenRCT2\\bin\\"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN "openrct2.exe"

#elif defined(Q_OS_OSX)

#define OPENRCT2_FLAVOR "3"
#define OPENRCT2_BIN "Library/Application Support/OpenRCT2/bin/"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN "OpenRCT2.app/Contents/MacOS/OpenRCT2"

#elif defined(Q_OS_LINUX)

#define OPENRCT2_FLAVOR "4"
#define OPENRCT2_BIN ".config/OpenRCT2/bin/"
#define OPENRCT2_EXEC_LOCATION OPENRCT2_BIN "openrct2"

#else

#error Unsupported Platform

#endif

#endif // PLATFORM_H
