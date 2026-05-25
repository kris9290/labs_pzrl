.PHONY: all clean run

TEST=rbt
LIBTEST=RBTreeTest

all: $(TEST)

$(TEST): RBT.cpp
		g++ RBT.cpp -L. -l$(LIBTEST) -o $(TEST) -std=c++11

run: all
		./$(TEST)

clean:
		rm -rf *.o $(TEST)
