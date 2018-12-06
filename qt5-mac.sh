
QT5_MAJOR=11
QT5_MINOR=2
QT5_PATCH=0-201809141947

# Qt likes to change how their filenames work
QT5_SHORT=5${QT5_MAJOR}${QT5_MINOR}
QT5_DIR=5.${QT5_MAJOR}.${QT5_MINOR}
QT5_OS_VER=MacOS-MacOS_10_12
QT5_DIR_PREFIX=qt.qt5

if (( $# < 2 )); then
  echo "Usage: <destination> <component> [component...]"
  (( "$#" == 0 ))
  exit $?;
fi

for x in ${@:2}; do
  if [[ $x = extra-* ]]; then
    DOWNLOAD_URL="https://download.qt.io/online/qtsdkrepository/mac_x64/desktop/qt5_${QT5_SHORT}/${QT5_DIR_PREFIX}.${QT5_SHORT}.qt${x/#extra-/}.clang_64/${QT5_DIR}-${QT5_PATCH}qt${x/#extra-/}-${QT5_OS_VER}-Clang-${QT5_OS_VER}-X86_64.7z"
  else
    DOWNLOAD_URL="https://download.qt.io/online/qtsdkrepository/mac_x64/desktop/qt5_${QT5_SHORT}/${QT5_DIR_PREFIX}.${QT5_SHORT}.clang_64/${QT5_DIR}-${QT5_PATCH}qt${x}-${QT5_OS_VER}-Clang-${QT5_OS_VER}-X86_64.7z"
  fi
  
  DOWNLOAD_CHECK=$(curl -f ${DOWNLOAD_URL}.sha1 2>/dev/null)
  if [[ -z $DOWNLOAD_CHECK ]]; then
    echo "ERROR: Unknown package $x"
    exit 1
  fi
  
  curl -L -o /tmp/qt5.7z $DOWNLOAD_URL
  DOWNLOAD_HASH=$(shasum -a 1 /tmp/qt5.7z)
  if [[ $DOWNLOAD_HASH != $DOWNLOAD_HASH ]]; then
    echo "ERROR: Hash missmatch for $x" 1>&2
    exit 1
  fi
  
  7z x -aoa "-o$1" /tmp/qt5.7z 1>&2
  rm /tmp/qt5.7z
done

# Minimal Qt Configuration File
echo "[Paths]" > $1/${QT5_DIR}/clang_64/bin/qt.conf
echo "Prefix=.." >> $1/${QT5_DIR}/clang_64/bin/qt.conf

# Why does Qt default to Enterprise Licence?
sed -i "" -E 's/^[[:space:]]*QT_EDITION[[:space:]]*=.*$/QT_EDITION = OpenSource/' $1/${QT5_DIR}/clang_64/mkspecs/qconfig.pri
sed -i "" -E 's/^[[:space:]]*QT_LICHECK[[:space:]]*=.*$//' $1/${QT5_DIR}/clang_64/mkspecs/qconfig.pri
sed -i "" -E 's/^[[:space:]]*QT_RELEASE_DATE[[:space:]]*=.*$//' $1/${QT5_DIR}/clang_64/mkspecs/qconfig.pri

echo $1/${QT5_DIR}/clang_64/bin
