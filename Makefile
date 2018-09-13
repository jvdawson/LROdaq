#! /usr/bin/make -f

clean: 
	rm -rf *.o

client.o: client.cc
	(g++ -c $< -o $@ )

server.o: server.cc
	(g++ -c $< -o $@ )

test_client.o: test_client.cc
	(g++ -c $< -o $@ )

test_client: test_client.o client.o
	 g++ -o $@  $^
card.o: card.cc
	(g++ -c $< -o $@ )

datareceiver.o: datareceiver.cc
	(g++ -c -pthread -std=c++11 $< -o $@ )
messagelistener.o: messagelistener.cc
	(g++ -c -pthread -std=c++11 $< -o $@ )

test_server.o: test_server.cc
	(g++ -c $< -o $@ )

test_server: test_server.o server.o
	 g++ -o $@ $^


test_card.o: test_card.cc
	(g++ -c $< -o $@ )
test_card: test_card.o card.o client.o messagelistener.o datareceiver.o
	 g++ -lpthread -o $@  $^


lrodaq.o: lrodaq.cc
	(g++ -c $< -o $@ )
lrodaq: lrodaq.o  card.o client.o messagelistener.o datareceiver.o
	 g++ -lpthread -o $@  $^
