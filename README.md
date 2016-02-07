# OpenRCT2 Launcher
An Alternative Open Source Launcher &amp; Updater for OpenRCT2

This is a new launcher for [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2). It's written in C++ using the Qt Framework. The main reason for creating this version is to add support for Mac OS X and Linux, as the other launcher only works properly on Windows.

If you're using Windows, I recommend checking out [PFCKrutonium's Launcher](https://github.com/PFCKrutonium/OpenRCT2Launcher), as it is more feature-complete at this moment.

## Dependencies

To build this project, you need [Qt 5](http://www.qt.io/download-open-source/). Note that Qt 4 will not work. You also need Zlib to compile and run this, but since Qt 5 is already dependent on it, the compiler will attempt to use the version that comes with Qt and fallback to a system-installed version if need be. Lastly, this project needs OpenSSL installed to communicate properly with the server.

For building on Windows, you should just grab the [Official Qt 5 Installer](http://www.qt.io/download-open-source/). It will include Zlib, but not OpenSSL. There are no official pre-build binaries of OpenSSL, but [there are some unofficial ones you can use](https://wiki.openssl.org/index.php/Binaries).

For OS X, it is recommended that you install Qt5 with brew using `brew install qt5`. The other dependencies are included with OS X by default. It's recommended that you download and use [Qt Creator](http://www.qt.io/download-open-source/#section-6) as your IDE.

For Linux, search your distro's package manager for Qt5 (and OpenSSL if it's not already installed). If you're using Ubuntu, follow [these instructions](https://wiki.qt.io/Install_Qt_5_on_Ubuntu) for getting Qt5 installed.
