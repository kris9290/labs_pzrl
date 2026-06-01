.PHONY: all clean run

TEST=test
LIBTEST=HashTest

all: $(TEST)

$(TEST): HashTable.cpp
	g++ HashTable.cpp -L. -l$(LIBTEST) -o $(TEST) -std=c++11

run: all
	./$(TEST)

clean:
	rm -rf *.o $(TEST)
