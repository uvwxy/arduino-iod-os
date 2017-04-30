#!/bin/bash

export PATH=$PATH:~/.platformio/penv/bin/

if [[ `git status --porcelain` ]]; then
  echo "Error: Repository not clean. Cleanup/Commit first."
  exit
else
  echo "OK: Building"
fi

BUILDINFO=src/BuildInfo.h

echo "#ifndef BuildInfo_h" > $BUILDINFO
echo "#define BuildInfo_h" >> $BUILDINFO
echo "#define BUILD_DATE \"$( date '+%Y-%m-%d')\"" >> $BUILDINFO
echo "#define BUILD_TIME \"$( date '+%H:%M:%S')\"" >> $BUILDINFO
echo "#define BUILD_HASH \"$(git log --pretty=format:'%h' -n 1)\"" >> $BUILDINFO
echo "#endif // ifndef BuildInfo_h" >> $BUILDINFO


if [ "$1" == "upload" ]; then
  platformio run --target upload --environment d1_mini
fi
