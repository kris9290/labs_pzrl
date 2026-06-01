#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>

//Структура, представляющая точку на плоскости.
struct Point2D {
    double x, y;

    //Оператор сравнения на равенство двух точек.
    bool operator==(const Point2D& other) const;

    //Оператор сравнения на неравенство двух точек.
    bool operator!=(const Point2D& other) const {
        return !(*this == other);
    }
};

//Структура, представляющая отрезок на плоскости.
struct LineSegment {
    Point2D start; ///< Начальная точка отрезка.
    Point2D end;   ///< Конечная точка отрезка.
};

//Структура, представляющая окружность.
struct Circle {
    Point2D center; ///< Центр окружности.
    double radius;  ///< Радиус окружности.

    //Проверяет, является ли окружность допустимой.
    bool isValid() const {
        return radius >= 0;
    }

    //Проверяет, содержится ли точка внутри окружности.
    bool contains(const Point2D& point) const;

    //Проверяет, содержится ли отрезок полностью внутри окружности.
    bool contains(const LineSegment& segment) const;

    //Проверяет, содержится ли весь отрезок внутри окружности, включая его середину.
    bool containsEntireSegment(const LineSegment& segment) const;
};

//Находит минимальную окружность, содержащую все заданные отрезки.
Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments);

// Вспомогательные функции для реализации
// Вычисляет квадрат расстояния между двумя точками.
double distanceSquared(const Point2D& p1, const Point2D& p2);

//Вычисляет расстояние между двумя точками
double distance(const Point2D& p1, const Point2D& p2);

//Создает окружность по двум точкам (используя диаметр).
Circle circleFromDiameter(const Point2D& p1, const Point2D& p2);

//Создает окружность, проходящую через три точки.
Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3);

//Рекурсивный алгоритм Вельцля для нахождения минимальной окружности.
Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n);

//Нахождение минимальной окружности для множества точек.
Circle minCircle(std::vector<Point2D> points);

//Извлекает все точки из отрезков (включая середины).
std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments);

//Находит середину отрезка.
Point2D findMidPoint(const LineSegment& segment);
/*{
	Point2D mid;
	mid.x = (segment.start.x + segment.end.x) / 2.0;
	mid.y = (segment.start.y + segment.end.y) / 2.0;
	return mid;
}
*/
