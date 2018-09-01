##
##
##
EXES = producer  consumer
SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC) )
RM :=rm -f 

CXXFLAGS = -g -Wall -rdynamic -ldl
CPPFLAGS = -I./include/librabbitmq-c
LIBS = -L./lib -lrabbitmq -lssl -lcrypto -lrt

all: $(OBJ) $(EXES)

show:
	@echo "EXES=$(EXES)"
	@echo "SRC=$(SRC)"
	@echo "OBJ=$(OBJ)"

producer: src/RabbitmqCpp.o src/example-producer.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

consumer: src/RabbitmqCpp.o src/example-consumer.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	$(RM) $(EXES) src/*.o


