#!/usr/bin/env python3
import subprocess

revision = subprocess.check_output(["git", "rev-parse", "HEAD"]).decode('utf-8').strip()
print('-DPIO_SRC_REV=%s' % revision)