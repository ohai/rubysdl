require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::setVideoMode(640,480,16,SDL::SWSURFACE)

image = SDL::Surface.loadBMP("icon.bmp")
image.setColorKey( SDL::SRCCOLORKEY ,0)
image = image.displayFormat

event=SDL::Event.new
i=0;
while true
  while event = SDL::Event2.poll
    case event
    when SDL::Event2::Quit, SDL::Event2::KeyDown
      exit
    end
  end
  screen.fillRect(0,0,640,480,0)

  image.setAlpha(SDL::SRCALPHA,i%256)
  screen.put(image,310,195)
    i+=1
  screen.flip
end
