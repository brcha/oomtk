#!/bin/sh

echo -n "Generating ChangeLog ... "
bzr log --gnu > ChangeLog
echo "done."

