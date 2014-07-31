
Community Judge Client
======================

License: GPLv3 Or Later
Project Site: http://www.shervin.org
Contact: communityjudge@shervin.org Or shervinkh145@gmail.com

Compile Notes
=============

I've tried my best to make this source code compilable with both qt4 and qt5.
For compiling this you probably need to have qt and its development package on
some distros (qt-dev or something) either qt4 or qt5.
OpenSSL maybe needed as well because program uses Qt SSL Module.

First run "qmake" to make "Makefile" for building project.
Then run "make" to compile the whole project.

Compilable with gcc>=4.6 or any standard compiler with c++0x (c++11) standard support.

Usage Notes
===========

Program saves its server list in "cjc_servers.dat" in its running directory.
Program loads SSL Certificates from "certs" folder in its running directory.
