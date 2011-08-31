PROGRAM = ../ch
OBJ = ../ch.o
TEST_PROGRAM = ch-test
TEST_OBJ = ../ch.o test.o

%.o : %.cpp
	g++ -g3 -W -Wall -c $< -o $@

build_test: $(TEST_PROGRAM)

$(TEST_PROGRAM): $(TEST_OBJ)
	g++ $(TEST_OBJ) -o $(TEST_PROGRAM)

../ch.o: ../ch.h
