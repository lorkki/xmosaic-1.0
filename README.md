# xmosaic-1.0

Inspired by the recent [repost at
HN](https://news.ycombinator.com/item?id=16928716), this repository contains the
original source code to xmosaic-1.0, with the minimal changes I made to have it
build and run on Ubuntu 17.10.

## Building

Prerequisite packages:

* build-essential
* libmotif-dev
* libxmu-dev

The makefiles are preconfigured, so proceed as follows, as per the original
README:

    $ cd libwww; make; cd ..
    $ cd libhtmlw; make; cd ..
    $ cd src; make

To run, execute `./xmosaic`.