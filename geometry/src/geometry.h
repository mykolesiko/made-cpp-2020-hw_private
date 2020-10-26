#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"
#include <utility>
#include <cmath>

using namespace std;

#pragma once
#define FRACTION_WRONG "fraction wrong"
#define POINTS_EQUAL "points equal"

const double EPS  = 1e-6;
class Line;
/*Напишите иерархию классов для работы с геометрическими фигурами на плоскости:
Структура Point - точка на плоскости. Точку можно задать двумя числами типа double.
Должны быть открыты поля x и y. Точки можно сравнивать операторами == и !=. */

class Point {
 public:
    Point(double x_, double y_): x(x_), y(y_) {};
    Point() {}
    bool operator ==(const Point& p);
    bool operator !=(const Point& p);
    double x;
    double y;
    void rotate(const Point& center, double angle);
    void reflex(const Point& center);
    void reflex(const Line& center);
    void scale(const Point& center, double coefficient);
};

/*Класс Line - прямая. Прямую можно задать двумя точками, можно двумя числами
 (угловой коэффициент и сдвиг), можно точкой и числом (угловой коэффициент).
Линии можно сравнивать операторами == и !=.
*/

//y = k * x + b
class Line {
 public:
    Line(double k_, double b_);
    Line(const Point& point, double k_);
    Line(const Point& p1, const Point& p2);
    bool operator ==(const Line& l);
    bool operator !=(const Line& l);
    double A;
    double B;
    double C;
};

/*Абстрактный класс Shape - фигура */

/*У любой фигуры можно спросить:
double perimeter() - периметр;
double area() - площадь;
operator==(const Shape& another) - совпадает ли эта фигура с другой;
С любой фигурой можно сделать:
rotate(Point center, double angle) - поворот на угол (в градусах, против часовой стрелки) относительно точки;
reflex(Point center) - симметрию относительно точки;
reflex(Line axis) - симметрию относительно прямой;
scale(Point center, double coefficient) - гомотетию с коэффициентом coefficient и центром center. */


class Shape {
 public:
    Shape() : perimeter_d(0), area_d(0) {}

    virtual double perimeter() = 0;
    virtual double area() = 0;
    virtual bool operator==(const Shape& another) = 0;
    virtual bool operator!=(const Shape& another) = 0;
    virtual void rotate(const Point& center, double angle) = 0;
    virtual void reflex(const Point& center) = 0;
    virtual void reflex(const Line& center) = 0;
    virtual void scale(Point center, double coefficient) = 0;
    double perimeter_d;
    double area_d;
};

/*Класс Polygon - многоугольник. Многоугольник - частный случай фигуры.
У многоугольника можно спросить verticesCount() - количество вершин
и std::vector<Point> getVertices - сами вершины без возможности изменения.
Можно сконструировать многоугольник из вектора точек-вершин в порядке обхода.
Для простоты будем считать, что многоугольники с самопересечениями
никогда не возникают (гарантируется, что в тестах таковые будут
отсутствовать). */

class Polygon : public Shape {
 public:
    Polygon(const vector<Point>& vertices_);
    Polygon() {};
    int verticesCount();
    std::vector<Point> getVertices();
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& center) override;
    void scale(Point center, double coefficient) override;
    bool operator==(const Shape& another) override;
    bool operator!=(const Shape& another) override;
    double perimeter() override;
    double area() override;
    void calc_sides();
    vector<Point> vertices;
    vector<double> sides;
};

/* Класс Ellipse - эллипс. Эллипс - частный случай фигуры.
 * У эллипса можно спросить std::pair<Point,Point> focuses()
 * - его фокусы; double eccentricity() - его эксцентриситет,
 * Point center() - его центр.
 * Эллипс можно сконструировать из двух точек и double
 * (два фокуса и сумма расстояний от эллипса до них); */

class Ellipse : public Shape {
 public:
    Ellipse(const Point& focus1_, \
            const Point& focus2_, double distance_) : \
        focus1(focus1_), focus2(focus2_), distance_to_focuses(distance_) {
        eccentricity();
    };
    double eccentricity();
    Point center();
    std::pair<Point,Point> focuses();
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& center) override;
    void scale(Point center, double coefficient) override;
    bool operator==(const Shape& another) override;
    bool operator!=(const Shape& another) override;
    double perimeter() override;
    double area() override;

  protected:
    Point focus1;
    Point focus2;
    double distance_to_focuses;
    double axis_big;
    double eccentricity_d;
    double distance_focuses;
    double axis_small;
};


/*Класс Circle - круг. Круг - частный случай эллипса.
 * У круга можно спросить double radius() - радиус.
 * Круг можно задать точкой и числом (центр и радиус). */

class Circle : public Ellipse {
 public:
    Circle(const Point& center_, double rad_) : Ellipse(center_, center_, 2 * rad_) {}
    double radius();

};

/* Класс Rectangle - прямоугольник. Прямоугольник - частный случай
 * многоугольника. У прямоугольника можно спросить Point center()
 * - его центр; std::pair<Line, Line> diagonals() - пару его диагоналей.
 * Прямоугольник можно сконструировать по двум точкам
 * (его противоположным вершинам) и числу (отношению смежных сторон),
 * причем из двух таких прямоугольников выбирается тот, у которого более
 * короткая сторона расположена по левую сторону от диагонали, если смотреть
 * от первой заданной точки в направлении второй. */

class Rectangle : public Polygon {
 public:
    Rectangle(const Point& point1,  const Point& point, double fraction);
    std::pair<Line, Line> diagonals();
    Point center();
};

 /* Класс Square - квадрат. Квадрат - частный случай прямоугольника.
  * У квадрата можно спросить Circle circumscribedCircle(),
  * Circle inscribedCircle(). Квадрат можно задать двумя точками -
  * противоположными вершинами. */
class Circle;

class Square : public Rectangle {
 public:
    Square(const Point& point1,  const Point& point2) : Rectangle(point1, point2, 1) {}
    Circle circumscribedCircle();
    Circle inscribedCircle();
};

 /* Класс Triangle - треугольник. Треугольник - частный случай
  * многоугольника. У треугольника можно спросить Circle
  * circumscribedCircle(), Circle inscribedCircle(), Point centroid()
  * - его центр масс, Point orthocenter() - его ортоцентр,
  * Line EulerLine() - его прямую Эйлера, Circle ninePointsCircle()
  *  - его окружность Эйлера.*/

class Triangle : public Polygon {
 public:
    Triangle(const Point& p1, const Point&p2, const Point& p3);
    Triangle(vector<Point>& vertices_): Polygon(vertices_) {};
    Circle circumscribedCircle();
    Circle inscribedCircle();
    Point centroid();
    Point orthocenter();
    Line EulerLine();
    Circle ninePointsCircle();
};

 double length(const Point& point1, const Point& point2) {
    return  sqrt(pow(point1.x - point2.x, 2) + pow((point1.y - point2.y), 2));
}

bool Point :: operator==(const Point& p) {
   return ((fabs(p.x - x) < EPS) && (fabs(p.y - y) < EPS));
}

bool Point :: operator!=(const Point& p) {
    //if ((fabs(p.x - x) < EPS) && (fabs(p.y - y) < EPS)) return false;
    return (!(operator== (p)));
}

void Point :: rotate(const Point& center, double angle) {
    double xnew = (x - center.x) * cos(angle/180.0 * M_PI) - (y - center.y) * sin(angle/180.0 * M_PI) + center.x;
    double ynew = (x - center.x) * sin(angle/180.0 * M_PI) + (y - center.y) * cos(angle/180.0 * M_PI) + center.y;
    x = xnew;
    y = ynew;
}
void Point :: reflex(const Line& line) {
    double t = (x * line.A + y * line.B + line.C) / (line.A * line.A + line.B * line.B);
    x = x + 2 * line.A * t;
    y = y + 2 * line.B * t;
}

void Point :: reflex(const Point& center) {
    x = 2 * center.x - x;
    y = 2 * center.y - y;
}

void Point :: scale(const Point& center, double coefficient) {
    x = coefficient * (x - center.x) + center.x;
    y = coefficient * (y - center.y) + center.y;
};


Line :: Line(double k_, double b_) {
    A = k_;
    B = -1;
    C = b_;
}

Line :: Line(const Point& point, double k_) {
    A = k_;
    B = -1;
    C = point.y - k_ * point.x;
}

Line :: Line(const Point& p1, const Point& p2) {
    if (fabs(p1.x - p2.x) > EPS) {
        A = (p1.y - p2.y) / (p1.x - p2.x);
        B = -1;
        C = p1.y - A * p1.x;
    } else if (fabs(p1.y - p2.y) > EPS) {
        B = (p1.x - p2.x) / (p1.y - p2.y);
        A = -1;
        C = p1.x - B * p1.y;
    } else throw(std::invalid_argument(POINTS_EQUAL));
}

bool Line :: operator ==(const Line& line) {
    //return ((fabs(A - line.A) < EPS) && (fabs(B - line.B) < EPS));
    double B1 = line.B;
    double A1 = line.A;
    double C1 = line.C;
    if (fabs(B) < EPS) {
        if (fabs(B1) > EPS)  return false;
        if (fabs(A) < EPS) return false;
        double k = A1 / A;
        if (fabs(C) < EPS) {
            if (fabs(C1) > EPS)  return false;
        }
        if (fabs(C1/C - k) < EPS) return true;
        else return (false);
    }
    if (fabs(A) < EPS) {
        if (fabs(A1) > EPS)  return false;
        if (fabs(B) < EPS) return false;
        double k = B1 / B;
        if (fabs(C) < EPS) {
            if (fabs(C1) > EPS)  return false;
        }
        if (fabs(C1/C - k) < EPS) return true;
        else return (false);
    }
}

bool Line :: operator !=(const Line& line) {
    return (!operator == (line));
}

Polygon :: Polygon(const vector<Point>& vertices_) {
   vertices = vertices_;
   calc_sides();
   perimeter();
}
int Polygon :: verticesCount() {
    return (vertices.size());
}
std::vector<Point> Polygon :: getVertices() {
    return vertices;
}

void Polygon :: rotate(const Point& center, double angle) {
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        it->rotate(center, angle);
    }
}
void Polygon :: reflex(const Point& center) {
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        it->reflex(center);
    }
}
void Polygon :: reflex(const Line& line) {
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        it->reflex(line);
    }
}
void Polygon :: scale(Point center, double coefficient) {
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        it->scale(center, coefficient);
    }
    calc_sides();
}

double Polygon ::perimeter() {
   perimeter_d = 0;
   for (auto el : sides) {
       perimeter_d += el;
   }
   return (perimeter_d);
}

double Polygon :: area() {
    area_d = 0;
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        area_d += (vertices[i].x * vertices[(i + 1) % n].y - vertices[(i + 1) % n].x * vertices[i].y);
    }
    area_d = fabs(area_d) / 2;
    return  (area_d);
}

void Polygon :: calc_sides() {
    int n = vertices.size();
    sides.resize(n);
    for (int i = 0; i < n; i++) {
        sides[i] = length(vertices[i], vertices[(i + 1) % n]);
    }
}

double Ellipse :: eccentricity() {
    axis_big = distance_to_focuses / 2;
    distance_focuses = length(focus1, focus2);
    eccentricity_d = distance_focuses / distance_to_focuses;
    axis_small = sqrt(axis_big * axis_big - distance_focuses * distance_focuses / 4);
    return(eccentricity_d);
}

Point Ellipse :: center() {
    return Point((focus1.x + focus2.x) / 2, (focus1.y + focus2.y) / 2);
}

std::pair<Point,Point> Ellipse :: focuses() {
    return (make_pair(focus1, focus2));
}

void  Ellipse :: rotate(const Point& center, double angle) {
    focus1.rotate(center, angle);
    focus2.rotate(center, angle);
}

void  Ellipse :: reflex(const Point& center) {
    focus1.reflex(center);
    focus2.reflex(center);
}

void  Ellipse :: reflex(const Line& line) {
    focus1.reflex(line);
    focus2.reflex(line);
}

void  Ellipse :: scale(Point center, double coefficient) {
    focus1.scale(center, coefficient);
    focus2.scale(center, coefficient);
    distance_to_focuses *= coefficient;

}

double Ellipse :: perimeter() {
    double a = axis_big ;
    double b = axis_small;
    double c = 3 * pow((a - b)/(a + b), 2);
    perimeter_d = M_PI * (a + b)*(1 + c/(10 + sqrt(4 - c)));// * std::comp_ellint_2(eccentricity_d);
    return perimeter_d;
}

double Ellipse :: area() {
    return (area_d = M_PI * axis_small * axis_big);
}

bool Ellipse :: operator==(const Shape& anotherShape) {
    const Ellipse* another = dynamic_cast<const Ellipse*>(&anotherShape);
    return ((focus1 == another->focus1) && (focus2 == another->focus2) && (distance_to_focuses == another->distance_to_focuses));
}

bool Ellipse :: operator!=(const Shape& anotherShape) {
    return (!operator==(anotherShape));
}

double Circle :: radius() {
    return distance_to_focuses / 2;
}

Rectangle :: Rectangle(const Point& point1,  const Point& point2, double fraction) {
    if (fraction < 0) {
        throw (std::invalid_argument(FRACTION_WRONG));
    }
    vertices.resize(4);
    vertices[0] = point1;
    vertices[2] = point2;
    double fraction_new;
    if (fraction > 1) {
        fraction_new = 1 / fraction;
    } else {
        fraction_new = fraction;
    }
    double diag_len= length(point1, point2);
    double big = diag_len / sqrt(1 +  fraction_new * fraction_new);
    double angle = acos(big / diag_len);
    Point point2_copy = point2;
    point2_copy.rotate(center(), M_PI - 2 * angle);
    vertices[1] = point2_copy;
    Point point1_copy = point1;
    point1_copy.rotate(center(), M_PI - 2 * angle);
    vertices[3] = point1_copy;
}

std::pair<Line, Line> Rectangle :: diagonals() {
    Line line1(vertices[0], vertices[2]);
    Line line2(vertices[1], vertices[3]);
    return (make_pair(line1, line2));
}

Point Rectangle :: center() {
    return Point((vertices[0].x + vertices[2].x) / 2, (vertices[0].y + vertices[2].y) / 2);
}

Circle Square :: circumscribedCircle() {
    return (Circle(center(), length(vertices[0],vertices[2])/2));
}

Circle Square :: inscribedCircle() {
    return (Circle(center(), length(vertices[0],vertices[1])/2));
}


double det(double mat[3][3]) {
    double dd = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    dd -= mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
    dd += mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return (dd);
}

Circle Triangle :: circumscribedCircle() {
    double a = sides[1];
    double b = sides[2];
    double c = sides[0];
    double rad = a * b * c / (4 * sqrt(perimeter_d/2 * (perimeter_d/2 - a) * \
                              (perimeter_d/2 - b) * (perimeter_d/2 - c)));
    Point vA = vertices[0];
    Point vB = vertices[1];
    Point vC = vertices[2];
    double mD[3][3] = {{vA.x, vA.y, 1}, {vB.x, vB.y, 1}, {vC.x, vC.y, 1}};
    double D = 2 * det(mD);
    double mX[3][3] = {{vA.x * vA.x + vA.y * vA.y, vA.y, 1}, {vB.x * vB.x + vB.y * vB.y, vB.y, 1}, \
                       {vC.x * vC.x + vC.y * vC.y, vC.y, 1}};
    double mY[3][3] = {{vA.x * vA.x + vA.y * vA.y, vA.x, 1}, {vB.x * vB.x + vB.y * vB.y, vB.x, 1}, \
                       {vC.x * vC.x + vC.y * vC.y, vC.x, 1}};
    double x0 = det(mX)/D;
    double y0 = -det(mY)/D;
    return(Circle(Point(x0, y0), rad));
}

Circle Triangle :: inscribedCircle() {
    double a = sides[1];
    double b = sides[2];
    double c = sides[0];
    Point vA = vertices[0];
    Point vB = vertices[1];
    Point vC = vertices[2];
    double rad = sqrt((perimeter_d/2 - a) * (perimeter_d/2 - b) * (perimeter_d/2 - c) / (perimeter_d/2));
    double x0 = (a * vA.x + b * vB.x + c* vC.x)/(a + b + c);
    double y0 = (a * vA.y + b * vB.y + c* vC.y)/(a + b + c);
    return(Circle(Point(x0, y0), rad));
}

Point Triangle ::centroid() {
    Point vA = vertices[0];
    Point vB = vertices[1];
    Point vC = vertices[2];
    double x = (vA.x + vB.x + vC.x)/3;
    double y = (vA.y + vB.y + vC.y)/3;
    return (Point(x, y));
}

Point Triangle ::orthocenter() {
    Point vA = vertices[0];
    Point vB = vertices[1];
    Point vC = vertices[2];
    double m1[3][3] = {{vA.y, vB.x * vC.x + vA.y * vA.y, 1 }, {vB.y, vA.x * vC.x + vB.y * vB.y, 1}, {vC.y, vA.x * vB.x + vC.y * vC.y , 1}};
    double m2[3][3] = {{vA.x, vB.y * vC.y + vA.x *vA.x , 1 }, {vB.x, vA.y * vC.y + vB.x * vB.x, 1}, {vC.x, vA.y * vB.y + vC.x * vC.x, 1}};
    double m[3][3] = {{vA.x, vA.y, 1}, {vB.x, vB.y, 1}, {vC.x, vC.y, 1}};
    double x0 = det(m1);
    x0 = x0 / det(m);
    double y0 = -det(m2);
    y0 = y0 / det(m);
    return Point(x0, y0);
}
Line Triangle ::EulerLine() {
    Point oc = orthocenter();
    Point cc = centroid();
    return Line(oc, cc);
}
Circle Triangle :: ninePointsCircle (){
    Point oc = orthocenter();
    Circle circ = circumscribedCircle();
    double x = (oc.x + circ.center().x)/2;
    double y = (oc.y + circ.center().y)/2;
    return Circle(Point(x, y), circ.radius()/2);
}

Triangle :: Triangle(const Point& p1, const Point&p2, const Point& p3) {
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    calc_sides();
    perimeter();
}

bool Polygon ::  operator==(const Shape& anotherShape) {
    const Polygon* another = dynamic_cast<const Polygon*>(&anotherShape);
    if (another == false) return false;
    if (vertices.size() != another->vertices.size()) return false;
    int sdvig = 0;
    int n = vertices.size();
    while (sdvig < n) {
        bool equal = true;
        for (int i = 0 ; i < vertices.size(); i++) {
            if (vertices[i] != another->vertices[(i + sdvig) % n])
                equal = false;
                break;
        }
        if (equal == true) return true;
        sdvig++;
    }
    std::reverse(vertices.begin(), vertices.end());
    sdvig = 0;
    while (sdvig < n) {
        bool equal = true;
        for (int i = 0 ; i < vertices.size(); i++) {
            if (vertices[i] != another->vertices[(i + sdvig) % n])
                equal = false;
                break;
        }
        if (equal == true) return true;
        sdvig++;
    }
     return(false);
}

bool Polygon ::  operator!=(const Shape& another) {
    return (!operator==(another));
}
