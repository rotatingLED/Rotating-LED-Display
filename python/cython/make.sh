#!/usr/bin/env sh

cython fastpwm.pyx || exit $?

# Compile the object file
gcc -c -O2 -fPIC -Wall -I/usr/include/python2.7/ fastpwm.c || exit $?

# Link it into a shared library
gcc -shared fastpwm.o -o fastpwm.so || exit $?

# Compile the object file
#gcc -c -O2 -fPIC fastpwmc.c || exit $?

# Link it into a shared library
#gcc -shared fastpwmc.o -o fastpwmc.so || exit $?
