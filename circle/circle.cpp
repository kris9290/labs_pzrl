#include "circle.h"
#include <random>
#include <algorithm>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>

//Реализация методов Point2D
bool Point2D::operator==(const Point2D& other) const {
    if (x==other.x && y==other.y)
	    return true;
    return false;
}

//Реализация методов Circle
bool Circle::contains(const Point2D& point) const {
    if (distance(center, point) <= radius)
	    return true;
    return false;
}

bool Circle::contains(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end);
}

bool Circle::containsEntireSegment(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end);
}

//Bспомогательные геометрические функции
double distanceSquared(const Point2D& p1, const Point2D& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
    return std::sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2) {
    Point2D center;
    center.x = (p1.x + p2.x) / 2.0;
    center.y = (p1.y + p2.y) / 2.0;
    double rad = distance(p1, p2) / 2.0;
    return {center, rad};
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    // Проверка коллинеарности: если площадь треугольника близка к нулю
    double area2 = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    if (std::fabs(area2) < 1e-12) {
        // Точки на одной прямой: берём две самые удалённые как диаметр
        double d12 = distanceSquared(p1, p2);
        double d23 = distanceSquared(p2, p3);
        double d31 = distanceSquared(p3, p1);
        if (d12 >= d23 && d12 >= d31)
            return circleFromDiameter(p1, p2);
        else if (d23 >= d12 && d23 >= d31)
            return circleFromDiameter(p2, p3);
        else
            return circleFromDiameter(p3, p1);
    }

    // Решение системы линейных уравнений для центра описанной окружности
    // Уравнения: (x - x1)^2 + (y - y1)^2 = (x - x2)^2 + (y - y2)^2
    // и (x - x1)^2 + (y - y1)^2 = (x - x3)^2 + (y - y3)^2
    double A = p2.x - p1.x;
    double B = p2.y - p1.y;
    double C = p3.x - p1.x;
    double D = p3.y - p1.y;
    double E = A * (p1.x + p2.x) + B * (p1.y + p2.y);
    double F = C * (p1.x + p3.x) + D * (p1.y + p3.y);
    double det = 2.0 * (A * D - B * C);
    if (std::fabs(det) < 1e-12) {
        // Не должно произойти из-за проверки коллинеарности, но на всякий случай
        return circleFromDiameter(p1, p2);
    }
    double cx = (D * E - B * F) / det;
    double cy = (A * F - C * E) / det;
    Point2D center = {cx, cy};
    double rad = distance(center, p1);
    return {center, rad};
}

//Алгоритм Вельцля для точек
Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n) {
    if (n == 0 || boundary.size() == 3) {
        if (boundary.empty())
            return {{0.0, 0.0}, -1.0}; // невалидная окружность
        else if (boundary.size() == 1)
            return {boundary[0], 0.0};
        else if (boundary.size() == 2)
            return circleFromDiameter(boundary[0], boundary[1]);
        else
            return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
    }

    // Случайно выбираем точку (алгоритм предполагает, что массив перетасован)
    size_t idx = n - 1;
    Point2D p = points[idx];
    
    //Рекурсивно находим круг для оставшихся n-1 точек
    Circle c = minCircleHelper(points, boundary, n - 1);
    
    if (c.isValid() && c.contains(p))
        return c;
    
    // Точка не внутри круга — добавляем её в границу
    boundary.push_back(p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points) {
    if (points.empty())
        return {{0.0, 0.0}, 0.0}; // пустое множество – окружность нулевого радиуса в нуле
    
    std::vector<Point2D> boundary;
    boundary.reserve(3);
    Circle res = minCircleHelper(points, boundary, points.size());
    return res;
}

//Преобразование отрезков в точки
std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> points;
    points.reserve(segments.size() * 2);
    for (const auto& seg : segments) {
        points.push_back(seg.start);
        points.push_back(seg.end);
    }
    return points;
}

//Основная функция для отрезков
Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> points = extractPointsFromSegments(segments);
    return minCircle(points);
}

Point2D findMidPoint(const LineSegment& segment)
{
	Point2D mid;
	mid.x = (segment.start.x + segment.end.x) / 2.0;
	mid.y = (segment.start.y + segment.end.y) / 2.0;
	return mid;
}
