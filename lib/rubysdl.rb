require 'rubysdl.so'


module SDL
  class Surface
    def put(surface,x,y)
      SDL::blitSurface(surface,0,0,surface.w,surface.h,self,x,y)
    end

    def rotateSurface(angle,bgcolor)
      rotateScaledSurface(angle,1.0,bgcolor)
    end
  end

  module_function
    
  def rotateScaled(src,dst,x,y,angle,scale)
    rotateXYScaled(src,dst,x,y,angle,scale,scale)
  end
  def rotate(src,dst,x,y,angle)
    rotateXYScaled(src,dst,x,y,angle,1.0,1.0)
  end

  def rotateBlit(src,dst,x,y,angle)
    rotateScaledBlit(src,dst,x,y,angle,1.0)
  end
end

