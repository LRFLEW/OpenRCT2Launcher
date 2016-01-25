# OpenRCT2 Launcher
An Alternative Open Source Launcher &amp; Updater for OpenRCT2

This is a new launcher for [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2). It's written in C++ using the Qt Framework. The main reason for creating this version is to add support for Mac OS X and Linux, as the other launcher only works properly on Windows.

Check out [PFCKrutonium's Launcher](https://github.com/PFCKrutonium/OpenRCT2Launcher), as it is more feature-complete at this moment.

## Dependencies

To build this project, you need two libraries:

 * [Qt 5](http://www.qt.io/download-open-source/). Note that Qt 4 will not work
 * [libarchive 2 or 3](http://www.libarchive.org). The upcoming version 4 won't work currently

For OS X, it is recommended that you install both dependencies with brew using `brew install qt5 libarchive`, but leave both of them keg-only. It's recommended that you download and use [Qt Creator](http://www.qt.io/download-open-source/#section-6) as your IDE.

For Linux, search your distro's package manager for libarchive and Qt5. If you're using Ubuntu, follow [these instructions](https://wiki.qt.io/Install_Qt_5_on_Ubuntu) for getting Qt5 installed.
