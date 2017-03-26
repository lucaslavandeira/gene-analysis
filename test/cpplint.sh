#!/usr/bin/env bash
cpplint --extensions=h,hpp,c,cpp --filter=`cat ../test/filter_options` `find -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`
