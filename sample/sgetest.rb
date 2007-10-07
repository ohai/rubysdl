require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)
SDL::WM.set_caption $0, $0

format = screen.format
# draw red pixel at (200,200)
screen[200,200]= format.map_rgb(255,0,0)
# draw green pixel at (250,200)
screen[250,200]= format.map_rgb(0,255,0)
# draw blue pixel at (200,200)
screen[300,200]= format.map_rgb(0,0,255)

Red=format.map_rgb(255,0,0)
screen.draw_line(20,20,300,200,Red)
screen.draw_rect(49,59,80,80,Red)
screen.draw_circle(100,100,50,[87,87,87])
screen.draw_circle(300,300,30,Red, true)
screen.draw_circle(230, 300, 30, Red, true, true)

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
