require 'sdl'

SDL.init(SDL::INIT_VIDEO)
screen = SDL::Screen.open(640, 480, 16, SDL::SWSURFACE)
SDL::WM::set_caption('testsprite.rb','testsprite.rb icon')
image = SDL::Surface.load_bmp("icon.bmp")
image.set_color_key(SDL::SRCCOLORKEY ,0)
$image = image.display_format

class Sprite
  def initialize
    @x=rand(640)
    @y=rand(480)
    @dx=rand(11)-5
    @dy=rand(11)-5
  end
  
  def move
    @x += @dx
    if @x >= 640 then
      @dx *= -1
      @x = 639
    end
    if @x < 0 then
      @dx *= -1
      @x = 0
    end
    @y += @dy
    if @y >= 480 then
      @dy *= -1
      @y = 479
    end
    @y += @dy
    if @y < 0 then
      @dy *= -1
      @y = 0
    end
  end
  
  def draw(screen)
    SDL::Surface.blit($image, 0, 0, 32, 32, screen, @x, @y)
  end
  
end

sprites = []
for i in 1..100
  sprites << Sprite.new
end


while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::KeyDown, SDL::Event::Quit
      exit
    end
  end
  screen.fill_rect(0, 0, 640, 480, 0)
  
  sprites.each {|i|
    i.move
    i.draw(screen)
  }
  
  screen.update_rect(0, 0, 0, 0)
end


