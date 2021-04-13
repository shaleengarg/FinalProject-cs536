CXX=g++
CC=gcc

LIBS=-lpthread -lrt -ldl
FLAGS=-fPIC -shared -std=gnu++11 

all: seq 

seq: frontend.cpp handler.cpp
	$(CXX) $(FLAGS) -g -o liblog.so $^ $(LIBS)


install:
	sudo cp *.so /usr/local/lib
	sudo cp *.so /usr/lib

clean:
	rm -rf *.o
	rm *.so

uninstall:
	sudo rm /usr/local/lib/liblog.so
	sudo rm /usr/lib/liblog.so
