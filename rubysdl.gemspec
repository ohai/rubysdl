
Gem::Specification.new do |spec|
  spec.name = "rubysdl"
  spec.version = "1.3.0"
  spec.summary = "The simple ruby extension library to use SDL"
  spec.author = "Ohbayashi Ippei"
  spec.email = "ohai@kmc.gr.jp"
  spec.homepage = "http://www.kmc.gr.jp/~ohai/"
  spec.files = File.read("MANIFEST").split(/\n/)
  spec.test_files = []
  spec.extensions = ["extconf.rb"]
  spec.has_rdoc = false
end
