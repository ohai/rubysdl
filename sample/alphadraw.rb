require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)
SDL::WM::set_caption $0, $0

Red=screen.format.map_rgb(255,0,0)
screen.draw_circle(320,240,220,[140,180,0], true, true)
screen.draw_line(20,20,300,200,Red,true, rand(64)+128)
screen.draw_circle(100,100,50,[87,87,87],false,true,rand(64)+128)
screen.draw_circle(300,170,50,[87,80,0],true, false, rand(64)+128)
screen.draw_ellipse(320,240,100,200,[200,255,0],false, false, rand(64)+128)

screen.flip

while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::KeyDown, SDL::Event::Quit
      exit
    end
  end

  sleep 0.2
end
