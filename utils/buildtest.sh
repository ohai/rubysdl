#!/bin/sh

ruby extconf.rb
make clean; make
ruby -I lib -I. sample/sgetest.rb

ruby extconf.rb --enable-imported-sge
make clean; make
ruby -I lib -I. sample/sgetest.rb

ruby1.9.1 extconf.rb
make clean; make
ruby1.9.1 -I lib -I. sample/sgetest.rb

ruby1.9.1 extconf.rb --enable-imported-sge
make clean; make
ruby1.9.1 -I lib -I. sample/sgetest.rb

ruby1.8 extconf.rb
make clean; make
ruby1.8 -I lib -I. sample/sgetest.rb
