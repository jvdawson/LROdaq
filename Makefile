#! /usr/bin/make -f

clean: 
	rm -rf *.o

client.o: client.cc
	(g++ -c $< -o $@ )

test_client.o: test_client.cc
	(g++ -c $< -o $@ )
test_client: test_client.o client.o
	 g++ -o $@  $^
card.o: card.cc
	(g++ -c $< -o $@ )

test_card.o: test_card.cc
	(g++ -c $< -o $@ )
test_card: test_card.o card.o client.o
	 g++ -o $@  $^
lrodaq.o: lrodaq.cc
	(g++ -c $< -o $@ )
lrodaq: lrodaq.o card.o client.o
	 g++ -o $@  $^
