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
stamp_last = int(open(stamp).read())
stamp_time = os.stat(stamp).st_mtime

#print(stamp_last, stamp_time)

if not os.path.exists(stamp) or stamp_last < stamp_time:
    subprocess.check_call(["./compile" if sys.stdout.isatty() else "make"] + make_cmd, cwd=src_dir)

    open(stamp, "w").write(str(int(time.time()) + 10))
else:
    print("Skipping VLC build. Touch '%s' to re-build" % stamp)

