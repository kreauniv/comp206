
program: q1.c q2.c q3.c utils.c main.c
	gcc -o program -g -fsanitize=address q1.c q2.c q3.c utils.c main.c

run: program
	./program

submission:
	date -Iseconds > timestamp.txt
	chmod a-w timestamp.txt
	cd .. && tar zcvf $(shell whoami)-endterm.tar.gz endterm

sync:
	rsync * comp206:~/endterm/

help:
	less README.txt
