class <<SDL  
  alias init init
  alias inited_system initedSystem
end

# rubysdl_video.c
module SDL
  class Screen
    alias update_rect updateRect
    alias flip flip
  end
end

class << SDL
  alias get_video_surface getVideoSurface
  alias blit_surface blitSurface
  alias set_video_mode setVideoMode
  alias check_video_mode checkVideoMode
  alias list_modes listModes
  alias set_gamma setGamma
  alias get_gamma_ramp getGammaRamp
  alias set_gamma_ramp setGammaRamp
  alias video_info videoInfo
end

module SDL
  
  class << Surface
#   alias new new
    alias load_bmp loadBMP
  end

  class Surface
    alias display_format displayFormat
    alias display_format_alpha displayFormatAlpha
    alias set_color_key setColorKey
    alias fill_rect fillRect
    alias set_clip_rect setClipRect
    alias get_clip_rect getClipRect
    alias set_alpha setAlpha
#   alias h h
#   alias w w
#   alias flags flags
    alias set_palette setPalette
    alias set_colors setColors
    alias get_palette getPalette
    alias must_lock? mustLock?
#   alias lock lock
#   alias unlock unlock
    alias map_rgb mapRGB
    alias map_rgba mapRGBA
    alias get_rgb getRGB
    alias get_rgba getRGBA
#   alias bpp bpp
#   alias colorkey colorkey
#   alias alpha alpha
  end
end

# rubysdl_event.c
class << SDL::Event
# alias new new
  alias app_state appState
end

module SDL
  class <<Event
    alias enable_unicode enableUNICODE
    alias disable_unicode disableUNICODE
    alias enable_unicode? enableUNICODE?
  end
  
  class Event
# alias poll poll
# alias wait wait
# alias type type
    alias key_press? keyPress?
    alias key_sym keySym
    alias key_mod keyMod
# alias gain? gain?
    alias app_state appState
    alias mouse_x mouseX
    alias mouse_y mouseY
    alias mouse_xrel mouseXrel
    alias mouse_yrel mouseYrel
    alias mouse_button mouseButton
    alias mouse_press? mousePress?
# alias info info
  end
end

# rubysdl_event2.c
module SDL
  if defined?(Event2) then
    class << Event2
#     alias poll poll
#     alias wait wait
#     alias new new
#     alias push push
      alias app_state appState
      alias enable_unicode enableUNICODE
      alias disable_unicode disableUNICODE
      alias enable_unicode? enableUNICODE?
    end
  end
end

#rubysdl_mouse.c
#alias state state
#alias warp warp
#alias set_cursor_imp setCursor_imp
#alias show show
#alias hide hide

# rubysdl_sge_video.c
class << SDL
  if defined?(autoLock)
    alias auto_lock autoLock
    alias auto_lock? autoLock?
    alias auto_lock= autoLock=
    alias auto_lock_on autoLockON 
    alias auto_lock_off autoLockOFF
    alias rotate_scaled_blit rotateScaledBlit
    alias rotate_xy_scaled rotateXYScaled
#   alias transform transform
  end
end


module SDL
  class Surface
    # shared with rubysdl_sge_video.c and rubysdl_pixel.c
    alias get_pixel getPixel
    alias put_pixel putPixel
    if defined?(drawLine)
      #  alias [] []
      #  alias []= []=
      alias draw_line drawLine
      alias draw_rect drawRect
      alias draw_circle drawCircle
      alias draw_filled_circle drawFilledCircle
      alias draw_ellispe drawEllispe
      alias draw_filled_ellispe drawFilledEllispe
      alias rotate_scaled_surface rotateScaledSurface
      alias transform_surface transformSurface
    end
  end
end


# rubysdl_time.c
class << SDL
  alias get_ticks getTicks
  alias delay delay
end

module SDL
# rubysdl_cdrom.c
  class << CD
    alias num_drive numDrive
    alias index_name indexName
# alias open open
  end

  class CD
# alias status status
# alias play play
    alias play_tracks playTracks
    alias pause pause
# alias resume resume
# alias stop stop
# alias eject eject
    alias num_tracks numTracks
    alias current_track currentTrack
    alias current_frame currentFrame
    alias track_type trackType
    alias track_length trackLength
  end
end

# rubysdl_joystick.c
module SDL
  class << Joystick
#  alias poll poll
#  alias poll= poll=
# alias num num
    alias index_name indexName
# alias open open
# alias open? open?
    alias update_all updateAll
  end

  class Joystick
# alias index index
    alias num_axes numAxes
    alias num_balls numBalls
    alias num_hats numHats
    alias num_buttons numButtons
# alias axis axis
# alias hat hat
# alias button button
# alias ball ball
  end
end

# rubysdl_event_key.c
module SDL
  class << Key
# alias scan scan
# alias press? press?
    alias mod_state modState
    alias enable_key_repeat enableKeyRepeat
    alias disable_key_repeat disableKeyRepeat
  end
end

# rubysdl_wm.c
class << SDL::WM
# alias caption caption
  alias set_caption setCaption
# alias icon= icon=
# alias iconify iconify
end

# rubysdl_mixer.c
module SDL
  if defined?(Mixer) then
    class << Mixer
#     alias open open
#     alias spec spec
      alias play_channel playChannel
#     alias play? play?
      alias set_volume setVolume
      alias allocate_channels allocateChannels
#     alias halt halt
#     alias pause pause
#     alias resume resume
#     alias pause? pause?
      alias play_music playMusic
      alias fade_in_music fadeInMusic
      alias set_volume_music setVolumeMusic
      alias halt_music haltMusic
      alias fade_out_music fadeOutMusic
      alias pause_music pauseMusic
      alias resume_music resumeMusic
      alias rewind_music rewindMusic
      alias pause_music? pauseMusic?
      alias play_music? playMusic?
    end

    module Mixer
      class Wave
#       alias load load
	alias set_volume setVolume
      end
    end
    # class Mixer::Music
    # alias load load
  end
end

# rubysdl_image.c
# alias load load

# rubysdl_ttf.c
module SDL
  if defined?(TTF) then
    #alias init init
    #alias open open

    class TTF
#     alias style style
#     alias style= style=
      alias text_size textSize
#     alias faces faces
      alias fixed_width? fixedWidth?
      alias family_name familyName
      alias style_name styleName
      alias draw_solid_utf8 drawSolidUTF8
      alias draw_blended_utf8 drawBlendedUTF8
      alias draw_shaded_utf8 drawShadedUTF8
      alias render_solid_utf8 renderSolidUTF8
      alias render_blended_utf8 renderBlendedUTF8
      alias render_shaded_utf8 renderShadedUTF8
    end
  end
end

# rubysdl_smpeg.c
module SDL
  if defined?(MPEG) then
    class MPEG
#     alias load load
#     alias new new
#     alias info info
      alias enable_audio enableAudio
      alias enable_video enableVideo
#     alias status status
      alias set_volume setVolume
      alias set_display setDisplay
      alias set_loop setLoop
#     alias scaleXY scaleXY
#     alias scale scale
#     alias move move
      alias set_display_region setDisplayRegion
#     alias play play
#     alias pause pause
#     alias stop stop
#     alias rewind rewind
#     alias seek seek
#     alias skip skip
      alias render_frame renderFrame
      alias set_filter setFilter
    end
  end
end

# sdl.rb

class << SDL
  if defined?(rotateScaledSurface) then
    alias rotate_scaled_surface rotateScaledSurface
  end
end

class << SDL
  if defined?(rotateScaled) then
    alias rotate_scaled rotateScaled 
#   alias rotate
    alias rotate_blit rotateBlit
  end

  if defined?(transform) then
    alias transform_blit transformBlit
  end

  alias blit_surface2 blitSurface2
end
