.PHONY:all
all:client server
client:client.o common.o
	g++ -o $@ $^ -ljson
common.o:common.cpp 
	g++ -c $^
server:server.o common.o
	g++ -o $@ $^ -lpthread -ljson

.PHONY:clean
clean:
	rm -rf client server *.o
