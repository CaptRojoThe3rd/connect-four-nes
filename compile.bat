@echo off

set path=%path%;..\bin\

set CC65_HOME=..\

c:\cc65\bin\cc65 -Oirs game.c --add-source
c:\cc65\bin\ca65 crt0.s
c:\cc65\bin\ca65 game.s -g

c:\cc65\bin\ld65 -C 17kb_rom.cfg -o connect_four.nes crt0.o game.o nes.lib -Ln labels.txt --dbgfile dbg.txt

del *.o

move /Y labels.txt BUILD\ 
move /Y game.s BUILD\ 
move /Y connect_four.nes BUILD\ 

BUILD\connect_four.nes