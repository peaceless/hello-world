all:server client
server:
	g++ -std=c++11 Server.cpp -o server
client:
	g++ -std=c++11 Client.cpp -o client
