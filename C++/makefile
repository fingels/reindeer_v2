SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	rm -rf ../tmp
	g++ -o $@ $^ -O3
	/usr/bin/time ./main 21 6 100

main.o: main.cpp
	g++ -o main.o -c main.cpp -O3

%.o: %.cpp %.hpp
	g++ -o $@ -c $< -Wall -O3

clean:
	rm -f *.o main

benchmark:
# exemple, il faudra faire pareil avec Python
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 10
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 100
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 1000