#!/bin/sh

cd `dirname $0`/..
bundle exec jekyll build
cp _site/stylesheets/*.css stylesheets/
cp _site/stylesheets/bootstrap/*.css stylesheets/bootstrap/

