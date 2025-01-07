#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BASE=`tput setaf 7`

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )

flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "s"
)

tests=(
"s test_files/test_0_grep.txt FLAGS"
"for s21_grep.c s21_grep.h Makefile FLAGS"
"for s21_grep.c FLAGS"
"-e for -e ^int s21_grep.c s21_grep.h Makefile FLAGS"
"-e for -e ^int s21_grep.c FLAGS"
"FLAGS no_file.txt"
"FLAGS -f no_pattern.txt no_file.txt"
"-e regex -e ^print s21_grep.c FLAGS -f test_files/test_ptrn_grep.txt"
"-e while -e void s21_grep.c Makefile FLAGS -f test_files/test_ptrn_grep.txt"
"-e intel -e int FLAGS test_files/test_7_grep.txt"
"-e int -e intel FLAGS test_files/test_7_grep.txt"
)

manual=(
"-n for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-n for test_files/test_1_grep.txt"
"-n -e ^\} test_files/test_1_grep.txt"
"-ce ^int test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e ^int test_files/test_1_grep.txt"
"-nivh = test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-ie INT test_files/test_5_grep.txt"
"-echar test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-ne = -e out test_files/test_5_grep.txt"
"-iv int test_files/test_5_grep.txt"
"-in int test_files/test_5_grep.txt"
"-c -l aboba test_files/test_1_grep.txt test_files/test_5_grep.txt"
"-v test_files/test_1_grep.txt -e ank"
"-noe ) test_files/test_5_grep.txt"
"-l for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-o -e int test_files/test_4_grep.txt"
"-e = -e out test_files/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_files/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_files/test_6_grep.txt"
"-c -e . test_files/test_1_grep.txt -e '.'"
"-l for no_file.txt test_files/test_2_grep.txt"
"-e int -si no_file.txt s21_grep.c no_file2.txt s21_grep.h"
"-si s21_grep.c -f no_pattern.txt"
"-f test_files/test_3_grep.txt test_files/test_5_grep.txt"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS++"
        echo "${RED}$COUNTER ${BASE}- ${GREEN}Тест пройден ${BASE}$param"
    else
        let "FAIL++"
        echo "${RED}$COUNTER ${BASE}- ${RED}Тест не пройден ${BASE}$param"
        exit 1
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

printf "\n"
echo "#######################"
echo "Тест на исключительные случаи."
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "#######################"
echo "Тест на вариативность флагов."
echo "#######################"
printf "\n"
echo "======================="
echo "1 флаг"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done

printf "\n"
echo "======================="
echo "2 флага"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done

printf "\n"
echo "======================="
echo "Комбинация из двух флагов."
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

printf "\n"
echo "${GREEN}ТЕСТИРОВАНИЕ ЗАВЕРШЕННО УСПЕШНО"
printf "\n"
