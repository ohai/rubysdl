
require 'rbconfig'
require 'fileutils'
require 'optparse'

dlldir = nil
option = { :noop => false, :verbose => true }

ARGV.options do |opt|
  opt.on('--dlldir DIR'){|dir| dlldir = dir}
  opt.on('--no-harm'){ option[:noop] = true }
  opt.on('--quiet'){ option[:verbose] = false }

  opt.parse!
end

dlldir ||= Config::CONFIG["bindir"]
sitelibdir = Config::CONFIG["sitelibdir"]
sitearchdir = Config::CONFIG["sitearchdir"]


FileUtils.mkpath(dlldir, option) 
FileUtils.mkpath(sitelibdir, option)
FileUtils.mkpath(sitearchdir, option)

Dir.glob("dll/*.dll"){|fname| FileUtils.install(fname, dlldir, option)}
Dir.glob("ext/*.so"){|fname| FileUtils.install(fname, sitearchdir, option)}
Dir.glob("lib/*.rb"){|fname| FileUtils.install(fname, sitelibdir, option)}


