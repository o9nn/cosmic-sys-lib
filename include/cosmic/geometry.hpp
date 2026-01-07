/**
 * @file geometry.hpp
 * @brief Geometric representations for the System hierarchy
 * 
 * This file provides geometric primitives and calculations for
 * visualizing the enneagram and nested system structures.
 */

#ifndef COSMIC_GEOMETRY_HPP
#define COSMIC_GEOMETRY_HPP

#include <cmath>
#include <array>
#include <vector>
#include <string>

namespace cosmic {
namespace geometry {

/// Mathematical constants
constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;
constexpr double GOLDEN_RATIO = 1.6180339887498948482;

/**
 * @brief 2D Point representation
 */
struct Point2D {
    double x = 0.0;
    double y = 0.0;
    
    Point2D() = default;
    Point2D(double x_, double y_) : x(x_), y(y_) {}
    
    Point2D operator+(const Point2D& other) const {
        return {x + other.x, y + other.y};
    }
    
    Point2D operator-(const Point2D& other) const {
        return {x - other.x, y - other.y};
    }
    
    Point2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }
    
    double distance(const Point2D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    Point2D rotate(double angle, const Point2D& center = {0, 0}) const {
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        double tx = x - center.x;
        double ty = y - center.y;
        return {
            center.x + tx * cos_a - ty * sin_a,
            center.y + tx * sin_a + ty * cos_a
        };
    }
};

/**
 * @brief 3D Point representation for tetrahedral structures
 */
struct Point3D {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    
    Point3D() = default;
    Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    
    Point3D operator+(const Point3D& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    
    Point3D operator-(const Point3D& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    
    Point3D operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
    
    double distance(const Point3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    
    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    Point3D normalize() const {
        double mag = magnitude();
        if (mag == 0) return {0, 0, 0};
        return {x / mag, y / mag, z / mag};
    }
};

/**
 * @brief Circle representation
 */
struct Circle {
    Point2D center;
    double radius = 1.0;
    
    Circle() = default;
    Circle(Point2D c, double r) : center(c), radius(r) {}
    Circle(double cx, double cy, double r) : center(cx, cy), radius(r) {}
    
    /// Get point on circle at given angle (radians, 0 = right, counter-clockwise)
    Point2D pointAt(double angle) const {
        return {
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        };
    }
    
    /// Get circumference
    double circumference() const {
        return TWO_PI * radius;
    }
    
    /// Get area
    double area() const {
        return PI * radius * radius;
    }
};

/**
 * @brief Triangle representation
 */
struct Triangle {
    std::array<Point2D, 3> vertices;
    
    Triangle() = default;
    Triangle(Point2D a, Point2D b, Point2D c) : vertices{a, b, c} {}
    
    /// Create equilateral triangle inscribed in circle
    static Triangle inscribedEquilateral(const Circle& circle, double rotation = 0.0) {
        Triangle t;
        for (int i = 0; i < 3; ++i) {
            double angle = rotation + (TWO_PI * i / 3.0);
            t.vertices[i] = circle.pointAt(angle);
        }
        return t;
    }
    
    /// Get centroid
    Point2D centroid() const {
        return {
            (vertices[0].x + vertices[1].x + vertices[2].x) / 3.0,
            (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0
        };
    }
    
    /// Get area using cross product
    double area() const {
        return 0.5 * std::abs(
            (vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y) -
            (vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y)
        );
    }
};

/**
 * @brief Enneagram geometry - the nine-pointed figure
 * 
 * The enneagram consists of:
 * - A circle
 * - 9 points equally spaced on the circle
 * - A triangle connecting points 3, 6, 9 (the triad)
 * - Internal lines following the sequence 1-4-2-8-5-7-1 (the process)
 */
class EnneagramGeometry {
public:
    EnneagramGeometry(const Circle& circle = Circle({0, 0}, 1.0));
    
    /// Get the bounding circle
    const Circle& circle() const { return circle_; }
    
    /// Get all 9 points
    const std::array<Point2D, 9>& points() const { return points_; }
    
    /// Get point at position (1-9)
    Point2D pointAt(int position) const;
    
    /// Get the triangle (points 3, 6, 9)
    Triangle triangle() const;
    
    /// Get the internal hexad lines (1-4-2-8-5-7-1)
    std::vector<std::pair<Point2D, Point2D>> hexadLines() const;
    
    /// Get all lines (triangle + hexad)
    std::vector<std::pair<Point2D, Point2D>> allLines() const;
    
    /// Scale the enneagram
    void scale(double factor);
    
    /// Translate the enneagram
    void translate(const Point2D& offset);
    
    /// Rotate the enneagram
    void rotate(double angle);
    
    /// Create nested enneagram at a specific position
    EnneagramGeometry nestedAt(int position, double scale_factor = 0.3) const;
    
private:
    Circle circle_;
    std::array<Point2D, 9> points_;
    
    void calculatePoints();
};

/**
 * @brief Tetrahedron geometry for System 5+ structures
 * 
 * The tetrahedron represents the 4-term structure with
 * 4 vertices, 6 edges, and 4 triangular faces.
 */
class TetrahedronGeometry {
public:
    TetrahedronGeometry(double edge_length = 1.0);
    
    /// Get all 4 vertices
    const std::array<Point3D, 4>& vertices() const { return vertices_; }
    
    /// Get all 6 edges as pairs of vertex indices
    static std::array<std::pair<int, int>, 6> edges();
    
    /// Get all 4 faces as triples of vertex indices
    static std::array<std::array<int, 3>, 4> faces();
    
    /// Get the centroid
    Point3D centroid() const;
    
    /// Get the circumradius
    double circumradius() const;
    
    /// Get the inradius
    double inradius() const;
    
    /// Scale the tetrahedron
    void scale(double factor);
    
    /// Translate the tetrahedron
    void translate(const Point3D& offset);
    
private:
    std::array<Point3D, 4> vertices_;
    double edge_length_;
    
    void calculateVertices();
};

/**
 * @brief Nested structure geometry for System 7+
 * 
 * Represents the recursive nesting of enneagrams within enneagrams.
 */
class NestedEnneagramGeometry {
public:
    NestedEnneagramGeometry(int depth = 1, const Circle& outer_circle = Circle({0, 0}, 1.0));
    
    /// Get the depth of nesting
    int depth() const { return depth_; }
    
    /// Get the outer enneagram
    const EnneagramGeometry& outer() const { return outer_; }
    
    /// Get nested enneagrams at each position
    const std::array<EnneagramGeometry, 9>& nested() const { return nested_; }
    
    /// Get all enneagrams flattened (for rendering)
    std::vector<EnneagramGeometry> allEnneagrams() const;
    
    /// Get total number of enneagrams
    size_t totalCount() const;
    
private:
    int depth_;
    EnneagramGeometry outer_;
    std::array<EnneagramGeometry, 9> nested_;
    
    void buildNested(int current_depth, double scale_factor);
};

/**
 * @brief SVG export utilities
 */
namespace svg {

/// Generate SVG path for a circle
std::string circlePath(const Circle& circle);

/// Generate SVG path for a triangle
std::string trianglePath(const Triangle& triangle);

/// Generate SVG path for enneagram
std::string enneagramPath(const EnneagramGeometry& ennea, bool include_circle = true);

/// Generate complete SVG document for an enneagram
std::string enneagramSVG(const EnneagramGeometry& ennea, 
                         double width = 400, double height = 400,
                         const std::string& stroke_color = "#333",
                         const std::string& fill_color = "none");

/// Generate SVG for nested enneagram structure
std::string nestedEnneagramSVG(const NestedEnneagramGeometry& nested,
                               double width = 800, double height = 800);

/// Generate SVG for the complete System 1-10 diagram
std::string systemHierarchySVG(double width = 400, double height = 1200);

} // namespace svg

} // namespace geometry
} // namespace cosmic

#endif // COSMIC_GEOMETRY_HPP
