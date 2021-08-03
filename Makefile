CC = g++

all:
	@echo "Going to compile the source code ..."
	${CC} -o main.exe *.cpp 

clean:
	@echo "Cleaning up ..."
	rm *.exe

