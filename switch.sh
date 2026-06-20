#!/usr/bin/env bash
cp main.cpp tmp
if [ -e $1.cpp ]; then
	cp $1.cpp main.cpp
fi
mv tmp $1.cpp