PROGRAM = ch
OBJ = ch.o
TEST_PROGRAM = ch-test
TEST_OBJ = ch.o test.o


build: $(PROGRAM)

$(PROGRAM): $(OBJ)
	g++ $(OBJ) -o $(PROGRAM)

%.o : %.cpp
	g++ -W -Wall -O2 -static -c $< -o $@

build_test: $(TEST_PROGRAM)

$(TEST_PROGRAM): $(TEST_OBJ)
	g++ $(TEST_OBJ) -o $(TEST_PROGRAM)

ch.o: ch.h

