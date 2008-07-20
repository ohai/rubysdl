#
# Test for rwops
#
# This sample needs
#   track01.ogg: Ogg Vorbis file

require 'sdl'
require 'stringio'
require 'zlib'

SDL.init( SDL::INIT_VIDEO|SDL::INIT_AUDIO )
SDL::Mixer.open(22050)

screen = SDL::setVideoMode(640,480,16,SDL::SWSURFACE)
SDL::WM::setCaption('from_str.rb','from_str.rb')

str = File.read("icon.bmp")
img = SDL::Surface.loadBMPFromIO(StringIO.new(str))
img2 = File.open("icon.bmp", "rb"){|f| SDL::Surface.loadBMPFromIO(f) }
img3 = Zlib::GzipReader.open("icon.bmp.gz"){|f| SDL::Surface.loadBMPFromIO(f) }
img4 = File.open("icon.png", "rb"){|f| SDL::Surface.loadFromIO(f) }

mus = SDL::Mixer::Music.loadFromString(File.read("track01.ogg"))
wav = File.open('sample.wav'){|f| SDL::Mixer::Wave.loadFromIO(f) }

SDL.blitSurface(img,0,0,32,32,screen,100,100)
SDL.blitSurface(img2,0,0,32,32,screen,200,100)
SDL.blitSurface(img3,0,0,32,32,screen,300,100)
SDL.blitSurface(img4,0,0,32,32,screen,400,100)
screen.updateRect(0, 0, 0, 0)

SDL::Mixer.playMusic(mus, -1)
SDL::Mixer.playChannel(0, wav, 0)

while true
  while event = SDL::Event2.poll
    case event
    when SDL::Event2::KeyDown, SDL::Event2::Quit
      exit
    end
  end

  sleep 0.1
end
