#!/bin/sh

# current directory should be rubysdl/win32

if [ "$1" = "" ]; then
    echo "usage: $0 VERSION"
    exit 1
fi
VERSION=$1
RUBY_VERSION=1.8.4
OPENGLDIR=../../opengl-0.32g
DLLDIR=../../lib

DIRNAME=rubysdl-$VERSION-mswin32-${RUBY_VERSION}
PACKAGE=${DIRNAME}.zip

mkdir $DIRNAME
mkdir $DIRNAME/lib
mkdir $DIRNAME/ext
mkdir $DIRNAME/dll

cp README.en.win32 README.ja.win32 $DIRNAME
cp ../sdl.so $OPENGLDIR/opengl.so $DIRNAME/ext
cp ../lib/*.rb $DIRNAME/lib
cp $DLLDIR/*.dll $DIRNAME/dll
cp install_rubysdl.rb $DIRNAME

zip -r $PACKAGE $DIRNAME
rm -r $DIRNAME

