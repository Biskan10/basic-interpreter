dir = interpreter
all: hw4

hw4: main.cpp $(dir)/interpreter.o $(dir)/command.o $(dir)/arithmetic.o
	g++ -g -Wall -std=c++11 main.cpp $(dir)/interpreter.o $(dir)/command.o $(dir)/arithmetic.o -o hw4

interpreter.o: $(dir)/interpreter.h $(dir)/interpreter.cpp
	g++ -g -Wall -c -std=c++11 $(dir)/interpreter.cpp -o $(dir)/interpreter.o

command.o: $(dir)/command.h $(dir)/command.cpp
	g++ -g -Wall -c -std=c++11 $(dir)/command.cpp -o $(dir)/command.o

arithmetic.o: $(dir)/arithmetic.h $(dir)/arithmetic.cpp
	g++ -g -Wall -c -std=c++11 $(dir)/arithmetic.cpp -o $(dir)/arithmetic.o

clean:
	rm hw4
	rm $(dir)/*.o	