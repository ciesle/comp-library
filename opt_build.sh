#!/bin/bash
out_path="./.vscode/auto_build"

if [[ $1 == *.cpp ]]; then
    name=${1%%.cpp}
else
    name="main"
fi
if [[ $name == "main" && $# -ne 1 ]]; then
    make clean_main
fi
echo -e "\e[35;1mbuild ${name}\e[m"
make ${name}
if [ $? -ne 0 ]; then
    printf "build \e[31;1mfailed\e[m\n"
    exit 1
fi
printf "build \e[32;1mcompleted!\e[m\n"