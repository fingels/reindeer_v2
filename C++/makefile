SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
# LIB_PROFILE=$(find /usr/lib -name libprofiler.so)
# CPUPROFILE=results.cpu LD_PRELOAD="$LIB_PROFILE" ./main

main: $(OBJ)
	rm -rf ../tmp
	g++ -o $@ $^ -O3 -g
	./main 21 6 100

main.o: main.cpp
	g++ -o main.o -c main.cpp -O3 -g

%.o: %.cpp %.hpp
	g++ -o $@ -c $< -Wall -O3 -g

clean:
	rm -f *.o main

# google:
# 	rm -rf ../tmp
# 	google-pprof --lines --pdf ./main 21 6 100 results. > ../results-cpu.pdf  
 
 benchmark:
# exemple, il faudra faire pareil avec Python
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 10
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 100
	rm -rf ../tmp
	/usr/bin/time ./main 21 6 1000