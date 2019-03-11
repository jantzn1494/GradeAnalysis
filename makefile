main: main.o gradeutil.o
	g++ -std=c++11 -o main -g main.o gradeutil.o
	./main

gradeutil.o: gradeutil.cpp gradeutil.h
	g++ -std=c++11 -c -g gradeutil.cpp
	
main.o: main.cpp gradeutil.h
	g++ -std=c++11 -c -g main.cpp
	
clean:
	rm main.o
	rm gradeutil.o
	rm main