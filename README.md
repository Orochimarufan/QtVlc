QtVlc
=====

a Qt library around libvlc

Qt5
---

QtVlc is Qt5 ONLY.

Note that any Qt5 app using libvlc might crash on startup if the libvlc plugin cache is outdated and it tries to enumerate the plugins.
This is caused by libvlc's attempt to load the Qt4 interface plugin which in turn loads Qt4 that conflicts with Qt5.

License
-------

QtVlc is mostly LGPL, but parts of QtVlcWidgets are GPL due to code taken from the VLC Qt4 interface plugin sources that are licensed under GPL

see the individual source files.

