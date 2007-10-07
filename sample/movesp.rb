require 'sdl'

SDL.init(SDL::INIT_VIDEO)

screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)

image = SDL::Surface.load_bmp("icon.bmp")
image.set_color_key( SDL::SRCCOLORKEY || SDL::RLEACCEL ,0)
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
    SDL::Surface.blit($image,0,0,32,32,screen,@x,@y)
  end
  
end

sprites = []
for i in 1..5
  sprites << Sprite.new
end

class MovableSp
  def initialize()
    @ud=@lr=0;
  end
  
  def move()
    @ud=@lr=0;
    @lr=-1 if SDL::Key.press?(SDL::Key::H) or SDL::Key.press?(SDL::Key::LEFT)
    @lr=1  if SDL::Key.press?(SDL::Key::L) or SDL::Key.press?(SDL::Key::RIGHT)
    @ud=1  if SDL::Key.press?(SDL::Key::J) or SDL::Key.press?(SDL::Key::DOWN)
    @ud=-1 if SDL::Key.press?(SDL::Key::K) or SDL::Key.press?(SDL::Key::UP)
  end
  
  def draw(screen)
    SDL::Surface.blit($image,0,0,32,32,screen,300+@lr*50,200+@ud*50)
  end
end

sprites << MovableSp.new
black = screen.format.map_rgb(0, 0, 0)

while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::Quit
      exit
    when SDL::Event::KeyDown
      exit if event.sym == SDL::Key::ESCAPE
    end
  end
  
  screen.fill_rect(0,0,640,480,black)
  SDL::Key.scan
  
  sprites.each {|i|
    i.move
    i.draw(screen)
  }
  screen.update_rect(0,0,0,0)
end


