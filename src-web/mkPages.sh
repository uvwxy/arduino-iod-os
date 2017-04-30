#!/bin/bash

BUILD_IN=./
BUILD_OUT=../src/

IFS=$'\n'       # make newlines the only separator
for file in $(ls *.html)
do
  FOUT=$BUILD_OUT$file.h
  FN_NAME=$(echo $file | tr "." "_")
  echo "#ifndef $FN_NAME""_h" > $FOUT
  echo "#define $FN_NAME""_h" >> $FOUT
  echo "#include <Arduino.h>" >> $FOUT
  echo "String create_$FN_NAME(){" >> $FOUT
  echo "  String ret = \"\";" >> $FOUT

  for line in $(cat $file)
  do
    if [[ $line == CPP_* ]];
      then
      echo ${line#CPP_} >> $FOUT
      else
      echo "  ret +=\"$line\";" >> $FOUT
    fi
  done

  echo "  return ret;" >> $FOUT
  echo "}" >> $FOUT
  echo "#endif" >> $FOUT
done
