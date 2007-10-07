# This sample needs a mod file `sample.it'.
#
require 'sdl'
SDL::init(SDL::INIT_AUDIO)

SDL::Mixer.open(22050)
music = SDL::Mixer::Music.load("sample.it")

SDL::Mixer.play_music(music,0)

while SDL::Mixer::play_music?
  sleep 1
end
