#!/bin/bash

bc --version &> /dev/null
bcInstalled=$?
valgrind --version &> /dev/null
vgInstalled=$?
if [ $bcInstalled -eq 127 ] || [ $vgInstalled -eq 127 ]; then
    printf "In order to run the checker, please install bc and valgrind\n"
    printf "\tsudo apt install bc\n"
    printf "\tsudo apt install valgrind\n"
    exit
fi

DEBUG=0 # change this to 1 to preserve output files
TESTS_NUMBER=20
TASKS_NUMBER=8
TOTAL_POINTS=40
STUDENT_SCORE=0.00
POINTS_PER_TEST=$(bc <<< "scale=2; $TOTAL_POINTS/$TESTS_NUMBER")
POINTS_PER_TASK=$(bc <<< "scale=2; $POINTS_PER_TEST/$TASKS_NUMBER")
README_POINTS=5
EXE=homework4

printf "_________________________________________________________\n"
printf "|          Introduction to Computer Programming         |\n"
printf "|                       Homework 4                      |\n"
printf "|_______________________________________________________|\n"

# Build solution
printf "|                   Building solution                   |\n"
make &> out.make
cnt=$(cat out.make| grep Error | wc -l)
if [ $cnt -gt 0 ]; then
    printf "|!!!!!!!!!!!!!!!!!!!Compilation errors!!!!!!!!!!!!!!!!!!|\n"
    printf "|_______________________________________________________|\n"
    cat out.make
    rm -f out.make
    exit
else
    printf "|-----------------Compilation successful----------------|\n"
    printf "|_______________________________________________________|\n"
fi
rm -f out.make

# Prepare environment
rm -rf out
mkdir out

# Run tests
printf "|.....................Running tests.....................|\n"
for (( i=0; i < TESTS_NUMBER; i++ )); do
    printf "|_______________________ Test $i _______________________"
    if [ $i -lt 10 ]; then
        printf "_"
    fi
    printf "|\n"

    cp in/test_$i.* .
    valgrind --leak-check=full --error-exitcode=1 ./$EXE test_$i > test_$i.err 2> /dev/null
    valgrindErrors=$?
    mv test_$i.*.out* out
    errorCode=$(head -n1 test_$i.err)

    rm test_$i.*

    if [ "$errorCode" -eq -1 ]; then
        printf "| Please rerun your submission, a checker error occured |\n"
        printf "|_______________________________________________________|\n"
        rm test_$i.*
        make clean &> /dev/null
        if [ DEBUG -eq 0 ]; then
            rm -rf out
        fi
        exit
    fi

    printf "|                        Part 1                         |\n"
    printf "| Task 1 - "
    head -n1 out/test_$i.txt.out > tsk1.out
    head -n1 ref/test_$i.txt.ref > tsk1.ref
    diff -q -Z tsk1.out tsk1.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk1*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk1*

    printf "| Task 2 - "
    t2size=$(head -n3 in/test_$i.txt.in | tail -n1)
    head -n$(($t2size + 2)) out/test_$i.txt.out | tail -n$t2size > tsk2.out
    head -n$(($t2size + 2)) ref/test_$i.txt.ref | tail -n$t2size > tsk2.ref
    diff -q -Z tsk2.out tsk2.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk2*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk2*

    printf "| Task 3 - "
    tail -n1 out/test_$i.txt.out > tsk3.out
    tail -n1 ref/test_$i.txt.ref > tsk3.ref
    diff -q -Z tsk3.out tsk3.ref > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        rm -f tsk3*
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
    rm -f tsk3*

    printf "|                        Part 2                         |\n"
    printf "| Task 4 - "
    if [ "$errorCode" -eq -2 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi

    printf "| Task 5 - "
    diff -q out/test_$i.bin.out.5 ref/test_$i.bin.ref.5 > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi

    printf "| Task 6 - "
    diff -q -Z out/test_$i.bin.out.6 ref/test_$i.bin.ref.6 > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi

    printf "| Task 7 - "
    diff -q -Z out/test_$i.bin.out.7 ref/test_$i.bin.ref.7 > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi

    printf "| Task 8 - "
    if [ $valgrindErrors -ne 0 ]; then
        printf "FAILED                                       |\n"
        continue
    else
        printf "PASSED                                       |\n"
        STUDENT_SCORE=$(echo "$STUDENT_SCORE+$POINTS_PER_TASK" | bc)
    fi
done

printf "|_______________________________________________________|\n"

README=README
if (( $(echo "$STUDENT_SCORE == 0" |bc -l)  )); then
    printf "| No $README points, tests points are $STUDENT_SCORE!              |"
elif [ ! -f $README ]; then
    printf "| Missing $README!                                       |"
elif [ -f $README ] && [ "`ls -l $README | awk '{print $5}'`" == "0" ]; then
    printf "| Empty $README!                                         |"
else
    printf "| $README - PASSED                                       |"
    STUDENT_SCORE=$(echo "$STUDENT_SCORE+$README_POINTS" | bc)
fi
printf "\n"

printf "| 5 points for coding style will be manually given      |\n"


if (( $(echo "$STUDENT_SCORE < 10.00" |bc -l)  )); then
        printf "|_____________________ TOTAL: $STUDENT_SCORE/50 ____________________|\n"
else
        printf "|____________________ TOTAL: $STUDENT_SCORE/50 __________________|\n"
fi
printf "|_______________________________________________________|\n"

make clean &> /dev/null
if [ $DEBUG -eq 0 ]; then
    rm -rf out
fi
