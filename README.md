# What's Ruby/SDL

Ruby/SDL is the ruby extension library to use SDL.
SDL(Simple DirectMedia Layer) is a cross-platform multimedia library      
designed to provide fast access to the graphics framebuffer and audio    
devices. See http://www.libsdl.org/ if you want to know SDL in detail.

# Platform

This library supports Linux, Win32, MacOS X.

This library perhaps works on FreeBSD and other standard
Unix-like systems.

# Library Needed

Ruby/SDL needs Ruby and SDL and optionally uses SDL_image, SDL_mixer,
SDL_ttf, SGE, OpenGL  and SMPEG.

Indispensable

- Ruby   http://www.ruby-lang.org/
- SDL   http://www.libsdl.org/

Optional

- SDL_mixer   http://www.libsdl.org/projects/SDL_mixer/index.html
- SDL_ttf   http://www.libsdl.org/projects/SDL_ttf/index.html
- SDL_image   http://www.libsdl.org/projects/SDL_image/index.html
- SGE   http://www.etek.chalmers.se/~e8cal1/sge/
- SMPEG  http://www.icculus.org/smpeg/
- SDLSKK http://www.kmc.gr.jp/~ohai/sdlskk.en.html

# INSTALL

Some packages are available: Debian's .deb, FreeBSD's ports,
or Windows' zipfile.

RubyGems are also available. You can install Ruby/SDL:

    gem install rubysdl

You can install from source code as follows:

1. install Ruby :
  See Ruby's document and install.

2. install SDL :
  Extract SDL's source and run `./configure`.
  And run `make`, `make install`.

3. install SGE :
  Run `make` and `make install`.
  
4. install SDL_mixer, SDL_ttf, SDL_image, SMPEG and SDLSKK :
  See documents of these libraries and install them .

5. install this extension library,Ruby/SDL :
  Run `ruby extconf.rb`, `make` and `make install`.

6. (Mac OS X user only) install rsdl :
  Run `gem install rsdl`.
  rsdl is a wrapper of the Ruby interpreter. This wrapper
  prepares Cocoa framework before running the interpreter.

# Use SGE without installing the library

You can use SGE without intalling:

    gem install rubysdl -- --enable-bundled-sge

or

    ruby extconf.rb --enable-bundled-sge

However, please do not use this option if you have
other way. Please install SGE separately if possible.

# USE Ruby/SDL

Write `require 'sdl'` in your Ruby script.
If your system is OS X, you must use rsdl command 
as an interpreter instead of ruby command.

See following documents and samples if you want to know the detail.

- `sample/*.rb` :
  Ruby/SDL samples. In some scripts, you have to prepare data such as
  `sample.wav` and `sample.mpg`.
- `rubysdl_ref.en.html` :
  The reference manual
- `rubysdl_ref.en.rd` :
  RD style reference manual(the contents is same as `rubysdl_ref.en.html`)
- documents of SDL :
  This is in SDL archive.
  Please read this when you want to know what does't describe
  these documents.

# OpenGL

You can use OpenGL with this library.

1. Setup OpenGL and SDL.
2. Install ruby-opengl (http://ruby-opengl.rubyforge.org/)
3. Run `ruby extconf.rb`, `make`, and `make install`
4. Write `require 'sdl'; require 'opengl'`  in your script.

We use OpenGL through ruby-opengl.
Please see SDL's document, OpenGL document and sample/testgl.rb .

# LICENSE

This library is distributed under GNU LGPL version 2.1 or later,
which can be found in the file `LICENSE`.
But samples, the Ruby scripts `sample/*.rb`, are not under LGPL, and
you can use their files freely.
 
# Thanks

Thanks to ...

- Tamura : some patches and advertising
- Akinori MUSHA : Porting on FreeBSD
- Danny van Bruggen : some ideas are from his library "RUDL"
- akira yamada : patch of SDL::TTF, making packages of Vine and Debian.
- Wayne Conrad : some patches
- Christian Neukirchen, Kent Dahl : send a patch
- Keffin Barnaby : some patches
- Rorien Dunn, Dennis Ranke, Keita Urashima : bug reports and useful information
- Yusuke TABATA : introduce me into SDL world
- SDLdoc Japanese translation project : base of Ruby/SDL's reference manual
- Hara Yutaka: Sample TrueTypeFont, MacOS X support
- Members of #sdl-fan-jp @ FriendChat: bug reports and advices
- Cyross Makoto: Windows support
- Dwayne C. Litzenberger: Some patches

# OTHER

Some functions, especially SDL::MPEG methods and pallete handling methods,
are not tested.

# AUTHORS

- Ippei Obayashi: main developer
- Cyross Makoto: Windows suppport
- yhara: MacOS X support

Please send comments and bug reports to the author.
Author's mail address: ohai@kmc.gr.jp
