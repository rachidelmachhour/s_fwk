test_read: s_serial.o test_read.o
	gcc   s_serial.o test_read.o  -g -o test_read  -lpthread  

s_serial.o: s_serial.c
	gcc s_serial.c -c -Wall  -I/. -lpthread  -std=gnu99

test_read.o: test_read.c
	 gcc test_read.c -c -Wall  -I/. -lpthread  -std=gnu99


clean:
	rm -f *.o
all: clean test