# This sample needs a MPEG file `sample.mpg'.
require 'sdl'

SDL.init( SDL::INIT_VIDEO|SDL::INIT_AUDIO )
SDL::Mixer.open

screen = SDL::Screen.open( 320, 240, 16, SDL::SWSURFACE )

mpeg = SDL::MPEG.load( 'sample.mpg' )

info = mpeg.info

p(info)

mpeg.enable_audio true
mpeg.enable_video true

mpeg.set_display(screen)
mpeg.set_display_region( 0, 0, screen.w, screen.h )
mpeg.play

loop do

  case event = SDL::Event.poll
  when SDL::Event::Quit
    mpeg.stop
    exit
  when SDL::Event::KeyDown
    case event.sym
    when SDL::Key::S
      mpeg.stop
    when SDL::Key::P
      mpeg.play
    when SDL::Key::R
      mpeg.rewind
      mpeg.play
    when SDL::Key::ESCAPE
      exit      
    end
  end
      
  sleep 0.1
  
end
