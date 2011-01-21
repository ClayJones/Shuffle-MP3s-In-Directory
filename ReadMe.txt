This little application is used to shuffle MP3 files.

I have several MP3 players that I use in various ways, and several are so cheap
that they have no shuffle option.  I hate listening to music in the same order
over and over again, so I figured I would write a quick program to shuffle those
file right in the file system.

I start by using Window Media player to populate a playlist or two to an SD
card.  Say for example my E:\ drive.  This leaves me with a Music directory
filled with Artist name, then each of those is filled with Album Names, and
finally individual songs sorted by Track number.

I run this application and point it at the E:\ directory and click the Read
button.  It finds all of the files and randomizes them, then puts the new
random list up for me to see.  I don't know why I wouldn't like the order it
picks, but if I didn't, I could just click the read button again.

The app also shows me how many files were found and how big the whole lot is.

Once I'm sure I have the right files and a good order, I click the Write
button.  The files get shuffled into multiple directories and renamed starting
with 0000name.mp3, 0001name.mp3 , 0002name.mp3 etc.

The directories start as 0000, 0100, 0200, 0300, ets to break the directories
so they contain no more than 100 files each.  You have to do this because if you
are using FAT formatted media, you are limited by the number of files in a
directory.  It's possible that 100 is still too many if you have really long
filenames on your MP3s, but mine seem to work.

WARNING!  When you click the Write button, it's going to take quite a while
to do all of the renaming.  Flash media isn't really that fast so be patient.
I usually have around 600 files on a 4gig SD card and it takes about 5
minutes to rename them all.

I have included a pre-built binary, but I named it "RENAME_AT_YOUR_OWN_RISK".
If you want to use it you might need to google search for "visual studio sp1
redistributable" to download and get the files you need.  Install them, then
run "rename RENAME_AT_YOUR_OWN_RISK ShuffleMP3sInDirectory.exe" so you can
run it.  As always, virus scan before you do it.  I don't think I have any
viruses on my computer, but who ever does?

It works for me, and if you need it, feel free to use it yourself.

Clay