

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	rm -rf tmp
	g++ -o $@ $^ -O3
	./main

main.o: main.cpp
	g++ -o main.o -c main.cpp -O3

%.o: %.cpp %.hpp
	g++ -o $@ -c $< -Wall -O3

clean:
	rm -f *.o main

