#!/bin/sh

gcc -fPIC -shared plugin.c -I/usr/include/python2.7 -lpython2.7 -o plugin.so

gcc linktest.c -lpython2.7 -ldl -o linktest-with-python
gcc linktest.c             -ldl -o linktest-no-python
