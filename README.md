# Embedded Space Invaders #

This implementation of Space Invaders is made for the Digilent [ATLYS Development Board](http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,400,836&Prod=ATLYS&CFID=6868855&CFTOKEN=8a46aff22a6e7e36-78556F94-5056-0201-024A395AE56BA1E9). The hardware and software is organized into their own respective folders.

### Where can I find more details? ###
Implementation and game details can be found here: [Embedded Space Invaders Guide](doc/EmbeddedSpaceInvadersGuide.pdf).

In addition, we built our own Programmable Interval Timer (PIT) Timer to allow us to change the speed of the game using software. The documentation on the PIT Timer can be found here: [PIT Timer Manual](doc/PITTimerManual.pdf).

We also included an IP Module to interface with a HC-SR04 Ultrasonic Rangefinder. Documentation on this IP Module can be found here: [Rangefinder IP Manual](doc/RangefinderIPManual.pdf).

### What is this repository for? ###

This repository is for source files only.
You will need the board support package for this board that is provided on the Digilent Website to compile.
