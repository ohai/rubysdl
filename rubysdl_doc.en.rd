=begin

= SDL

Interface of this lib is very close to SDL.
So you also had better refer to SDL document.

All classes/modules in Ruby/SDL are in "module SDL".
In this module number starts 0,not 1.

== Class/Module
* ((<Error>))
* ((<Surface>))
  * ((<Screen>))
* ((<PixelFormat>))
* ((<Event>))
* ((<Key>)) (module)
* ((<Mouse>)) (module)
* ((<Mixer>)) (module)
* ((<Mixer::Wave>))
* ((<WM>)) (module)
* ((<CD>))
* ((<Joystick>))
* ((<TTF>))

== Error

SDL::Error is exception class. Almost all of errors in Ruby/SDL are
reported by this class.

=== super class

StandardError

== initialize

=== module functions in module SDL

--- init(flag)
      Initialize SDL.This should be called before all other SDL functions. The
      flags parameter specifies what part(s) of SDL to initialize.
        INIT_AUDIO
        INIT_VIDEO
        INIT_CDROM
        INIT_JOYSTICK

--- initializedSystem(flag)
      Not documented yet

== video

some functions need SGE or SDL_image

=== functions in module SDL

--- getVideoSurface
      Not documented yet

--- setVideoMode(w,h,bpp,flags)
      Set up a video mode with the specified width, height and bits-per-pixel.
      If bpp is 0, it is treated as the current display bits per pixel.
      Return the instanse of ((<Screen>)),if succeeded.
      Raise SDL::Error,if failed.
      * SWSURFACE
        
        Create the video surface in system memory.

      * HWSURFACE

        Create the video surface in video memory.

      * FULLSCREEN

        SDL will attempt to use a fullscreen mode.

      * SDL_DOUBLEBUF

        Enable double buffering.
        Calling ((<Screen#flip>)) will flip the  buffers and update
        the screen.

--- checkVideoMode(w,h,bpp,flags)
      Check to see if a particular video mode is supported.
      Returns 0 if the requested mode is not supported under any
      bit depth, or returns the bits-per-pixel of the closest available mode
      with the given width, height and requested surface flags (see 
      SDL_SetVideoMode).

      The bits-per-pixel value returned is only a suggested mode. You can
      usually request and bpp you want when setting the video mode and SDL will
      emulate that color depth with a shadow video surface.
      
      The arguments to checkVideoMode are the same ones you would pass to
      ((<setVideoMode>)).

--- listModes(flags)
      Returns availanel screen demensions for the given flags.
      Returns nil if there are no available dimensions.
      Returns true if any dimension is okay.
      Otherwise returns list of available dimensions in array.

--- setGamma(redgamma,greengamma,bluegamma)
      Sets the "gamma function" for the display of each color component.
      Gamma controls the brightness/contrast of colors displayed on
      the screen. A gamma value of 1.0 is identity
      (i.e., no adjustment is made).


--- autoLock
      Needs SGE

      Returns whether Ruby/SDL locks surface automatically when
      need.Default is true.

      Please see ((<Surface::lock>)) to know more.

--- autoLock=(autolocking)
      Needs SGE

      Set whether Ruby/SDL locks surface automatically when
      need.

--- videoInfo
      Return  video information in the object of VideoInfo .
      The contents are following.
      Values represent boolean value are true/false .
      Please read SDL document to know about this information.
        VideoInfo#hw_available
        VideoInfo#wm_available
        VideoInfo#blit_hw
        VideoInfo#blit_hw_CC
        VideoInfo#blit_hw_A
        VideoInfo#blit_sw
        VideoInfo#blit_sw_CC
        VideoInfo#blit_sw_A
        VideoInfo#blit_fill
        VideoInfo#video_mem
        VideoInfo#vfmt
	
--- blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)
      This performs a fast blit from the source surface to the destination
      surface.

      If srcX=srcY=srcW=srcH=0,the entire surface is copied.

      The blit function should not be called on a locked surface.

--- blitSurface2(src,srcRect,dst,dstRect)
      You use this function like:
      blitSurface2(src,[0,32,32,32],dst,[100,200])

--- rotateXYScaled(src,dst,x,y,angle,xscale,yscale)
      Needs SGE

      Note that this function is different from SGE sge_rotate_xyscaled API
      in that src and dst are changing.
      Following two functions are same.
      This is equal to blitSurface.

      And note that this function ignores colorkey.

--- rotateScaled(src,dst,x,y,angle,scale)
      Same as above, but with xscale=yscale.

--- rotate(src,dst,x,y,angle)
      Same as above, but with xscale=yscale=1.0.

--- rotateScaledBlit(src,dst,x,y,angle,scale)
      Needs SGE

      Same as rotateScaled,but respect colorkey.

--- rotateBlit(src,dst,x,y,angle)
      Same as rotateScaledBlit,but with scale=1.0;

=== Surface

This class have image.

==== super class

Object

==== class method

--- Surface.new(flag,w,h,format)
      Not documented yet

--- Surface.loadBMP(filename)
      Loads a image from a named Windows BMP file and return
      the instance of ((<Screen>)).

      Raise SDL::Error if you have an error,for example file didn't exist.
      
--- Surface.load(filename)
      Needs SDL_image
      Loads a image from a named Windows BMP file and return
      the instance of ((<Screen>)).

      Available formats are BMP,PPX,XPM,PCX,GIF,JPEG,PNG,TGA.

==== method

--- Surface#displayFormat
      This method copies self to a new surface of the pixel
      format and colors of the video framebuffer, suitable for fast blitting
      onto the display surface.
      
      If you want to take advantage of hardware colorkey or alpha blit
      acceleration, you should set the colorkey and alpha value before calling
      this method.

--- Surface#setColorKey(flag,key)
      Sets the color key (transparent pixel) in a blittable surface and enables
      or disables RLE blit acceleration.
      If flag is SDL_SRCCOLORKEY then key is the transparent pixel value in the
      source image of a blit.
      If flag is OR'd with SDL_RLEACCEL then the surface will be draw using RLE
      acceleration when drawn with blitting.

--- Surface#fillRect(x,y,w,h)
      This function performs a fast fill of the given rectangle with color.

--- Surface#setClipRect(x,y,w,h)
      Sets the clipping rectangle for a surface. When this surface is the
      destination of a blit, only the area within the clip rectangle will
      be drawn into.
      
      The rectangle pointed to by rect will be clipped to the edges of the
      surface so that the clip rectangle for a surface can never fall
      outside the edges of the surface.


--- Surface#setAlpha(flag,alpha)
      SDL_SetAlpha is used for setting the per-surface alpha and/or enabling
      and disabling per-pixel alpha blending.
      
      flags is used to specify whether alpha blending should be used
      (SDL::SRCALPHA) and whether the surface should use RLE acceleration
      for blitting (SDL::RLEACCEL). flags can be an OR'd combination of these
      two options, one of these options or 0. If SDL_SRCALPHA is not passed
      as a flag then all alpha information is ignored when blitting
      the surface. The alpha parameter is the per-surface alpha value,
      a surface need not have an alpha channel to use
      per-surface alpha and blitting can still be accelerated with
      SDL_RLEACCEL. Setting the per-surface alpha value to 0 disables
      per-surface alpha blending.

--- Surface#h
      Return height.

--- Surface#w
      Return width.

--- Surface#format
      Return pixel format.
      See ((<PixelFormat>)).

--- Surface#put(image,x,y)
      Draw image on (x,y) in self.
      This method are implemented using blitSurface.

--- Surface#lock
      This method sets up a surface for directly accessing the pixels.You call
      this before calling ((<Surface#getPixel>)) , ((<Surface#drawLine>)) or
      some other mehtods of Surface.

      Between calls to Surface#lock and Surface#unlock, you can use
      methods that 'need locking'.
      Once you are done accessing the surface, you
      should use SDL_UnlockSurface to release it.
      
      Not all surfaces require locking. If ((<Surface#mustLock?>))  returns
      false, then you can read and write to the surface at any time, and the
      pixel format of the surface will not change.
      
      No operating system or library calls should be made between lock/unlock
      pairs, as critical system locks may be held during this time.
      
      If ((<autoLock>)) returns true,you don't have to call this methods
      because this library locks surface automatically.

--- Surface#unlock
      Unlock the surface.

--- Surface#mustLock?
      Returns true if you must lock surface for directly accessing the pixels,
      otherwise returns false.

--- Surface#getPixel(x,y) 
--- Surface#[](x,y)
      Needs SGE ,Needs lock
      Gets the color of the specified pixel.

--- Surface#putPixel(x,y,pixel)
--- Surface#[]=(x,y,pixel)
      Needs SGE ,Needs lock
      Writes a pixel to the specified position.

--- Surface#drawLine(x1,y1,x2,y2,color)
      Needs SGE ,Needs lock
      Draws a line from (x1,y1) to (x2,y2).

--- Surface#drawRect(x,y,w,h,color)
      Needs SGE ,Needs lock
      Draws a rectangle.

--- Surface#drawCircle(x,y,r,color)
      Needs SGE ,Needs lock
      Draws a circle.

--- Surface#drawFilledCircle(x,y,r,color)
      Needs SGE ,Needs lock
      Draws a filled circle.

--- Surface#rotateScaledSurface(angle,scale,bgcolor)
      Needs SGE
      This function makes the instance of Surface with a rotated and scaled
      copy of "self". "angle" is the rotation angle in degrees.
      "scale" is the scaling value , 1.0 is the normal size.
      
--- Surface#rotateSurface(angle,bgcolor)
      Same as above,but with scale=1.0 .

--- Surface#mapRGB(r,g,b)
    Maps the RGB color value to the pixel format of specified surface
    and returns the pixel value as a integer.

--- Surface#mapRGBA(r,g,b,a)
      Same as above,but includes alpha value.

--- Surface#getRGB(pixel)
      Get RGB component values from a pixel stored in the specified pixel
      format.Returns r,g,b value in array as [r,g,b].

--- Surface#getRGBA(pixel)
      Same as above, but return value includes alplah value.
      Returns r,g,b,a in arrary as [r,g,b,a].

--- Surface#bpp
      Return bits per pixel on this surface.

--- Surface#colorkey
      Returns colorkey on this surface.

--- Surface#alpha
      Returns alpha on this surface.

=== Screen

SDL display the image that the instance of "Screen" have.
Only SDL::setVideoMode makes this object.
In fact the class named "Screen" doesn't exist,and the object that
SDL::setVideoMode returns is the instance of Surface with following
singleton methods.

==== super class

((<Surface>))

==== class method


==== method

--- Screen#updateRect(x,y,w,h)
      Makes sure the given  rectangle is updated on the given screen.
      Thhis function should not be called while screen is locked.

      If 'x', 'y', 'w' and 'h' are all 0, SDL_UpdateRect will update the entire
      screen.

--- Screen#flip
      On hardware that supports double-buffering, this method sets up a flip
      and returns. The hardware will wait for vertical retrace, and then swap
      video buffers before the next video surface blit or lock will return. On
      hardware that doesn't support double-buffering, this is equivalent to
      calling ((<Screen#updateRect>))(0, 0, 0, 0)
      
      The DOUBLEBUF flag must have been passed to ((<setVideoMode>)), when
      setting the video mode for this method to perform hardware flipping.

=== PixelFormat

((<Surface>)) has all methods of ((<PixelFormat>)),
and you should use ((<Surface>)) instead of ((<PixelFormat>)).

The wrapper class of SDL_PixelFormat.
You can get the instance by Surface#format, and ((<Surface.new>)) can use
this object as "format".

In SDL you can use some type surface,for example 32bit color surface and
16bit surface.Therefore you need the information about that type,and
object of PixelFormat represents this information.

In this lib,you use one integer to specify color.To change from r,g,b value
to this integer you use ((<PixelFormat#mapRGB>)) and to change this integer
to r,g,b value ((<PixelFormat#getRGB>))

==== super class

Object

==== class method

nothing

==== method

--- PixelFormat#mapRGB(r,g,b)
    Maps the RGB color value to the specified pixel format and returns the
    pixel value as a integer.

--- PixelFormat#mapRGBA(r,g,b,a)
      Same as above,but includes alpha value.

--- PixelFormat#getRGB(pixel)
      Get RGB component values from a pixel stored in the specified pixel
      format.Returns r,g,b value in array as [r,g,b].

--- PixelFormat#getRGBA(pixel)
      Same as above, but return value includes alplah value.
      Returns r,g,b,a in arrary as [r,g,b,a].

--- PixelFormat#bpp
      Return bits per pixel on this format.

--- PixelFormat#colorkey
      Not documented yet

--- PixelFormat#alpha
      Not documented yet

== Event handling

=== Event

The class handling event.

==== super class

Object

==== class method

--- Event.new

==== method

--- Event#poll
      Polls for currently pending events, and returns 1 if there are any
      pending events, or 0 if there are none available.
      If there are any events ,the next event is removed from the queue
      and stored in self.

--- Event#wait
      Waits indefinitely for the next available event, returning 1, or 0 if
      there was an error while waiting for events.
      The next event is removed from the queue and stored in self.

--- Event#type
      Returns the type of stored event.
        Event::ACTIVEEVENT 
        Event::KEYDOWN
        Event::KEYUP
        Event::MOUSEMOTION
        Event::MOUSEBUTTONDOWN
        Event::MOUSEBUTTONUP
        Event::JOYAXISMOTION
        Event::JOYBALLMOTION
        Event::JOYHATMOTION
        Event::JOYBUTTONDOWN
        Event::JOYBUTTONUP
        Event::QUIT
        Event::SYSWMEVENT
        Event::VIDEORESIZE

--- Event#info
      Return event information in array.
      This method can handle all event.

--- Event#keyPress?
      Returns true when you got key event and a key pressed down,
      otherwise returns false.

--- Event#keySym
      Returns SDL vertual keysym.

--- Event#keyMod
      Returns Current Key modifiers.

--- Event#gain?
      On ACTIVEEVENT,
      returns true when gaining focus in this event,otherwise retursn false.

--- Event#appState
      Returns the kind of ActiveEvent.
      This value is following.
        Event::APPMOUSEFOCUS
        Event::APPINPUTFOCUS
        Event::APPACTIVE

--- Event#mouseX
      Returns the X coordinate of the mouse.

--- Event#mouseY
      Returns the Y coordinate of the mouse.

--- Event#mouseXrel
      Returns the relative motion in the X direction.

--- Event#mouseYrel
      Returns the relative motion in the Y direction.

--- Event#mouseButton
      Returns the mouse button index.
        SDL::Mouse::BUTTON_LEFT  
        SDL::Mouse::BUTTON_MIDDLE  
        SDL::Mouse::BUTTON_RIGHT 

--- Event#mousePress?
      Returns true when this mouse event is SDL::Event::MOUSEBUTTONDOWN,
      otherwise returns false.

=== Key

The module defines key constants.
This module has some functions to get the key state.

==== module functions

--- scan
      scan key state.

--- press?(key)
      Get key state that "scan" function scan.
      return true if "key" is pressed and return false if "key" is released.

--- modState
      Returns the current of the modifier keys (CTRL,ATL,etc.).
      The return value can be an OR'd combination of following constants.  
        Key::MOD_NONE
        Key::MOD_LSHIFT
        Key::MOD_RSHIFT
        Key::MOD_LCTRL
        Key::MOD_RCTRL
        Key::MOD_LALT
        Key::MOD_RALT
        Key::MOD_LMETA
        Key::MOD_RMETA
        Key::MOD_NUM
        Key::MOD_CAPS
        Key::MOD_MODE
        Key::MOD_RESERVED
        Key::MOD_CTRL = Key::MOD_LCTRL|Key::MOD_RCTRL
        Key::MOD_SHIFT = Key::MOD_LSHIFT|Key::MOD_RSHIFT
        Key::MOD_ALT = Key::MOD_LALT|Key::MOD_RALT
        Key::MOD_META = Key::MOD_LMETA|Key::MOD_RMETA

--- enableKeyRepeat(delay,interval)
      Set keyboard repeat rate.

--- disableKeyRepeat
      Disables key repeat.

== Mouse

The module mouse constants and mouse functions.

=== module functions

--- state
      Return mouse state in array.
      Return value is following,
        [ x , y , pressLButton? , pressMButton? , pressRButton? ]

--- warp(x,y)
      Set the position of the mouse cursor (generates a mouse motion event).

--- show
      Show mouse cursor.

--- hide
      Hide mouse cursor.

--- setCursor(bitmap,white,black,transparent,inverted,hot_x=0,hot_y=0)
      Change mouse cursor. bitmap is form of cursor,instance of
      ((<Surface>)).The cursor width must be a multiple of 8.
      The cursor is created in black and white according to
      bitmap and white,black,transparent,inverted.
      white,black,transparent,inverted represents that elements in
      bitmap as pixel value.

== audio

=== Mixer

The module that have sound functions and constants.
Note that volume is between 0 and 128.
Needs SDL_mixer to use functions if this module.

==== module functions

--- open(frequency=Mixer::DEFAULT_FREQUENCY,format=Mixer::DEFAULT_FORMAT,cannels=Mixer::DEFAULT_CHANNELS,chunksize=4096)
      Initializes SDL_mixer.

--- spec
      Returns the audio spec in array.
        [ rate,format,channels ]

--- playChannel(channel,wave,looping)
      Play a wave on a specific channel.

      If the specified channel is -1, play on the first free channel.
      If 'loops' is greater than zero, loop the sound that many times.
      If 'loops' is -1, loop inifinitely (~65000 times).

      Returns which channel was used to play the sound.

--- play?(channel)
      Returns whether specific channel is playing or not.

      If the specified channel is -1, check all channels.

--- setVolume(channel,volume)
      Set the volume in the range of 0-128 of a specific channel.
      If the specified channel is -1, set volume for all channels.
      Returns the original volume.
      If the specified volume is -1, just return the current volume.

--- halt(channel)
      Halt playing of a particular channel

--- pause(chennel)
      Pause a particular channel

--- resume(channel)
      Resume a particular channel

--- pause?(channel)
      Returns whether a particular channel is pausing.

--- playMusic(music,loops)
      Play a music.
      
--- fadeInMusic(music,loops,ms)
      Fade in the given music in ms milliseconds.
      The meaning of loops is same as in ((<Mixer::playChannel>))

--- setVolumeMusic(volume)
      Sets the volume of music.

--- haltMusic
      Halts music.

--- fadeOutMusic(ms)
      Fade out the music in ms milliseconds.

--- pauseMusic
      Pauses music.

--- resumeMusic
      Resumes music.

--- rewindMusic
      Rewinds music.

--- pauseMusic?
      Returns whether the music is pausing.

--- playMusic?
      Returns whether the music is playing.

=== Mixer::Wave

The class handling wave

==== super class

Object

==== class method

--- Mixer::Wave.load(filename)
      Loads a wave file and returns the object of ((<Mixer::Wave>)).

==== method

--- Mixer::Wave#setVolume(volume)
      Set volume of self.

=== Mixer::Music

==== super class

Object

==== class method

--- Mixer::Music.load(filename)
      Loads a music (.mod .s3m .it .xm .mid .mp3) file and returns the
      object of ((<Mixer::Music>)).

      You have to setup your environment to play MIDI file and MP3 file.

== Window Manager 

=== WM

The module that have the functions for window management.

==== module functions

--- caption
      Returns captions of the window title and icon name.

--- setCaption(title,icon)
      Set captions of the window title and icon name.

--- icon=(iconImage)
      Sets the icon for the display window.
      
      This function must be called before the first call to
      setVideoMode.

      It takes an icon surface.

--- iconify
      If the application is running in a window managed environment SDL
      attempts to iconify/minimise it. If SDL_WM_IconifyWindow is successful,
      the application will receive a SDL_APPACTIVE loss event.

== CDROM

=== CD

The class represents CDROM drive.

Note that the information that you get with CD#numTracks is stored
when you call CD#status.

A CD-ROM is organized into one or more tracks, each consisting of a
certain number of "frames". Each frame is ~2K in size, and at normal
playing speed, a CD plays 75 frames per second. SDL works with the
number of frames on a CD.

=== super class

=== class method

--- CD.numDrive
      Returns the number of CD-ROM drives on the system.

--- CD.name(drive)
      Returns a human-readable, system-dependent identifier for the CD-ROM.
      drive is the index of the drive. Drive indices start to 0 and end at
      SDL_CDNumDrives()-1.

--- CD.open(drive)
      Opens a CD-ROM drive for access.
      It returns a object of CD.
      Raises SDL::Error if the drive was invalid or busy.
      Drives are numbered starting with 0. Drive 0 is the system default
      CD-ROM.

=== method

--- CD#status
      Stores the information of currentTrack,currentFrame,numTracks,trackType,
      trackLenght .
      This function returns the current status.
      Status is described like so:
        TRAYEMPTY
        STOPPED
        PLAYING
        PAUSED
        ERROR

--- CD#play(start,length)
      Plays the given cdrom, starting a frame start for length frames.

--- CD#playTrack(start_track,start_frame,ntracks,nframes)
      SDL_CDPlayTracks plays the given CD starting at track start_track, for
      ntracks tracks.
      
      start_frame is the frame offset, from the beginning of the start_track,
      at which to start. nframes is the frame offset, from the beginning of
      the last track (start_track+ntracks), at which to end playing.

      CD#playTracks should only be called after calling ((<CD#status>)) to get
      track information about the CD.

--- CD#pause
      Pauses play.

--- CD#resume
      Resumes play.

--- CD#stop
      Stops play.

--- CD#eject
      Ejects cdrom.

--- CD#numTracks
      Returns the number of tracks on the CD.

--- CD#currentTrack
      Returns current track.

--- CD#currentFrame
      Returns current frame.

--- CD#trackType(track)
      Returns the track type of given track.
      AUDIO_TRACK
      DATA_TRACK

--- CD#trackLength(track)
      Returns the track length in frame,of given track.

== Joystick handling

=== Joystick

The class represents one joystick.

=== super class

Object

=== class method

--- Joystick.pall
      Return whether Joystick.updateAll are called automatically.
      
--- JoyStick.pall=(polling)
      Set whether Joystick.updateAll are called automatically.

--- Joystick.num
      Returns the number of attached joysticks.

--- Joystick.name(index)
      Get the implementation dependent name of joystick. The index parameter
      refers to the N'th joystick on the system.

--- Joystick.open(index)
      Opens a joystick for use within SDL. The index refers to the N'th
      joystick in the system. A joystick must be opened before it game be
      used.
      Returns the object of Joystick.

--- Joystick.open?(index)
      Determines whether a joystick has already been opened within the
      application. index refers to the N'th joystick on the system.

--- Joystick.updateAll
      Updates the state(position, buttons, etc.) of all open joysticks.

=== method

--- Joystick#index
      Returns the index of self.

--- Joystick#numAxes
      Return the number of axes available from a previously opened 
      joystick.

--- Joystick#numBalls
      Return the number of trackballs available.

--- Joystick#numButtons
      Returns the number of buttons available.

--- Joystick#axis(axis_index)
      Returns a 16-bit signed integer representing the current position of the
      axis.
      On most modern joysticks the X axis is usually represented by axis 0 and
      the Y axis by axis 1. The value returned by Joystick#axis is a
      signed integer (-32768 to 32768) representing the current position of
      the axis, it maybe necessary to impose certain tolerances on these
      values to account for jitter. It is worth noting that some joysticks use
      axes 2 and 3 for extra buttons.

--- Joystick#hat(hat_index)
      The current state is returned as a 8bit unsigned integer
      which is defined as an OR'd combination of one or more of the following
        Joystick::HAT_CENTERED
        Joystick::HAT_UP
        Joystick::HAT_RIGHT
        Joystick::HAT_DOWN
        Joystick::HAT_LEFT
        Joystick::HAT_RIGHTUP
        Joystick::HAT_RIGHTDOWN
        Joystick::HAT_LEFTUP
        Joystick::HAT_LEFTDOWN

--- Joystick#button(button_index)
      Returns the current state of the given button.
      Returns true if the button is pressed,otherwise 0.

--- Joystick#ball(ball_index)
      Returns the motion deltas in array, [ dx,dy ] .
      Trackballs can only return relative motion since the last call to
      Joystick#ball, these motion deltas a placed into dx and dy.

== Font handling

This needs SDL_ttf.

=== TTF

The class handles True Type Font.

==== super class

Object

==== class method

--- TTF.init
      You must call TTF.init first when you use True Type Font.

--- TTF.open(filename,size)
      Open a font file and create a font of the specified point size.

==== method

--- TTF#style
      Returns the font style.

--- TTF#style=(style)
      Set font style.
      style is an OR'd conbination of one or more of the following
        TTF::STYLE_NORMAL
        TTF::STYLE_BOLD
        TTF::STYLE_ITALIC
        TTF::STYLE_UNDERLINE

--- TTF#textSize(text)
      Get Text size on image in array. Return [x,y].

--- TTF#drawSolidUTF8(dest,text,x,y,r,g,b)
      Draw text on "dest" at ("x","y"). r,g,b are color elements of text.
      This function use colorkey internally.
      Text must be UTF-8 (you can use ASCII code).

--- TTF#drawBlendedUTF8(dest,text,x,y,r,g,b)
      Similar to drawSolidUTF8.
      More beautiful than drawSolidUTF8,but more slowly than drawSolidUTF8.

== Time

=== module function

--- getTicks
      Get the number of milliseconds since the SDL library initialization.
      Note that this value wraps if the program runs for more than ~49 days.

--- delay(ms)
      Wait a specified number of milliseconds before returning. this
      function will wait at least the specified time, but possible
      longer due to OS scheduling.

=end
