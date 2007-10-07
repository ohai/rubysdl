# SDL random ellipses
# Author: Wayne Conrad <wconrad@yagni.com>

require 'sdl'

def show(fill)
  screen = SDL::Screen.open(640, 480, 16, SDL::SWSURFACE)
  format = screen.format
  loop do
    color = format.map_rgb(rand(256),rand(256),rand(256))
    x = rand(screen.w)
    y = rand(screen.h)
    xr = rand(80)
    yr = rand(80)
    screen.draw_ellipse(x, y, xr, yr, color, fill)
    while event = SDL::Event.poll
      case event
      when SDL::Event::Quit
        exit
      when SDL::Event::KeyDown
        if event.sym == SDL::Key::ESCAPE
          exit
        else
          return
        end
      when SDL::Event::MouseButtonDown
        return
      end
    end
    screen.updateRect(0, 0, 0, 0)
    sleep 0.05
  end
  screen.destroy
end

srand
SDL.init SDL::INIT_VIDEO
show(false)
show(true)
