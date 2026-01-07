/**
 * @file test_geometry.cpp
 * @brief Tests for the geometry classes
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include "cosmic/cosmic.hpp"

using namespace cosmic;
using namespace cosmic::geometry;

constexpr double EPSILON = 1e-9;

bool approxEqual(double a, double b) {
    return std::abs(a - b) < EPSILON;
}

void test_point2d() {
    std::cout << "Testing Point2D..." << std::endl;
    
    Point2D p1(3.0, 4.0);
    Point2D p2(1.0, 2.0);
    
    // Test addition
    auto sum = p1 + p2;
    assert(approxEqual(sum.x, 4.0));
    assert(approxEqual(sum.y, 6.0));
    
    // Test subtraction
    auto diff = p1 - p2;
    assert(approxEqual(diff.x, 2.0));
    assert(approxEqual(diff.y, 2.0));
    
    // Test scalar multiplication
    auto scaled = p1 * 2.0;
    assert(approxEqual(scaled.x, 6.0));
    assert(approxEqual(scaled.y, 8.0));
    
    // Test distance
    Point2D origin(0, 0);
    assert(approxEqual(p1.distance(origin), 5.0));
    
    // Test rotation
    Point2D p(1.0, 0.0);
    auto rotated = p.rotate(PI / 2.0);
    assert(approxEqual(rotated.x, 0.0));
    assert(approxEqual(rotated.y, 1.0));
    
    std::cout << "  PASSED" << std::endl;
}

void test_point3d() {
    std::cout << "Testing Point3D..." << std::endl;
    
    Point3D p1(1.0, 2.0, 3.0);
    Point3D p2(4.0, 5.0, 6.0);
    
    // Test addition
    auto sum = p1 + p2;
    assert(approxEqual(sum.x, 5.0));
    assert(approxEqual(sum.y, 7.0));
    assert(approxEqual(sum.z, 9.0));
    
    // Test magnitude
    Point3D p(3.0, 4.0, 0.0);
    assert(approxEqual(p.magnitude(), 5.0));
    
    // Test normalize
    auto norm = p.normalize();
    assert(approxEqual(norm.magnitude(), 1.0));
    
    std::cout << "  PASSED" << std::endl;
}

void test_circle() {
    std::cout << "Testing Circle..." << std::endl;
    
    Circle c({0, 0}, 1.0);
    
    // Test point at angle
    auto p0 = c.pointAt(0);
    assert(approxEqual(p0.x, 1.0));
    assert(approxEqual(p0.y, 0.0));
    
    auto p90 = c.pointAt(PI / 2.0);
    assert(approxEqual(p90.x, 0.0));
    assert(approxEqual(p90.y, 1.0));
    
    // Test circumference
    assert(approxEqual(c.circumference(), TWO_PI));
    
    // Test area
    assert(approxEqual(c.area(), PI));
    
    std::cout << "  PASSED" << std::endl;
}

void test_triangle() {
    std::cout << "Testing Triangle..." << std::endl;
    
    Triangle t(Point2D(0, 0), Point2D(1, 0), Point2D(0.5, std::sqrt(3)/2));
    
    // Test centroid
    auto centroid = t.centroid();
    assert(approxEqual(centroid.x, 0.5));
    assert(approxEqual(centroid.y, std::sqrt(3)/6));
    
    // Test inscribed equilateral
    Circle c({0, 0}, 1.0);
    auto inscribed = Triangle::inscribedEquilateral(c);
    
    // All vertices should be on the circle
    for (const auto& v : inscribed.vertices) {
        double dist = v.distance(c.center);
        assert(approxEqual(dist, c.radius));
    }
    
    std::cout << "  PASSED" << std::endl;
}

void test_enneagram_geometry() {
    std::cout << "Testing EnneagramGeometry..." << std::endl;
    
    EnneagramGeometry ennea(Circle({0, 0}, 1.0));
    
    // Test 9 points exist
    const auto& points = ennea.points();
    assert(points.size() == 9);
    
    // All points should be on the circle
    for (const auto& p : points) {
        double dist = p.distance(ennea.circle().center);
        assert(approxEqual(dist, ennea.circle().radius));
    }
    
    // Test pointAt
    auto p1 = ennea.pointAt(1);
    auto p9 = ennea.pointAt(9);
    assert(!approxEqual(p1.x, p9.x) || !approxEqual(p1.y, p9.y));
    
    // Test triangle
    auto tri = ennea.triangle();
    assert(tri.vertices.size() == 3);
    
    // Test hexad lines
    auto hexad = ennea.hexadLines();
    assert(hexad.size() == 6);
    
    // Test all lines
    auto all = ennea.allLines();
    assert(all.size() == 9);  // 6 hexad + 3 triangle
    
    // Test nested enneagram
    auto nested = ennea.nestedAt(1, 0.3);
    assert(nested.circle().radius < ennea.circle().radius);
    
    std::cout << "  PASSED" << std::endl;
}

void test_tetrahedron_geometry() {
    std::cout << "Testing TetrahedronGeometry..." << std::endl;
    
    TetrahedronGeometry tetra(1.0);
    
    // Test 4 vertices
    const auto& vertices = tetra.vertices();
    assert(vertices.size() == 4);
    
    // Test edges
    auto edges = TetrahedronGeometry::edges();
    assert(edges.size() == 6);
    
    // Test faces
    auto faces = TetrahedronGeometry::faces();
    assert(faces.size() == 4);
    
    // Test centroid is near origin
    auto centroid = tetra.centroid();
    assert(std::abs(centroid.x) < 0.01);
    assert(std::abs(centroid.y) < 0.01);
    assert(std::abs(centroid.z) < 0.01);
    
    // Test circumradius and inradius
    assert(tetra.circumradius() > tetra.inradius());
    
    std::cout << "  PASSED" << std::endl;
}

void test_nested_enneagram_geometry() {
    std::cout << "Testing NestedEnneagramGeometry..." << std::endl;
    
    NestedEnneagramGeometry nested(1, Circle({0, 0}, 1.0));
    
    // Test depth
    assert(nested.depth() == 1);
    
    // Test outer enneagram
    assert(nested.outer().points().size() == 9);
    
    // Test nested enneagrams
    const auto& inner = nested.nested();
    assert(inner.size() == 9);
    
    for (const auto& n : inner) {
        assert(n.points().size() == 9);
        assert(n.circle().radius < nested.outer().circle().radius);
    }
    
    // Test all enneagrams
    auto all = nested.allEnneagrams();
    assert(all.size() == 10);  // 1 outer + 9 nested
    
    // Test total count
    assert(nested.totalCount() == 10);
    
    std::cout << "  PASSED" << std::endl;
}

void test_svg_generation() {
    std::cout << "Testing SVG generation..." << std::endl;
    
    // Test circle path
    Circle c({100, 100}, 50);
    std::string circle_svg = svg::circlePath(c);
    assert(circle_svg.find("circle") != std::string::npos);
    assert(circle_svg.find("cx=") != std::string::npos);
    
    // Test triangle path
    Triangle t(Point2D(0, 0), Point2D(100, 0), Point2D(50, 86.6));
    std::string tri_svg = svg::trianglePath(t);
    assert(tri_svg.find("polygon") != std::string::npos);
    
    // Test enneagram SVG
    EnneagramGeometry ennea;
    std::string ennea_svg = svg::enneagramSVG(ennea, 400, 400);
    assert(ennea_svg.find("<svg") != std::string::npos);
    assert(ennea_svg.find("</svg>") != std::string::npos);
    assert(ennea_svg.find("circle") != std::string::npos);
    assert(ennea_svg.find("line") != std::string::npos);
    
    // Test nested enneagram SVG
    NestedEnneagramGeometry nested(1);
    std::string nested_svg = svg::nestedEnneagramSVG(nested, 600, 600);
    assert(nested_svg.find("<svg") != std::string::npos);
    
    // Test system hierarchy SVG
    std::string hierarchy_svg = svg::systemHierarchySVG(400, 1200);
    assert(hierarchy_svg.find("<svg") != std::string::npos);
    assert(hierarchy_svg.find("SYSTEM 1") != std::string::npos);
    assert(hierarchy_svg.find("SYSTEM 10") != std::string::npos);
    
    std::cout << "  PASSED" << std::endl;
}

int main() {
    std::cout << "=== Geometry Tests ===" << std::endl;
    
    test_point2d();
    test_point3d();
    test_circle();
    test_triangle();
    test_enneagram_geometry();
    test_tetrahedron_geometry();
    test_nested_enneagram_geometry();
    test_svg_generation();
    
    std::cout << "\nAll tests PASSED!" << std::endl;
    return 0;
}
