require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::setVideoMode(640,480,16,SDL::SWSURFACE)
SDL::WM::setCaption $0,$0

loop do
  if event=SDL::Event.poll then
    case event
    when SDL::Event::Active
      print "Active ", (event.gain)?"gain":"lost","\n"
    when SDL::Event::KeyDown
      print "KeyDown sym:#{event.sym} mod:#{event.mod}","\n"
      if event.sym==SDL::Key::A then
	e=SDL::Event::KeyDown.new
	e.sym=3333
	e.mod=3333
	e.press=true
        e.unicode=200
	SDL::Event.push(e)
      end
    when SDL::Event::KeyUp
      print "KeyUp sym:#{event.sym} mod:#{event.mod}","\n"
      
    when SDL::Event::Quit
      exit
      
    end
  end
end
  
	
