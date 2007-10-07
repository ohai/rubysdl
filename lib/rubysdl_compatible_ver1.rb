
module SDL
  # compatible method definition
  module_function
  def blitSurface(*args)
    SDL::Surface.blit(*args)
  end

  def getVideoSurface
    SDL::Screen.get
  end

  def setVideoMode(*args)
    SDL::Screen.open(*args)
  end
  
  def checkVideoMode(*args)
    SDL::Screen.checkMode(*args)
  end

  def listModes(*args)
    SDL::Screen.listModes(*args)
  end

  def setGamma(*args)
    SDL::Screen.setGamma(*args)
  end

  def getGammaRamp(*args)
    SDL::Screen.getGammaRamp(*args)
  end

  def setGammaRamp(*args)
    SDL::Screen.setGammaRamp(*args)
  end

  def videoDriverName
    SDL::Screen.driverName
  end
  
  def videoInfo
    SDL::Screen.info
  end
  
  def getGLAttr(arg)
    GL.getAttr(arg)
  end

  def setGLAttr(*args)
    GL.setAttr(*args)
  end

  def GLSwapBuffers
    GL.swap_buffers
  end
  
  GL_RED_SIZE = GL::RED_SIZE
  GL_GREEN_SIZE = GL::GREEN_SIZE
  GL_BLUE_SIZE = GL::BLUE_SIZE
  GL_ALPHA_SIZE = GL::ALPHA_SIZE
  GL_BUFFER_SIZE = GL::BUFFER_SIZE
  GL_DOUBLEBUFFER = GL::DOUBLEBUFFER
  GL_DEPTH_SIZE = GL::DEPTH_SIZE
  GL_STENCIL_SIZE = GL::STENCIL_SIZE
  GL_ACCUM_RED_SIZE = GL::ACCUM_RED_SIZE
  GL_ACCUM_GREEN_SIZE = GL::ACCUM_GREEN_SIZE
  GL_ACCUM_BLUE_SIZE = GL::ACCUM_BLUE_SIZE
  GL_ACCUM_ALPHA_SIZE = GL::ACCUM_ALPHA_SIZE
  
  class Surface
    extend Forwardable

    def_delegators(:format,
                   :alpha, :colorkey, :mapRGB, :mapRGBA,
                   :getRGB, :getRGBA, :bpp, :bytesPerPixel,
                   :Rmask, :Gmask, :Bmask, :Amask,
                   :Rloss, :Gloss, :Bloss, :Aloss,
                   :Rshift, :Gshift, :Bshift, :Ashift)
    
    def getPalette
      format.palette
    end
  end

  Event2 = Event

  if defined?(CollisionMap)
    class Surface
      def drawAALine(x1, y1, x2, y2, col)
        drawLine(x1, y1, x2, y2, col, true)
      end
      def drawAALineAlpha(x1, y1, x2, y2, col, alpha)
        drawLine(x1, y1, x2, y2, col, true, alpha)
      end
      def drawLineAlpha(x1, y1, x2, y2, col, alpha)
        drawLine(x1, y1, x2, y2, col, false, alpha)
      end

      def drawAACircle(x, y, r, color)
        drawCircle(x, y, r, color, false, true)
      end
      def drawAAFilledCircle(x, y, r, color)
        drawCircle(x, y, r, color, true, true)
      end
      def drawAACircleAlpha(x, y, r, color, alpha)
        drawCircle(x, y, r, color, false, true, alpha)
      end
      def drawCircleAlpha(x, y, r, color, alpha)
        drawCircle(x, y, r, color, false, false, alpha)
      end
      def drawAACircleAlpha(x, y, r, color, alpha)
        drawCircle(x, y, r, color, false, true, alpha)
      end
      def drawFilledCircleAlpha(x, y, r, color, alpha)
        drawCircle(x, y, r, color, true, false, alpha)
      end
      def drawFilledCircle(x, y, r, color)
        drawCircle(x, y, r, color, true)
      end
      

      def drawRectAlpha(x1, y1, x2, y2, color, alpha)
        drawRect(x1, y1, x2, y2, color, false, alpha)
      end
      def drawFilledRectAlpha(x1, y1, x2, y2, color, alpha)
        drawRect(x1, y1, x2, y2, color, true, alpha)
      end

      def drawFilledEllipse(x, y, rx, ry, color)
        drawEllipse(x, y, rx, ry, color, true)
      end
      def drawAAFilledEllipse(x, y, rx, ry, color)
        drawEllipse(x, y, rx, ry, color, true, true)
      end
      def drawFilledEllipseAlpha(x, y, rx, ry, color, alpha)
        drawEllipse(x, y, rx, ry, color, true, false, alpha)
      end

      def drawAAEllipse(x, y, rx, ry, color)
        drawEllipse(x, y, rx, ry, color, false, true)
      end
      def drawAAEllipseAlpha(x, y, rx, ry, color, alpha)
        drawEllipse(x, y, rx, ry, color, false, true, alpha)
      end
      def drawEllipseAlpha(x, y, rx, ry, color, alpha)
        drawEllipse(x, y, rx, ry, color, false, false, alpha)
      end

      def drawAABezier(x1,y1,x2,y2,x3,y3,x4,y4,level,color)
        drawBezier(x1,y1,x2,y2,x3,y3,x4,y4,level,color,true)
      end
      def drawBezierAlpha(x1,y1,x2,y2,x3,y3,x4,y4,level,color,alpha)
        drawBezier(x1,y1,x2,y2,x3,y3,x4,y4,level,color,false,alpha)
      end
      def drawAABezierAlpha(x1,y1,x2,y2,x3,y3,x4,y4,level,color,alpha)
        drawBezier(x1,y1,x2,y2,x3,y3,x4,y4,level,color,true,alpha)
      end

    end

    def autoLock?; Surface.autoLock?; end
    def autoLock; Surface.autoLock?; end
    def autoLockON; Surface.autoLockON; end
    def autoLockOFF; Surface.autoLockOFF; end
    def autoLock=(val)
      if val then autoLockON else autoLockOFF end
    end
    def transform(src,dst,angle,xscale,yscale,px,py,qx,qy,flags)
      Surface.transformDraw(src,dst,angle,xscale,yscale,px,py,qx,qy,flags)
    end
    def transformBlit(src,dst,angle,xscale,yscale,px,py,qx,qy,flags)
      Surface.transformBlit(src,dst,angle,xscale,yscale,px,py,qx,qy,flags)
    end
  end

  # aliases

  class << self
    alias get_video_surface getVideoSurface
    alias blit_surface blitSurface
    alias set_video_mode setVideoMode
    alias check_video_mode checkVideoMode
    alias list_modes listModes
    alias set_gamma setGamma
    alias get_gamma_ramp getGammaRamp
    alias set_gamma_ramp setGammaRamp
    alias video_info videoInfo
    alias video_driver_name videoDriverName
  end

  class Surface
    alias map_rgb mapRGB
    alias map_rgba mapRGBA
    alias get_rgb getRGB
    alias get_rgba getRGBA
    alias get_palette getPalette
  end

  if defined?(CollisionMap)
    class << self
      alias auto_lock? autoLock?
      alias auto_lock_on autoLockON 
      alias auto_lock_off autoLockOFF
      alias auto_lock autoLock
      alias auto_lock= autoLock=
      alias transform_blit transformBlit
    end

    class Surface
      alias draw_filled_circle drawFilledCircle
      alias draw_aa_line drawAALine
      alias draw_aa_circle drawAACircle
      alias draw_aa_filled_circle drawAAFilledCircle
      alias draw_aa_ellipse drawAAEllipse
      
      alias draw_line_alpha drawLineAlpha
      alias draw_rect_alpha drawRectAlpha
      alias draw_filled_rect_alpha drawFilledRectAlpha
      alias draw_circle_alpha drawCircleAlpha
      alias draw_filled_circle_alpha drawFilledCircleAlpha
      alias draw_ellipse_alpha drawEllipseAlpha
      alias draw_filled_ellipse_alpha drawFilledEllipseAlpha
      alias draw_filled_ellipse drawFilledEllipse
      
      alias draw_aa_line_alpha drawAALineAlpha
      alias draw_aa_circle_alpha drawAACircleAlpha
      alias draw_aa_ellipse_alpha drawAAEllipseAlpha

      alias draw_bezier drawBezier
      alias draw_aa_bezier drawAABezier
      alias draw_bezier_alpha drawBezierAlpha
      alias draw_aa_bezier_alpha drawAABezierAlpha
    end
  end

  if defined?(GL)
    class << self
      alias set_GL_attr setGLAttr
      alias get_GL_attr getGLAttr
      alias GL_swap_buffers GLSwapBuffers 
    end
  end
end
