all: twelvedaystest maintest

twelvedaystest: twelvedays.c myprintf.c printinteger.s
	gcc twelvedays.c myprintf.c printinteger.s -std=c99 -g -o twelvedaystest

maintest: main.c myprintf.c printinteger.s
	gcc main.c myprintf.c printinteger.s -std=c99 -g -o maintest

clean:
	rm -f maintest twelvedaystest

