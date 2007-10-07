require 'sdl'

SDL.init( SDL::INIT_VIDEO )

screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)
SDL::WM::set_caption($0,$0)

SDL::TTF.init

font = SDL::TTF.open('sample.ttf',24)
font.style = SDL::TTF::STYLE_NORMAL

font.draw_solid_utf8(screen,'test SDL_ttf',300,150,255,255,255)

screen.flip

loop do
  while event = SDL::Event2.poll
    case event
    when SDL::Event2::KeyDown, SDL::Event2::Quit
      exit
    end
  end
  
end
