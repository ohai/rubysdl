# -*- Ruby -*-

Gem::Specification.new do |spec|
  spec.name = "rubysdl"
  spec.version = "2.2.0"
  spec.summary = "The simple ruby extension library to use SDL"
  spec.description = <<-EOS
    Ruby/SDL is an extension library to use SDL(Simple DirectMedia
    Layer). This library enables you to control audio, keyboard,
    mouse, joystick, 3D hardware via OpenGL, and 2D video 
    framebuffer. Ruby/SDL is used by games and visual demos.
  EOS
  spec.license = "LGPL"
  spec.author = "Ohbayashi Ippei"
  spec.email = "ohai@kmc.gr.jp"
  spec.homepage = "http://www.kmc.gr.jp/~ohai/rubysdl.en.html"
  spec.files = `git ls-files`.split(/\n/)
  spec.test_files = []
  spec.extensions = ["extconf.rb"]
  spec.has_rdoc = false
end
