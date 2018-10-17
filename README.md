# Programming Assignment 2 - Cisc361
Calvin Banning
John Maloy

Test Cases:

pwd
    ls &
    ls -l &
    cd /
    sleep 20 &
    ls & 			; run before sleep is done
    pid
    tty
    /bin/ps -lfu USERNAME	; replace USERNAME with your own  
    cd
    cd [project test dir of your choosing]
    pwd
    ls -l
    rm -f mail1 mail2
    touch mail1			; create this file
    watchmail mail1
    echo hi >> mail1
    echo HiThere > mail2	; create another file
    watchmail mail2
    echo there >> mail1
    echo Subject: >> mail2
    cat mail1
    cat mail2
    watchmail mail1 off
    echo bye >> mail1
    echo bye >> mail2		; still watching this one

    rm -f test1 test2 test3 test4 test5 test6 test7 test8
    test-1+2 > test1
    test-1+2 >> test2
    test-1+2 >& test3
    test-1+2 >>& test4
    cat test1 test2 test3 test4
    test-1+2 > test1
    test-1+2 >> test2
    test-1+2 >& test3
    test-1+2 >>& test4
    cat test1 test2 test3 test4

    noclobber				; turn noclobber on
    test-1+2 > test5
    test-1+2 >> test6
    test-1+2 >& test7
    test-1+2 >>& test8
    cat test5 test6 test7 test8
    test-1+2 > test5
    test-1+2 >> test6
    test-1+2 >& test7
    test-1+2 >>& test8
    cat test5 test6 test7 test8

    grep hello < test8
    grep error < test8

    rm -f test9 test10 test11 test12
    noclobber				; turn noclobber off
    test-1+2 > test9
    test-1+2 >> test10
    test-1+2 >& test11
    test-1+2 >>& test12
    cat test9 test10 test11 test12

    ls | fgrep .c                   ; show pipes works
    ./test-1+2 | grep  hello
    ./test-1+2 |& grep hello
    ./test-1+2 | grep output
    ./test-1+2 |& grep output
    ./test-1+2 |& grep error

    pid                                        ; zombie avoidance checking
    /bin/ps -lfu USERNAME | grep defunct       ; replace USERNAME with your username
