require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)

font = SDL::BMFont.open("font.bmp",SDL::BMFont::TRANSPARENT)

y = 0

while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::KeyDown, SDL::Event::Quit
      exit
    end
  end
  screen.fill_rect(0,0,640,480,0)

  y = (y + 1) % 480
  font.textout(screen,"BitMapFont Testing..",40,y)
  
  screen.updateRect(0,0,0,0)
  sleep 0.005
end
