#include "Vector2D.h"
#include <cmath>   // Para std::sqrt y std::pow
#include <ostream> // Para std::ostream

// Constructors
Vector2D::Vector2D() : x(0), y(0) {}
Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

// Getters and Setters
float Vector2D::getX() const {
    return x;
}

void Vector2D::setX(float x) {
    this->x = x;
}

float Vector2D::getY() const {
    return y;
}

void Vector2D::setY(float y) {
    this->y = y;
}

// Vector operations
float Vector2D::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalized() const {
    float mag = magnitude();
    if (mag == 0) return Vector2D(0, 0); // Evitar división por cero
    return Vector2D(x / mag, y / mag);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    if (scalar == 0) throw std::runtime_error("Division by zero"); // Manejo de errores
    return Vector2D(x / scalar, y / scalar);
}

float Vector2D::distanceMahattan(const Vector2D& other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
}

float Vector2D::distanceEuclidean(const Vector2D& other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
}

float Vector2D::distanceSquared(const Vector2D& other) const {
    return std::pow(x - other.x, 2) + std::pow(y - other.y, 2);
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

// Comparison operators
bool Vector2D::operator==(const Vector2D& other) const {
    return x == other.x && y == other.y;
}

bool Vector2D::operator!=(const Vector2D& other) const {
    return !(*this == other); // Reutilizar la lógica del operador ==
}

bool Vector2D::operator<(const Vector2D& other) const {
    if (x < other.x) return true;
    if (x == other.x && y < other.y) return true;
    return false;
}
