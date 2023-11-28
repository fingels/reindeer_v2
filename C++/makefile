SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	g++ -o $@ $^ -O3 -g
	
main.o: main.cpp
	g++ -o main.o -c main.cpp -O3 -g

%.o: %.cpp %.hpp
	g++ -o $@ -c $< -Wall -O3 -g

clean:
	rm -f *.o main

run:
	rm -rf ../tmp
	./main -f ../unitigs -k 21 -m 6 -c 100
 
 benchmark:
# exemple, il faudra faire pareil avec Python
	rm -rf ../tmp
	/usr/bin/time ./main -f ../unitigs -k 21 -m 6 -c 10
	rm -rf ../tmp
	/usr/bin/time ./main -f ../unitigs -k 21 -m 6 -c 100
	rm -rf ../tmp
	/usr/bin/time ./main -f ../unitigs -k 21 -m 6 -c 1000