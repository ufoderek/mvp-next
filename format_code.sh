#!/bin/bash

astyle --quiet --options=astyle.options `find -type f -name '*.c'` `find -type f -name '*.cpp'` `find -type f -name '*.h'`

rm -f `find . | grep .orig`

