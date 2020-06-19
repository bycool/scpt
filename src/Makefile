so:
	gcc test.c -I ../incl/ -L ../lib/ -l zlog -o ../bin/main

ao:
	gcc test.c ../lib/libzlog.a -I ../incl/ -L ../lib/ -o ../bin/main -pthread
#	gcc -c test.c -I ../incl/
#	ar rcs ../lib/libzlog.a test.o

clean:
	rm  ./test.o ../bin/main -f
