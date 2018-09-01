##
##
##
EXES = bin/test_zkclient
SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC) )
RM :=rm -f 

CXXFLAGS = -g -Wall -rdynamic -ldl
CPPFLAGS = -I./include 
LIBS = -L./lib -lzookeeper_mt -lpthread

all: $(OBJ) $(EXES)

show:
	@echo "EXES=$(EXES)"
	@echo "SRC=$(SRC)"
	@echo "OBJ=$(OBJ)"

$(EXES): $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	$(RM) $(EXES) src/*.o


