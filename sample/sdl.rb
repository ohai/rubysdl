require 'rubysdl'

printf "%d \n" ,  SDL::INIT_TIMER | SDL::INIT_VIDEO 

SDL.init(SDL::INIT_TIMER | SDL::INIT_VIDEO )
screen = SDL::Screen.setVideoMode(640,480,16,SDL::SWSURFACE)
exit if screen == nil

sprite = SDL::Surface.loadBMP("icon.bmp")
sprite.setColorKey( 0x00001000 | 0x00002000 ,0)
sprite = sprite.displayFormat


screen.fillRect(100,100,100,100,678432)
SDL.blitSurface(sprite,0,0,32,32,screen,0,0)
screen.updateRect(0,0,640,480)
event = SDL::Event.new

while true 
  if  event.pollEvent != 0 then
    if event.type==SDL::Event::QUIT then
      break
    end
    if event.type==SDL::Event::KEYDOWN then
      break
    end
  end
end




	
