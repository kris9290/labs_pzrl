#include "circle.h"
#include <iostream>

int main() {
// Создаем несколько отрезков
	LineSegment segment1 = {{0, 0}, {4, 0}};
	LineSegment segment2 = {{2, 0}, {2, 3}};
// Создаем вектор отрезков
        std::vector<LineSegment> segments = {segment1, segment2};
// Находим минимальную окружность
	Circle result = MinimumEnclosingCircleForSegments(segments);
 // Выводим результат
	std::cout << "Центр: (" << result.center.x << ", " << result.center.y << ")" << std::endl;
	std::cout << "Радиус: " << result.radius << std::endl;
	 return 0;
}
