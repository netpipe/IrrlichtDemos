Linux Makefile for irrBullet (modified version of PIM)

0 Contents:

1 About this Makefile
2 Multiple OS/CC Support
3 Use
4 Legal Notice

1 About this Makefile

This is a Makefile for irrBullet, written by Johannes Lorenz for Josiah Hartzell
(the author of irrBullet). The Makefile's sources are modified PIM Sources.
You can contact Johannes and download PIM on http://odf.sourceforge.net.

2 Multiple OS/CC Support

This Makefile shall be kept cross platform, so you can use it on multiple
systems with multiple compilers. The default is Linux and g++. If it does not
compile, in order to make it fit for your platform, change the 'SYS_MAKEFILE'
variable in the Makefile and copy the Makefile.lin to Makefile.<yourOS>.

Please report to Johannes or Josiah if you had to make any changes!

3 Use

For a short introduction, the most common commands are:

$ make		# compiles irrBullet
$ make install	# installs irrBullet, so it can be shared by other programs

For a better documentation, please type

$ make help

4 Legal Notice

This Makefile is distributed under the terms of GPL3. Please note that this
Makefile comes without any warranty!
