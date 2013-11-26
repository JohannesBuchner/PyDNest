CFLAGS = -fPIC -O2 -Wall -Wextra -ansi -pedantic -I ../../include/ 
LIBS =  -ldnest3s -lgsl -lgslcblas -lboost_system -lboost_thread -L ../../lib/

all: libdnestbridge.so
%.o: %.cpp
	g++ $(CFLAGS) -c $? -o $@
libdnestbridge.so: MyModel.o main.o
	g++ -shared -Wl,-soname,libdnestbridge.so -o $@ $? $(LIBS)

clean:
	rm -f *.o libdnestbridge.so
