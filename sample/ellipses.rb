# SDL random ellipses
# Author: Wayne Conrad <wconrad@yagni.com>

require 'sdl'

def show(function)
  event = SDL::Event.new
  screen = SDL::setVideoMode(640, 480, 16, SDL::SWSURFACE|SDL::ANYFORMAT)
  loop do
    color = screen.mapRGB(rand(256),rand(256),rand(256))
    x = rand(screen.w)
    y = rand(screen.h)
    xr = rand(80)
    yr = rand(80)
    eval("screen.#{function}(x, y, xr, yr, color)")
    while event = SDL::Event2.poll
      case event
      when SDL::Event2::Quit
        exit
      when SDL::Event2::KeyDown
        if event.sym == SDL::Key::ESCAPE
          exit
        else
          return
        end
      when SDL::Event2::MouseButtonDown
        return
      end
    end
    SDL::Key.scan
    screen.updateRect(0, 0, 0, 0)
  end
end

srand
SDL.init SDL::INIT_VIDEO
show("drawEllipse")
show("drawFilledEllipse")
