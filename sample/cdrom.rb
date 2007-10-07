require 'sdl'

SDL.init( SDL::INIT_CDROM )

num_drive = SDL::CD.num_drive
if num_drive == 0
  print "No cdrom drive is detected\n"
  exit
else
  print "#{num_drive} cdrom drives are detected\n"
end

print SDL::CD.index_name(0),"\n"
cd=SDL::CD.open(0)
cd.status
print cd.num_tracks," ",cd.status, "\n"

begin
  cd.play_tracks 1,0,1,0
  sleep 100
ensure
  cd.stop
end

