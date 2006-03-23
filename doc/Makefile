
RSD = ruby rsd.rb
RDFILES = general.rd init.rd video.rd event.rd joystick.rd cdrom.rd time.rd
RD2 = rd2
RD2OPT = -r rd/rd2html-lib.rb --html-lang=ja --html-charset=EUC-JP

rubysdl.html: rubysdl.rd
	$(RD2) $(RD2OPT) --html-title="Ruby/SDL Reference Manual" $< > $@ || rm -f $@

rubysdl.rd: $(RDFILES)
	cat $(RDFILES) > rubysdl.rd

%.rd: %.rsd
	$(RSD) < $< > $@
