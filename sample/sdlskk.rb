#
# SDLSKK from Ruby/SDL
#
# This sample needs three files.
#   jisyo       : SKK's dictionary
#   rule_table  : SDLSKK's
#   nihongo.ttf : True Type Font file that has Japanese characters
#
# Usage:
#   ruby sdlskk.rb [-m]
#   
#     -m  Use minibuffer
#
require 'sdl'

use_minibuffer = (ARGV[0]=='-m')
  
SDL.init( SDL::INIT_VIDEO )
SDL::TTF.init
SDL::Event.enableUNICODE

font = SDL::TTF.open( 'nihongo.ttf', 14 )

dict = SDL::SKK::Dictionary.new
dict.load( 'jisyo', false )
table = SDL::SKK::RomKanaRuleTable.new( 'rule_table' )
bind = SDL::SKK::Keybind.new
bind.set_default_key

context = SDL::SKK::Context.new( dict, table, bind, use_minibuffer )

screen = SDL::Screen.open( 640, 480, 16, SDL::SWSURFACE )
SDL::WM.set_caption( $0, $0 )

BLACK = screen.format.map_rgb( 0, 0, 0 )
loop do

  while event = SDL::Event.poll do
    case event
    when SDL::Event::Quit
      exit
    when SDL::Event::KeyDown
      if event.sym == SDL::Key::ESCAPE then
	exit
      end
      if event.sym == SDL::Key::F1
	dict.save("test_user_dict")
      end
      
      context.input( event )      
    end
  end

  text_surface = context.render_str( font, 255, 0, 255 )

  screen.fill_rect( 0, 0, 640, 480, BLACK )
  SDL::Surface.blit( text_surface, 0, 0, 0, 0, screen, 0, 0 )

  if use_minibuffer then
    minibuffer_surface = context.render_minibuffer_str( font, 255, 0, 255 )
    if minibuffer_surface then
      SDL::Surface.blit( minibuffer_surface, 0, 0, 0, 0, screen, 0, 40 )
    end
  end
  
  screen.update_rect( 0, 0, 0, 0 )

  sleep 0.05
  
end
