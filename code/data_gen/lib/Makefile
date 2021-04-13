CXX=g++
CC=gcc

LIBS=-lpthread -lrt -ldl
FLAGS=-fPIC -shared -std=gnu++11 

all: seq 

seq: frontend.cpp handler.cpp
	$(CXX) $(FLAGS) -g -o liblog.so $^ $(LIBS)
