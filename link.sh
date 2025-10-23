#!/usr/bin/bash

if [ $# -ne 1 ];then
	echo "Usage: $0 dir_name"
	exit 1
fi

name=$(cd $(dirname $0) && pwd)
name=${name##*/}
$(cd ../$1 && rm -f ./Makefile && ln -s ../$name/Makefile)
mkdir -p ../$1/.vscode/
mkdir -p ../$1/.vscode/auto_build

$(cd ../$1 && rm -rf ./library && ln -s ../$name/library/ library)
rm -f ../$1/library/comp

$(cd ../$1/ && rm -rf snippet && ln -s ../../$name/snippet snippet)

files=("gdb_exit.sh" "expand_include.py" "launch.json" "randcheck.sh" "c_cpp_properties.json" "dbg_build.sh" "opt_build.sh" "settings.json" "tasks.json")
for file in ${files[@]};do
	$(cd ../$1/.vscode && rm -f ./$file && ln -s ../../$name/$file)
done

touch ../$1/main.cpp ../$1/gen.cpp ../$1/brute.cpp ../$1/in.txt
g++ -O2 -o $(cd `dirname $0` && pwd)/snip $(cd `dirname $0` && pwd)/snippet.cpp
$(cd ../$1 && make snippet)
