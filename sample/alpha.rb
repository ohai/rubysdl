require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)

image = SDL::Surface.load_bmp("icon.bmp")
image.set_color_key( SDL::SRCCOLORKEY ,0)
image = image.display_format


i=0;
black = screen.format.map_rgb(0,0,0)
while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::Quit, SDL::Event::KeyDown
      exit
    end
  end
  screen.fillRect(0,0,640,480,black)

  image.set_alpha(SDL::SRCALPHA,i%256)
  screen.put(image,310,195)
    i+=1
  screen.flip
end
