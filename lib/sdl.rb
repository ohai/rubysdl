#
# Ruby/SDL   Ruby extention library for SDL
#  Copyright (C) 2001 Ohbayashi Ippei
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#  This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#  You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 
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
    rotateXYScaled(src,dst,x,y,angle,1,1)
  end

  def rotateBlit(src,dst,x,y,angle)
    rotateScaledBlit(src,dst,x,y,angle,1)
  end
end

