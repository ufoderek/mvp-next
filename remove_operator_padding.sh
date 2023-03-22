#!/bin/bash

uncrustify -c uncr.conf --no-backup `find -type f -name '*.c'` `find -type f -name '*.cpp'` `find -type f -name '*.h'`

