require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)
SDL::WM::set_caption $0, $0

Red=screen.format.map_rgb(255,0,0)
screen.draw_line(20,20,300,200,Red, true)
screen.draw_circle(100,100,50,[87,87,87], false, true)
screen.draw_circle(300,300,30,Red, true, true)
screen.draw_ellipse(320,240,100,200,[200,255,0], false, true)

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
