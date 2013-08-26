all: map.func.demo map.bonus.func.demo

map.bonus.func.demo: map.o map.bonus.func.tester utility.o
	gcc -o map.bonus.func.demo map.o map.bonus.func.tester utility.o -g

map.func.demo: map.o map.func.tester utility.o
	gcc -o map.func.demo map.o map.func.tester utility.o -g

map.perf.demo: map.o map.perf.tester
	gcc -o map.perf.demo map.o map.perf.tester -g

map.o: map.c
	gcc -c -o map.o map.c -g

map.bonus.func.tester: map.bonus.func.tester.c
	gcc -c -o map.bonus.func.tester map.bonus.func.tester.c -g

map.func.tester: map.func.tester.c
	gcc -c -o map.func.tester map.func.tester.c -g

utility.o: utility.c
	gcc -c -o utility.o utility.c -g

map.perf.tester: map.perf.tester.c
	gcc -c -o map.perf.tester map.perf.tester.c -g

clean:
	rm -f *.demo *.tester *.o
