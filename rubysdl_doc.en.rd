=begin

= SDL

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
* ((<JoyStick>))
* ((<TTF>))

== Error

SDL::Error is exception class. Almost all of errors in Ruby/SDL are
reported by this class.

=== super class

StandardError

== initialize

=== module functions in mudule SDL

--- init(flag)

--- initializedSystem(flag)

== video

some functions need SGE or SDL_image

=== functions in module SDL

--- checkVideoMode(w,h,bpp,flags)

--- videoInfo
      Return  video information in array. The contents are following.
      Values represent boolean value are true/false .
      Please read SDL document to know the meanings of this information.
      [ hw_available,
        wm_available,
	blit_hw,
	blit_hw_CC,
	blit_hw_A,
	blit_sw,
	blit_sw_CC,
	blit_sw_A,
	blit_fill,
	video_mem,
	vfmt,
	]
	
--- blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)

--- warpMouse(x,y)

--- rotateXYScaled(src,dst,x,y,angle,xscale,yscale)
      Need SGE
      Note that this function is different from SGE sge_rotate_xyscaled API
      in that src and dst are changing.
      Following two functions are same.
      This is equal to blitSurface.
      And note that this funcion ignores colorkey.

--- rotateScaled(src,dst,x,y,angle,scale)

--- rotate(src,dst,x,y,angle)

--- rotateScaledBlit(src,dst,x,y,angle,scale)
      Need SGE
      respect colorkey
      following function "rotateBlit" is same.

--- rotateBlit(src,dst,x,y,angle)

=== Surface

This class have image.

==== super class

Object

==== 

--- Surface.new(flag,w,h,format)

--- Surface.loadBMP(filename)

--- Surface.load(filename)
      Need SDL_image

==== method

--- Surface#displayFormat

--- Surface#setColorKey(flag,key)

--- Surface#fillRect(x,y,w,h)

--- Surface#setClipRect(x,y,w,h)

--- Surface#setAlpha(flag,alpha)

--- Surface#h

--- Surface#w

--- Surface#format

--- Surface#put(image,x,y)

--- Surface#getPixel(x,y) 
--- Surface#[](x,y)
      Need SGE

--- Surface#putPixel(x,y,pixel)
--- Surface#[]=(x,y,pixel)
      Need SGE

--- Surface#drawLine(x1,y1,x2,y2,color)
      Need SGE

--- Surface#drawRect(x,y,w,h,color)
      Need SGE

--- Surface#drawCircle(x,y,r,color)
      Need SGE

--- Surface#drawFilledCircle(x,y,r,color)
      Need SGE

--- Surface#rotateScaledSurface(angle,scale,bgcolor)
      Need SGE
      This function makes the instance of Surface with a rotated and scaled
      copy of "self". "angle" is the rotation angle in degrees.
      "scale" is the scaling value , 1.0 is the normal size.
      
--- Surface#rotateSurface(angle,bgcolor)
      
=== Screen

SDL display the image that the instance of "Screen" have.
Only SDL::setVideoMode makes this object.
In fact the class nemed "Screen" doesn't exist,and the object that
SDL::setVideoMode returns is the isntance of Surface with following
singleton methods.

==== super class

((<Surface>))

==== class method


==== method

--- Screen#updateRect(x,y,w,h)

--- Screen#flip

=== PixelFormat

The wrapper class of SDL_PixelFormat.
You can get the instance by Surface#format, and ((<Surface.new>)) can use
this object as "format".

==== super class

Object

==== class method

nothing

==== method

--- PixelFormat#MapRGB(r,g,b)

--- PixelFormat#MapRGBA(r,g,b,a)

--- PixelFormat#getRGB(pixel)

--- PixelFormat#getRGBA(pixel)

--- PixelFormat#bpp

== Event handling

=== Event

The class handling event.

==== super class

Object

==== class method

--- Event.new

==== method

--- Event#poll

--- Event#wait

--- Event#type

--- Event#info
      Return event information in array.
      This method handle all event.

--- Event#keyPress?

--- Event#keySym

--- Event#keyMod

--- Event#gain?

--- Event#appState

--- Event#mouseX

--- Event#mouseY

--- Event#mouseXrel

--- Event#mouseYrel

--- Event#mouseButton

--- Event#mousePress?

=== Key

The module defines key consants.
This module some functions to get the key state.

==== module functions

--- scan
      scan key state.

--- press?(key)
      Get key state that "scan" funcion scan.
      return true if "key" is pressed and return false if "key" is released.

--- modState

--- enableKeyRepeat(delay,interval)

--- disableKeyRepeat

== Mouse

The module mouse consants and mouse functuions.

=== module functions

--- state
      Return mouse state in array.
      Return value is following,

      [ x , y , pressLButton? , pressMButton? , pressRButton? ]


== audio

=== Mixer

The module that have sound functions and constants.
Note that volume is between 0 and 128.
Need SDL_mixer to use functions if this module.

==== module functions

--- open(frequency,format,channels,chunksize)

--- spec

--- playChannel(channel,wave,looping)

--- play?(channel)

--- setVolume(channel,volume)

--- halt(channel)

--- pause(chennel)

--- resume(channel)

--- pause?(channel)

=== Mixer::Wave

The class handling wave

==== super class

Object

==== class method

--- Mixer::Wave.load(filename)

==== method

--- Mixer::Wave#setVolume(volume)

== Window Manager 

=== WM

The module that have the functions for window management.

==== module functions

--- caption

--- setCaption(title,icon)

--- icon=(iconImage)

--- iconify

== CDROM

=== CD

The class represents CDROM drive.
Note that the information that you get with CD#numTracks is stored
when you call CD#status.

=== super class

Object

=== class method

--- CD.numDrive

--- CD.name(drive)

--- CD.open(drive)

=== method

--- CD#status

--- CD#play(start,length)

--- CD#playTrack(start_track,start_frame,ntracks,nframes)

--- CD#pause

--- CD#resume

--- CD#stop

--- CD#eject

--- CD#numTracks

--- CD#currentTrack

--- CD#currentFrame

--- CD#trackType(track)

--- CD#trackLength(track)

== Joystick handling

=== JoyStick

The class represents one joystick.

=== super class

Object

=== class method

--- JoyStick.pall?
      Return whether JoyStick.updateAll are called automatically.
      
--- JoyStick.pall?=(polling)

--- JoyStick.num

--- JoyStick.name(index)

--- JoyStick.open(index)

--- JoyStick.open?(index)

--- JoyStick.updateAll

=== method

--- JoyStick#index

--- JoyStick#numAxes

--- JoyStick#numBalls

--- JoyStick#numButtons

--- JoyStick#axis(axis)

--- JoyStick#hat(hat)

--- JoyStick#button(button)

--- JoyStick#ball(ball)

== Font handling

=== TTF

The class handles True Type Font.

==== super class

Object

==== class method

--- TTF.init
      You must call TTF.init first when you use True Type Font.

--- TTF.open(filename,size)

==== method

--- TTF#style

--- TTF#style=(style)

--- TTF#textSize(text)
      Get Text size on image in array. Return [x,y].

--- TTF#drawSolidUTF8(dest,text,x,y,r,g,b)
      Draw text on "dest" at ("x","y"). r,g,b are color elements of text.
      This function use colorkey internally.
      Text must be UTF-8 (you can use ascii code).

--- TTF#drawBlendedUTF8(dest,text,x,y,r,g,b)
      Similar to drawSolidUTF8.
      More beautiful than drawSolidUTF8,but more slowly than drawSolidUTF8.

=end
