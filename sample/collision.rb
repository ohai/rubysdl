require 'sdl'

SDL.init( SDL::INIT_VIDEO )
screen = SDL::Screen.open(640,480,16,SDL::SWSURFACE)
SDL::WM::set_caption('collision.rb','collision.rb icon')
$image = SDL::Surface.load_bmp("icon.bmp")
$image.set_color_key( SDL::SRCCOLORKEY ,0)
$image = $image.display_format
$cMap = $image.make_collision_map

class Sprite

  attr_reader :x, :y

  def initialize(screen)
    @screen = screen
    @x=rand(screen.w)
    @y=rand(screen.h)
    @dx=rand(11)-5
    @dy=rand(11)-5
  end
  
  def move
    @x, @dx = move_coord(@x, @dx, xMax)
    @y, @dy = move_coord(@y, @dy, yMax)
  end

  def bounce
    @dx *= -1
    @dy *= -1
    move
  end

  def draw
    SDL::Surface.blit($image,0,0,$image.w,$image.h,@screen,@x,@y)
  end
  
  def collide_with?(sprite)
    $cMap.collision_check(@x, @y, $cMap, sprite.x, sprite.y) != nil
  end

  private

  def move_coord(coord, delta, max)
    coord += delta
    if coord >= max then
      delta *= -1
      coord = max - 1
    end
    if coord < 0 then
      delta *= -1
      coord = 0
    end
    [coord, delta]
  end
  
  def xMax
    @screen.w - $image.w
  end

  def yMax
    @screen.h - $image.h
  end
  
end

def detect_collisions(sprites)
  collisions = []
  for i in (0 ... sprites.size - 1) do
    for j in (i + 1 ... sprites.size) do
      if sprites[i].collide_with?(sprites[j])
        collisions << sprites[i]
        collisions << sprites[j]
      end
    end
  end
  collisions.uniq
end

sprites = (1..8).collect {Sprite.new(screen)}

background = screen.format.map_rgb(64, 64, 64)
while true
  while event = SDL::Event.poll
    case event
    when SDL::Event::KeyDown, SDL::Event::Quit
      exit
    end
  end

  screen.fill_rect(0,0,640,480,background)
  sprites.each {|i| i.move}
  detect_collisions(sprites).each {|i| i.bounce}
  sprites.each {|i| i.draw}
  screen.update_rect(0,0,0,0)
end

