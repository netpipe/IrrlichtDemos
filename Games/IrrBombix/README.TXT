This is a XBomberBox2 clone. 

More a very dirty hack of different great softwares.
Expect random and deterministic sigsegvs, memory leaks everywhere. It even crashes everytime you quit it! 

YOU ARE RUNNING THIS SOFTWARE AT YOUR OWN RISK

But it runs already not too bad and is not intended to do any harm.
If it does please change it.


Installation:
for ubuntu 10/11:
    run ./install.sh in the IrrBombix dir.
    other distros:
    inspect ./install.sh and adapt accordingly

first run:
      it creates config/grapics.xml and config/sound.xml based on your input on the terminal. So run it from a terminal!

in Game:
   everything could be done with configured controller button1 select button2 back, F10 for pause menu, ESC for instant quit. (F9 for a Screenshot)
   default keys (Player2 directions  a,w,s,y buttons q,d Player1 directions  4,8,6,2 buttons 0,+)

configure Controllers:
   configuring keyboard controllers requires you curently to edit config/controller<Number>. There should be 2 default ones.
   The keycodes should be in the file, you can also see them in the terminal when you hit the key in the game.
   You can add Keyboard Controllers by adding a new config/controller<higest Number+1> file.
   Joystick should work out of the box. You may have to calibrate them by pressing all directions once in the Game.
   
configure display:
  edit config/graphics.xml to your desire or
  remove config/graphics.xml to reconfigure the graphic engine via terminal on the next start

configure sound:
  remove config/sound.xml to reconfigure the sound engine via terminal on the next start

configure net:
  you can also connect per lan. It requires a tcp connection.
  After that it expects a line of 5 floats seperated by a space. (x y button1 button2 pause)
  (you can play it per telnet)
  edit config/net.xml to change port and address to listen on.
   

Mod, change, improve it! (Nearly every part of this software needs some love)
