# Programming Assignment 2 - Cisc361
Calvin Banning
John Maloy

Test Cases:

[return]
Ctrl-D
Ctrl-Z
Cotrl-C
which					; test which
which ls
ls					; execute it
[return]
Ctrl-D					; make sure still work
Ctrl-Z
Ctrl-C
ls -l					; test passing arguments
ls -l -a /proc/tty
ls -l -F /proc/tty
ls -l -F /etc/perl
ls -l -a /etc/perl
where					; test where
where ls
/bin/ls -l -g			; test absolutes and passing args
/bin/ls -l
file *					; test out * wildcard
ls *
ls *.c
ls -l sh.*
ls -l s*.c
ls -l s*h.c
ls sh.?					; test out ? wildcard
ls ?h.c	
ls *.?					; test combination of ? and *	
blah					; try a command that doesn't exist
/blah					; an absolute path that doesn't exist
ls -l /blah
/usr/bin				; try to execute a directory
/bin/ls -la /
file /bin/ls /bin/rm
which ls rm				; test multiple args
where ls rm
list					; test list
list / /usr/proc/bin
cd 					; cd to home
pwd
cd /blah				; non-existant
cd /usr/bin /usr/ucb			; too many args
cd -					; should go back to project dir
pwd
more sh.c   (and give a Crtl-C)		; more should exit
cd /usr/bin
pwd
./ls /					; test more absolutes
../bin/ls /
history					; test history
history 15
history
history 5
history
pid					; get pid for later use
kill
kill pid-of-shell			; test default
kill -1 pid-of-shell			; test sending a signal, should kill
					; the shell, so restart a new one
prompt	    (and enter some prompt prefix)
prompt 361shell
alias changedir cd
changedir ..				; should go up a directory
alias	  				; list all alias
printenv PATH
printenv
setenv
setenv TEST
printenv TEST
setenv TEST testing
printenv TEST
setenv TEST testing more
setenv HOME /
cd
pwd
exit
