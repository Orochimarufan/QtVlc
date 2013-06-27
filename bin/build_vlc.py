#!/usr/bin/python
from __future__ import print_function

import subprocess
import time
import sys
import os

bin_dir = os.path.normpath(sys.argv[1])
src_dir = os.path.normpath(sys.argv[2])
make_cmd = sys.argv[3:]

stamp = os.path.join(bin_dir, "stamp", "build")

if not os.path.exists(stamp) or int(open(stamp).read()) < os.stat(stamp).st_mtime:
    subprocess.check_call(["./compile" if sys.stdout.isatty() else "make"] + make_cmd, cwd=src_dir)

    open(stamp, "w").write(str(int(time.time()) + 10))
else:
    print("Skipping VLC build. Touch '%s' to re-build" % stamp)

