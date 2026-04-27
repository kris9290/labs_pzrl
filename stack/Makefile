.PHONY: clean
#Объектные файлы всех модулей проекта
OBJS = Vector.o List.o VectorStackImpl.o ListStackImpl.o Stack.o

#Итоговый исполняемый файл
TARGET = stack_test

#Тестовая библиотека
TEST_LIB = libStackTest.a


all: $(TARGET)

#Линковка: все объектные файлы + тестовая библиотека
$(TARGET): $(OBJS)
	g++ $^ -o $@ -L. -l:$(TEST_LIB)

#Правила для каждого объектного файла
Vector.o: Vector.cpp Vector.h
	 g++ -c Vector.cpp -o Vector.o

List.o: List.cpp List.h
	 g++ -c List.cpp -o List.o

VectorStackImpl.o: VectorStackImpl.cpp VectorStackImpl.h Vector.h IStackImplementation.h
	 g++ -c VectorStackImpl.cpp -o VectorStackImpl.o

ListStackImpl.o: ListStackImpl.cpp ListStackImpl.h List.h IStackImplementation.h
	 g++ -c ListStackImpl.cpp -o ListStackImpl.o

Stack.o: Stack.cpp Stack.h IStackImplementation.h VectorStackImpl.h ListStackImpl.h
	 g++ -c Stack.cpp -o Stack.o

clean:
	 rm -f $(OBJS) $(TARGET)

