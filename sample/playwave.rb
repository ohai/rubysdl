# This sample needs a wave file `sample.wav', please prepare.
require 'sdl'

SDL::init(SDL::INIT_AUDIO)

SDL::Mixer.open(22050, SDL::Mixer::DEFAULT_FORMAT, 2, 512)
wave = SDL::Mixer::Wave.load("sample.wav")

SDL::Mixer.play_channel(0, wave, 0)

while SDL::Mixer.play?(0)
  sleep 1
end


