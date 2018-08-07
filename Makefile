#! /usr/bin/make -f

clean: 
	rm -rf *.o

client.o: client.cc
	(g++ -c $< -o $@ )

test_client.o: test_client.cc
	(g++ -c $< -o $@ )
test_client: test_client.o client.o
	 g++ -o $@  $^
