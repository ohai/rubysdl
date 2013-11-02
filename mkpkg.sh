#!/bin/sh

if [ "$1" = "" ];then
    echo "Usage: $0 VERSION [TAG]"
    exit 1
fi
VERSION=$1

if [ "$2" = "" ];then
    TAG=$VERSION
else
    TAG=$2
fi

PACKAGE=rubysdl-$VERSION.tar.gz
DIRNAME=rubysdl-$VERSION

git archive --format=tar --prefix=$DIRNAME/ $TAG | tar xf -

export RBENV_VERSION=system 
LANG=ja_JP.eucJP make -C $DIRNAME/doc
make install -C $DIRNAME/doc 
make -C $DIRNAME/doc-en
make install -C $DIRNAME/doc-en


tar czf $PACKAGE $DIRNAME/*
rm -r $DIRNAME
