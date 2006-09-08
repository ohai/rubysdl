#!/bin/sh

if [ "$1" = "" ];then
    echo "Usage: $0 VERSION"
    exit 1
fi
VERSION=$1
PACKAGE=rubysdl-$VERSION.tar.gz
DIRNAME=rubysdl-$VERSION

mkdir $DIRNAME
mkdir ${DIRNAME}/lib
mkdir ${DIRNAME}/sample
mkdir ${DIRNAME}/doc

make -C doc
make install -C doc 

for file in `cat MANIFEST`
do
  cp $file ${DIRNAME}/$file
done

tar czf $PACKAGE $DIRNAME/*
rm -r $DIRNAME
