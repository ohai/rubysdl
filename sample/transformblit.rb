require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)

image = SDL::Surface.load_bmp("icon.bmp")
image.set_color_key( SDL::SRCCOLORKEY , image[0,0])
image = image.display_format

aimage = SDL::Surface.load_bmp("icon.bmp")
aimage.set_color_key( SDL::SRCCOLORKEY, aimage[0,0] )
aimage.set_alpha(SDL::SRCALPHA, 128 )
aimage = aimage.display_format

angle = 0

loop do

  while event = SDL::Event.poll
    case event
    when SDL::Event::Quit
      exit
    when SDL::Event::KeyDown
      exit if event.sym == SDL::Key::ESCAPE
    end
  end

  angle += 2
  
  screen.fill_rect(0,0,640,480,[100,100,100])

  SDL::Surface.transform_draw(image,screen,angle,1,1,image.w/2,image.h/2,100,100,0 )
  SDL::Surface.transform_blit(image,screen,angle,1,1,image.w/2,image.h/2,200,200,0 )

  SDL::Surface.transform_draw(image,screen,angle,1,1,0,0,300,300,0 )
  SDL::Surface.transform_blit(image,screen,angle,1,1,0,0,400,200,0 )

  SDL::Surface.transform_blit(aimage,screen,angle,1,1,0,0,100,400,0 )
  
  screen.update_rect(0,0,0,0)

end
