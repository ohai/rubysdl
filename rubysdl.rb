require 'rubysdl.so'

module SDL
  class Screen
    def put(surface,x,y)
      blitSurface(surface,0,0,surface.w,surface.h,self,x,y)
    end
    
  end
end

