
module SDL
  module_function
  
  def setVideoMode(*args)
    SDL::Screen.open(*args)
  end
  def blitSurface(*args)
    SDL::Screen.blit(*args)
  end

  class Surface
    extend Forwardable
    def_delegators :format, :mapRGB, :mapRGBA, :getRGB, :getRGBA
    def_delegators :format, :bpp, :colorkey, :alpha
    def_delegators :format, :Rmask, :Gmask, :Bmask, :Amask
  end

  Event2 = Event
end
