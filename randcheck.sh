#!/bin/bash
out_path="./.vscode/auto_build"
gen_path="gen.cpp"
main_path="main.cpp"
brute_path="brute.cpp"
input_path="rnd_in.txt"
output_main_path="rnd_outm.txt"
output_brute_path="rnd_outb.txt"

echo -e "\e[35;1mrandom check\e[m"

function clean_up(){
    if [ $? -eq 0 ]; then
        printf "random check \e[32;1mcompleted!!\e[m\n" 1>&2
        rm $input_path $output_brute_path $output_main_path
        if [ -e ./${gen_path%%.cpp} ]; then rm ${gen_path%%.cpp}; fi
        if [ -e ./${brute_path%%.cpp} ]; then rm ${brute_path%%.cpp}; fi
    else
        cp $out_path/${gen_path%%.cpp} ./${gen_path%%.cpp}
        cp $out_path/${brute_path%%.cpp} ./${brute_path%%.cpp}
    fi
}

function fail_output(){
    if [ $1 -ne 0 ] ; then
        printf "$2 \e[31;1mfailed\e[m\n" 1>&2
        exit 1
    fi
}

function re_build(){
    local files=($main_path $gen_path $brute_path)
    for i in "${!files[@]}"; do
        make ${out_path}/${files[$i]%%.cpp}
        if [ $? -ne 0 ]; then exit 1; fi
    done
}

trap clean_up EXIT

re_build

#optionのパース
options=""
num=100
option_list=($(echo "$*"))
for ((i=0; i<${#option_list[@]}; i++)) ; do
    case ${option_list[$i]} in
        "-n")
            if [[ $((i+1)) -ge ${#option_list[@]} ]] || [[ ${option_list[$((i+1))]} =~ ^-+ ]]; then
                echo -e "option \e[31;1merror\e[m" 1>&2
                exit 1
            else
                num=${option_list[$((i+1))]}
            fi
            ((i=i+1))
            ;;
        *)
            if [[ -z options ]]; then
                options=${option_list[$i]}
            else
                options=$options" "${option_list[$i]}
            fi;;
        esac
done

for ((i = 1 ; i <= $num ; i++)) ; do
    ${out_path}/${gen_path%%.cpp} $options > "$input_path"
    fail_output $? "generate"
    ${out_path}/${main_path%%.cpp} < "$input_path" > "$output_main_path"
    fail_output $? "${main_path%%.cpp} run"
    ${out_path}/${brute_path%%.cpp} < "$input_path" > "$output_brute_path"
    fail_output $? "${brute_path%%.cpp} run"
    if [ "$(cat $output_main_path)" != "$(cat $output_brute_path)" ] ; then
        printf "check \e[31;1mfailed\e[m\n" 1>&2
        printf "in\n%s\n${main_path%%.cpp}\n%s\n${brute_path%%.cpp}\n%s\n" "`cat $input_path`" "`cat $output_main_path`" "`cat $output_brute_path`"
        exit 1
    fi
    if [ $(($i % (num/10))) -eq 0 ] ; then
        echo "$i case finished!" 1>&2
    fi
done
