CXX=g++
OUTPUT=lab05
CXXFLAGS=-Wall -Isrc -g -std=c++17

all:$(OUTPUT)
src/scanner.o: src/scanner.cpp src/scanner.hpp src/tree.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
src/tree.o: src/tree.cpp src/tree.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
src/calculator.o: src/calculator.cpp src/calculator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
tests/test1.o: tests/test1.cpp src/scanner.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
lab05: src/tree.o tests/test1.o src/scanner.o src/calculator.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lgtest -lpthread
test: lab05
	./lab05
check:
	valgrind ./lab05
clean:
	rm -f src/*.o lab05 tests/*.o
