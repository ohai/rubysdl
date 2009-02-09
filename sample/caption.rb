# -*- coding: euc-jp -*-
require 'sdl'
SDL.init(SDL::INIT_VIDEO)
screen = SDL::Screen.open(640, 480, 16, SDL::SWSURFACE)
SDL::WM::set_caption('キャプションのテスト','キャプション')

s,* = SDL::WM.caption
STDOUT.puts s.encode('external')
p s.encoding
loop do
  while event = SDL::Event.poll
    case event
    when SDL::Event::KeyDown, SDL::Event::Quit
      exit
    end
  end
  screen.fill_rect(0, 0, 640, 480, 0)
  screen.update_rect(0, 0, 0, 0)

  SDL.delay(10)
end
