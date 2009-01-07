
RSD = ruby ../doc/rsd.rb -e
RDFILES = general.rd init.rd video.rd opengl.rd wm.rd event.rd joystick.rd cdrom.rd mixer.rd time.rd font.rd collision.rd sdlskk.rd mpeg.rd
RD2 = rd2
RD2OPT = -r rd/rd2html-lib.rb --html-lang=en --html-charset=UTF-8


rubysdl_ref.en.html: rubysdl_ref.en.rd
	$(RD2) $(RD2OPT) --html-title="Ruby/SDL Reference Manual" $< > $@ || rm -f $@

rubysdl_ref.en.rd: $(RDFILES)
	cat $(RDFILES) > rubysdl_ref.en.rd

%.rd: %.rsd
	$(RSD) < $< > $@

install: rubysdl_ref.en.html
	cp rubysdl_ref.en.html rubysdl_ref.en.rd ..
