#!/bin/sh

VERSION=1.0.0
PACKAGE=rubysdl-$VERSION.tar.gz
DIRNAME=rubysdl-$VERSION

mkdir $DIRNAME
mkdir ${DIRNAME}/lib
mkdir ${DIRNAME}/sample

for file in `cat MANIFEST`
do
  cp $file ${DIRNAME}/$file
done

tar czf $PACKAGE $DIRNAME/*
rm -r $DIRNAME
