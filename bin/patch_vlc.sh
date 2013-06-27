#!/bin/sh

#sed 's?--git-dir="\$\(top_srcdir\)/\.git/"?--git-dir="$(top_srcdir)/.git"?g' src/Makefile.am -i.orig

sed -e 's:\/\.git\/:/.git:g' src/Makefile.am -i.orig
