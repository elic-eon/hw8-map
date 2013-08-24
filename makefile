all: map.func.demo map.bonus.func.demo

map.bonus.func.demo: map.o map.bonus.func.tester utility.o
	gcc -o map.bonus.func.demo map.o map.bonus.func.tester utility.o

map.func.demo: map.o map.func.tester utility.o 
	gcc -o map.func.demo map.o map.func.tester utility.o

map.perf.demo: map.o map.perf.tester 
	gcc -o map.perf.demo map.o map.perf.tester

map.o: map.c
	gcc -c -o map.o map.c

map.bonus.func.tester: map.bonus.func.tester.c
	gcc -c -o map.bonus.func.tester map.bonus.func.tester.c

map.func.tester: map.func.tester.c
	gcc -c -o map.func.tester map.func.tester.c

utility.o: utility.c
	gcc -c -o utility.o utility.c

map.perf.tester: map.perf.tester.c
	gcc -c -o map.perf.tester map.perf.tester.c

clean:
	rm -f *.demo *.tester *.o
