##
##
##
EXES = boost_log_test
SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC) )
RM :=rm -f 

CXXFLAGS = -g -Wall -rdynamic -ldl
CPPFLAGS = -I/home/tangyibo/local/boost/include
LIBS = -L/home/tangyibo/local/boost/lib -lboost_log_setup -lboost_log -lboost_system -lboost_filesystem -lboost_thread

all: $(OBJ) $(EXES)

show:
	@echo "EXES=$(EXES)"
	@echo "SRC=$(SRC)"
	@echo "OBJ=$(OBJ)"

$(EXES): $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	$(RM) $(EXES) src/*.o


