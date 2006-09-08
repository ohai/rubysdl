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
RUBYARCHIVE=ruby-1.8.4-i386-mswin32.zip
PACKAGE=${DIRNAME}.zip
PACK_PACKAGE=${DIRNAME}-pack.zip

mkdir $DIRNAME
mkdir $DIRNAME/lib
mkdir $DIRNAME/ext
mkdir $DIRNAME/dll
mkdir $DIRNAME/doc
mkdir $DIRNAME/sample
mkdir ruby
(cd ruby; unzip -q ../$RUBYARCHIVE)
PACKDOCDIR=ruby/doc/rubysdl-$VERSION
echo $PACKDOCDIR
mkdir $PACKDOCDIR
mkdir $PACKDOCDIR/sample
mkdir $PACKDOCDIR/doc

for fname in `cat ../MANIFEST`; do
    case $fname in
        sample/*)
            cp ../$fname $DIRNAME/sample/
            cp ../$fname $PACKDOCDIR/sample/
            ;;
        doc/*)
            cp ../$fname $DIRNAME/doc/
            cp ../$fname $PACKDOCDIR/doc/
            ;;
    esac
done
make -C ../doc/
cp ../doc/rubysdl_ref.rd ../doc/rubysdl_ref.html $DIRNAME
cp ../rubysdl_doc_old.rd ../rubysdl_doc.en.rd $DIRNAME
cp ../doc/rubysdl_ref.rd ../doc/rubysdl_ref.html $PACKDOCDIR
cp ../rubysdl_doc_old.rd ../rubysdl_doc.en.rd $PACKDOCDIR

cp README.en.win32 README.ja.win32 $DIRNAME
cp ../README.ja ../README.en $DIRNAME
cp ../NEWS.ja ../NEWS.en $DIRNAME
cp ../sdl.so $OPENGLDIR/opengl.so $DIRNAME/ext
cp ../lib/*.rb $DIRNAME/lib
cp $DLLDIR/*.dll $DIRNAME/dll

PACK_LIBDIR=ruby/lib/ruby/site_ruby/1.8
PACK_EXTDIR=$PACK_LIBDIR/i386-msvcrt
cp README.en.win32 README.ja.win32 $PACKDOCDIR
cp ../NEWS.ja ../NEWS.en $PACKDOCDIR
cp ../README.ja ../README.en $PACKDOCDIR
cp ../sdl.so $OPENGLDIR/opengl.so ruby/lib/ruby/site_ruby/1.8/i386-msvcrt
cp ../lib/*.rb ruby/lib/ruby/site_ruby/1.8/
cp $DLLDIR/*.dll ruby/bin

cp install_rubysdl.rb $DIRNAME

zip -q -r $PACKAGE $DIRNAME
zip -q -r $PACK_PACKAGE ruby
rm -r $DIRNAME
rm -r ruby
