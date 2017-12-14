
test.bin : main.o Object.o
	g++ -o $@ $^

main.o : main.cpp
	g++ -c -g -Wall -o $@ $<

Object.o : Object.cpp
	g++ -c -g -Wall -o $@ $<

all: test.bin

clean:
	-rm ./*.o test.bin	
