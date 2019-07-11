echo creating new unittest class file for $1
test -e unittests/ut$1.h && mv unittests/ut$1.h unittests/ut$1.h.bak
cat scripts/templates/utDUMMY.h | sed "s/DUMMY/$1/g" > unittests/ut$1.h
ls unittests/ut$1.h