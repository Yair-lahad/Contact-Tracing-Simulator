# All Targets
all: cTrace

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
cTrace: bin/Graph.o bin/Session.o bin/main.o bin/Agent.o bin/Tree.o
	@echo 'Building target: cTrace'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/cTrace bin/Graph.o bin/Session.o bin/main.o bin/Agent.o bin/Tree.o 

# Depends on the source and header files

bin/Graph.o: src/Graph.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Graph.o src/Graph.cpp
bin/Session.o: src/Session.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp
bin/main.o: src/main.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
bin/Agent.o: src/Agent.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Agent.o src/Agent.cpp
bin/Tree.o: src/Tree.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Tree.o src/Tree.cpp

#Clean the build directory
clean: 
	rm -f bin/*