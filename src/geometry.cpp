/**
 * @file geometry.cpp
 * @brief Implementation of geometric representations
 */

#include "cosmic/geometry.hpp"
#include <sstream>
#include <iomanip>

namespace cosmic {
namespace geometry {

// ============================================================================
// EnneagramGeometry Implementation
// ============================================================================

EnneagramGeometry::EnneagramGeometry(const Circle& circle) : circle_(circle) {
    calculatePoints();
}

void EnneagramGeometry::calculatePoints() {
    // Points are numbered 1-9, starting at the top and going clockwise
    // Point 9 is at the top (12 o'clock position)
    // In standard math coordinates, this is PI/2 radians
    
    for (int i = 0; i < 9; ++i) {
        // Position 9 is at index 8, position 1 is at index 0
        // We want position 9 at top, so start at PI/2 and go clockwise (negative direction)
        int position = (i + 1);  // 1-9
        double angle = PI / 2.0 - (TWO_PI * i / 9.0);
        points_[i] = circle_.pointAt(angle);
    }
}

Point2D EnneagramGeometry::pointAt(int position) const {
    if (position < 1 || position > 9) {
        return {0, 0};
    }
    return points_[position - 1];
}

Triangle EnneagramGeometry::triangle() const {
    // The triangle connects positions 3, 6, 9
    return Triangle(points_[2], points_[5], points_[8]);
}

std::vector<std::pair<Point2D, Point2D>> EnneagramGeometry::hexadLines() const {
    // The internal hexad follows the sequence 1-4-2-8-5-7-1
    // This represents the law of seven (the octave with two intervals)
    std::vector<std::pair<Point2D, Point2D>> lines;
    
    // 1 -> 4
    lines.push_back({points_[0], points_[3]});
    // 4 -> 2
    lines.push_back({points_[3], points_[1]});
    // 2 -> 8
    lines.push_back({points_[1], points_[7]});
    // 8 -> 5
    lines.push_back({points_[7], points_[4]});
    // 5 -> 7
    lines.push_back({points_[4], points_[6]});
    // 7 -> 1
    lines.push_back({points_[6], points_[0]});
    
    return lines;
}

std::vector<std::pair<Point2D, Point2D>> EnneagramGeometry::allLines() const {
    auto lines = hexadLines();
    
    // Add triangle lines (3-6-9)
    lines.push_back({points_[2], points_[5]});  // 3 -> 6
    lines.push_back({points_[5], points_[8]});  // 6 -> 9
    lines.push_back({points_[8], points_[2]});  // 9 -> 3
    
    return lines;
}

void EnneagramGeometry::scale(double factor) {
    circle_.radius *= factor;
    calculatePoints();
}

void EnneagramGeometry::translate(const Point2D& offset) {
    circle_.center = circle_.center + offset;
    calculatePoints();
}

void EnneagramGeometry::rotate(double angle) {
    for (auto& point : points_) {
        point = point.rotate(angle, circle_.center);
    }
}

EnneagramGeometry EnneagramGeometry::nestedAt(int position, double scale_factor) const {
    if (position < 1 || position > 9) {
        return EnneagramGeometry();
    }
    
    Point2D center = points_[position - 1];
    double new_radius = circle_.radius * scale_factor;
    
    return EnneagramGeometry(Circle(center, new_radius));
}

// ============================================================================
// TetrahedronGeometry Implementation
// ============================================================================

TetrahedronGeometry::TetrahedronGeometry(double edge_length) 
    : edge_length_(edge_length) {
    calculateVertices();
}

void TetrahedronGeometry::calculateVertices() {
    // Regular tetrahedron with centroid at origin
    // Using the standard orientation with one vertex pointing up
    
    double a = edge_length_;
    double h = a * std::sqrt(2.0 / 3.0);  // Height of tetrahedron
    double r = a / std::sqrt(3.0);         // Radius of base triangle circumcircle
    
    // Base triangle vertices (in xy plane, shifted down)
    double base_z = -h / 4.0;
    vertices_[0] = {r, 0, base_z};
    vertices_[1] = {-r / 2.0, r * std::sqrt(3.0) / 2.0, base_z};
    vertices_[2] = {-r / 2.0, -r * std::sqrt(3.0) / 2.0, base_z};
    
    // Top vertex
    vertices_[3] = {0, 0, 3.0 * h / 4.0};
}

std::array<std::pair<int, int>, 6> TetrahedronGeometry::edges() {
    return {{
        {0, 1}, {1, 2}, {2, 0},  // Base triangle
        {0, 3}, {1, 3}, {2, 3}   // Edges to apex
    }};
}

std::array<std::array<int, 3>, 4> TetrahedronGeometry::faces() {
    return {{
        {0, 1, 2},  // Base
        {0, 1, 3},  // Front
        {1, 2, 3},  // Right
        {2, 0, 3}   // Left
    }};
}

Point3D TetrahedronGeometry::centroid() const {
    return {
        (vertices_[0].x + vertices_[1].x + vertices_[2].x + vertices_[3].x) / 4.0,
        (vertices_[0].y + vertices_[1].y + vertices_[2].y + vertices_[3].y) / 4.0,
        (vertices_[0].z + vertices_[1].z + vertices_[2].z + vertices_[3].z) / 4.0
    };
}

double TetrahedronGeometry::circumradius() const {
    return edge_length_ * std::sqrt(6.0) / 4.0;
}

double TetrahedronGeometry::inradius() const {
    return edge_length_ * std::sqrt(6.0) / 12.0;
}

void TetrahedronGeometry::scale(double factor) {
    edge_length_ *= factor;
    for (auto& v : vertices_) {
        v = v * factor;
    }
}

void TetrahedronGeometry::translate(const Point3D& offset) {
    for (auto& v : vertices_) {
        v = v + offset;
    }
}

// ============================================================================
// NestedEnneagramGeometry Implementation
// ============================================================================

NestedEnneagramGeometry::NestedEnneagramGeometry(int depth, const Circle& outer_circle)
    : depth_(depth), outer_(outer_circle) {
    buildNested(depth, 0.25);
}

void NestedEnneagramGeometry::buildNested(int current_depth, double scale_factor) {
    if (current_depth <= 0) return;
    
    for (int i = 0; i < 9; ++i) {
        nested_[i] = outer_.nestedAt(i + 1, scale_factor);
    }
}

std::vector<EnneagramGeometry> NestedEnneagramGeometry::allEnneagrams() const {
    std::vector<EnneagramGeometry> result;
    result.push_back(outer_);
    
    for (const auto& n : nested_) {
        result.push_back(n);
    }
    
    return result;
}

size_t NestedEnneagramGeometry::totalCount() const {
    // 1 outer + 9 nested = 10 for depth 1
    // For deeper nesting, it would be 1 + 9 + 81 + ...
    size_t count = 1;
    size_t level_count = 9;
    for (int d = 0; d < depth_; ++d) {
        count += level_count;
        level_count *= 9;
    }
    return count;
}

// ============================================================================
// SVG Export Functions
// ============================================================================

namespace svg {

std::string circlePath(const Circle& circle) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "<circle cx=\"" << circle.center.x 
       << "\" cy=\"" << circle.center.y 
       << "\" r=\"" << circle.radius << "\"/>";
    return ss.str();
}

std::string trianglePath(const Triangle& triangle) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "<polygon points=\"";
    for (int i = 0; i < 3; ++i) {
        if (i > 0) ss << " ";
        ss << triangle.vertices[i].x << "," << triangle.vertices[i].y;
    }
    ss << "\"/>";
    return ss.str();
}

std::string enneagramPath(const EnneagramGeometry& ennea, bool include_circle) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    if (include_circle) {
        ss << circlePath(ennea.circle()) << "\n";
    }
    
    // Draw all lines
    auto lines = ennea.allLines();
    for (const auto& line : lines) {
        ss << "<line x1=\"" << line.first.x 
           << "\" y1=\"" << line.first.y
           << "\" x2=\"" << line.second.x 
           << "\" y2=\"" << line.second.y << "\"/>\n";
    }
    
    // Draw points
    const auto& points = ennea.points();
    for (int i = 0; i < 9; ++i) {
        ss << "<circle cx=\"" << points[i].x 
           << "\" cy=\"" << points[i].y 
           << "\" r=\"3\" class=\"point\"/>\n";
        
        // Add position number
        double label_offset = ennea.circle().radius * 0.15;
        double angle = PI / 2.0 - (TWO_PI * i / 9.0);
        double lx = points[i].x + label_offset * std::cos(angle);
        double ly = points[i].y - label_offset * std::sin(angle);
        ss << "<text x=\"" << lx << "\" y=\"" << ly 
           << "\" class=\"label\">" << (i + 1) << "</text>\n";
    }
    
    return ss.str();
}

std::string enneagramSVG(const EnneagramGeometry& ennea, 
                         double width, double height,
                         const std::string& stroke_color,
                         const std::string& fill_color) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    // Calculate transform to center the enneagram
    double scale = std::min(width, height) * 0.4 / ennea.circle().radius;
    double tx = width / 2.0;
    double ty = height / 2.0;
    
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
       << "width=\"" << width << "\" height=\"" << height << "\" "
       << "viewBox=\"0 0 " << width << " " << height << "\">\n";
    
    // Style
    ss << "<style>\n";
    ss << "  .enneagram { stroke: " << stroke_color << "; fill: " << fill_color << "; stroke-width: 1.5; }\n";
    ss << "  .point { fill: " << stroke_color << "; }\n";
    ss << "  .label { font-family: Arial, sans-serif; font-size: 12px; text-anchor: middle; dominant-baseline: middle; }\n";
    ss << "  .triangle { stroke: " << stroke_color << "; stroke-width: 2; fill: none; }\n";
    ss << "  .hexad { stroke: " << stroke_color << "; stroke-width: 1; fill: none; }\n";
    ss << "</style>\n";
    
    // Background
    ss << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    
    // Transform group
    ss << "<g transform=\"translate(" << tx << "," << ty << ") scale(" << scale << "," << -scale << ")\" class=\"enneagram\">\n";
    
    // Circle
    ss << "  " << circlePath(ennea.circle()) << "\n";
    
    // Triangle (3-6-9) - thicker line
    auto tri = ennea.triangle();
    ss << "  <polygon points=\"";
    for (int i = 0; i < 3; ++i) {
        if (i > 0) ss << " ";
        ss << tri.vertices[i].x << "," << tri.vertices[i].y;
    }
    ss << "\" class=\"triangle\"/>\n";
    
    // Hexad lines
    auto hexad = ennea.hexadLines();
    ss << "  <g class=\"hexad\">\n";
    for (const auto& line : hexad) {
        ss << "    <line x1=\"" << line.first.x 
           << "\" y1=\"" << line.first.y
           << "\" x2=\"" << line.second.x 
           << "\" y2=\"" << line.second.y << "\"/>\n";
    }
    ss << "  </g>\n";
    
    // Points and labels (need to flip y for text)
    const auto& points = ennea.points();
    for (int i = 0; i < 9; ++i) {
        ss << "  <circle cx=\"" << points[i].x 
           << "\" cy=\"" << points[i].y 
           << "\" r=\"" << (3.0 / scale) << "\" class=\"point\"/>\n";
    }
    
    ss << "</g>\n";
    
    // Labels (outside transform to keep text upright)
    ss << "<g class=\"label\">\n";
    for (int i = 0; i < 9; ++i) {
        double angle = PI / 2.0 - (TWO_PI * i / 9.0);
        double label_r = ennea.circle().radius * 1.15;
        double lx = tx + scale * label_r * std::cos(angle);
        double ly = ty - scale * label_r * std::sin(angle);
        ss << "  <text x=\"" << lx << "\" y=\"" << ly << "\">" << (i + 1) << "</text>\n";
    }
    ss << "</g>\n";
    
    ss << "</svg>\n";
    
    return ss.str();
}

std::string nestedEnneagramSVG(const NestedEnneagramGeometry& nested,
                               double width, double height) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    double scale = std::min(width, height) * 0.35 / nested.outer().circle().radius;
    double tx = width / 2.0;
    double ty = height / 2.0;
    
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
       << "width=\"" << width << "\" height=\"" << height << "\">\n";
    
    ss << "<style>\n";
    ss << "  .outer { stroke: #333; fill: none; stroke-width: 2; }\n";
    ss << "  .nested { stroke: #666; fill: none; stroke-width: 1; }\n";
    ss << "  .point { fill: #333; }\n";
    ss << "</style>\n";
    
    ss << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    
    // Outer enneagram
    ss << "<g transform=\"translate(" << tx << "," << ty << ") scale(" << scale << "," << -scale << ")\" class=\"outer\">\n";
    ss << "  " << circlePath(nested.outer().circle()) << "\n";
    auto lines = nested.outer().allLines();
    for (const auto& line : lines) {
        ss << "  <line x1=\"" << line.first.x 
           << "\" y1=\"" << line.first.y
           << "\" x2=\"" << line.second.x 
           << "\" y2=\"" << line.second.y << "\"/>\n";
    }
    ss << "</g>\n";
    
    // Nested enneagrams
    ss << "<g transform=\"translate(" << tx << "," << ty << ") scale(" << scale << "," << -scale << ")\" class=\"nested\">\n";
    for (const auto& n : nested.nested()) {
        ss << "  " << circlePath(n.circle()) << "\n";
        auto nlines = n.allLines();
        for (const auto& line : nlines) {
            ss << "  <line x1=\"" << line.first.x 
               << "\" y1=\"" << line.first.y
               << "\" x2=\"" << line.second.x 
               << "\" y2=\"" << line.second.y << "\"/>\n";
        }
    }
    ss << "</g>\n";
    
    ss << "</svg>\n";
    
    return ss.str();
}

std::string systemHierarchySVG(double width, double height) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
       << "width=\"" << width << "\" height=\"" << height << "\">\n";
    
    ss << "<style>\n";
    ss << "  .system { stroke: #333; fill: none; stroke-width: 1.5; }\n";
    ss << "  .filled { fill: #ccc; }\n";
    ss << "  .title { font-family: Arial, sans-serif; font-size: 14px; font-weight: bold; }\n";
    ss << "  .desc { font-family: Arial, sans-serif; font-size: 10px; }\n";
    ss << "</style>\n";
    
    ss << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    
    // Title
    ss << "<text x=\"" << width/2 << "\" y=\"30\" text-anchor=\"middle\" class=\"title\" style=\"font-size: 16px;\">";
    ss << "The Proliferation of the System</text>\n";
    
    double y_start = 60;
    double y_step = (height - 80) / 10.0;
    double icon_size = std::min(y_step * 0.7, 60.0);
    double icon_x = 80;
    double text_x = 160;
    
    // System 1: Cross/Plus
    double y = y_start;
    ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
    ss << "  <line x1=\"-15\" y1=\"0\" x2=\"15\" y2=\"0\" class=\"system\"/>\n";
    ss << "  <line x1=\"0\" y1=\"-15\" x2=\"0\" y2=\"15\" class=\"system\"/>\n";
    ss << "</g>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM 1</text>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">Active interface between subjective &amp; objective</text>\n";
    
    // System 2: Double lines
    y += y_step;
    ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
    ss << "  <line x1=\"-10\" y1=\"-15\" x2=\"-10\" y2=\"15\" class=\"system\"/>\n";
    ss << "  <line x1=\"10\" y1=\"-15\" x2=\"10\" y2=\"15\" class=\"system\"/>\n";
    ss << "  <line x1=\"-15\" y1=\"-5\" x2=\"-5\" y2=\"-5\" class=\"system\"/>\n";
    ss << "  <line x1=\"5\" y1=\"5\" x2=\"15\" y2=\"5\" class=\"system\"/>\n";
    ss << "</g>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM 2</text>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">Objective &amp; subjective modes</text>\n";
    
    // System 3: Triangle
    y += y_step;
    ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
    ss << "  <polygon points=\"0,-18 -16,12 16,12\" class=\"system\"/>\n";
    ss << "</g>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM 3</text>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">The primary activity &amp; the cosmic movie</text>\n";
    
    // System 4: Triangle in circle
    y += y_step;
    ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
    ss << "  <circle cx=\"0\" cy=\"0\" r=\"20\" class=\"system\"/>\n";
    ss << "  <polygon points=\"0,-18 -16,12 16,12\" class=\"system\"/>\n";
    ss << "</g>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM 4</text>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">The primary creative process &amp; the enneagram</text>\n";
    
    // System 5: Two overlapping triangles in circles
    y += y_step;
    ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
    ss << "  <circle cx=\"-8\" cy=\"0\" r=\"18\" class=\"system\"/>\n";
    ss << "  <polygon points=\"-8,-16 -22,10 6,10\" class=\"system\"/>\n";
    ss << "  <circle cx=\"8\" cy=\"0\" r=\"18\" class=\"system\"/>\n";
    ss << "  <polygon points=\"8,-16 -6,10 22,10\" class=\"system filled\"/>\n";
    ss << "</g>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM 5</text>\n";
    ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">Complementary objective &amp; subjective enneagrams</text>\n";
    
    // System 6-10: Increasingly complex nested structures
    for (int sys = 6; sys <= 10; ++sys) {
        y += y_step;
        ss << "<g transform=\"translate(" << icon_x << "," << y << ")\">\n";
        
        // Draw nested triangular structure
        double base_size = 25;
        int layers = sys - 4;
        for (int layer = 0; layer < std::min(layers, 4); ++layer) {
            double size = base_size * (1.0 - layer * 0.25);
            double offset_y = layer * 8;
            ss << "  <polygon points=\"0," << (-size + offset_y) 
               << " " << (-size*0.866) << "," << (size*0.5 + offset_y)
               << " " << (size*0.866) << "," << (size*0.5 + offset_y) 
               << "\" class=\"system" << (layer % 2 == 1 ? " filled" : "") << "\"/>\n";
        }
        
        ss << "</g>\n";
        ss << "<text x=\"" << text_x << "\" y=\"" << y-5 << "\" class=\"title\">SYSTEM " << sys << "</text>\n";
        
        std::string desc;
        switch (sys) {
            case 6: desc = "Primary activity of enneagrams"; break;
            case 7: desc = "Enneagram of enneagrams"; break;
            case 8: desc = "Objective and subjective enneagrams of enneagrams"; break;
            case 9: desc = "A primary activity, each term an enneagram of enneagrams"; break;
            case 10: desc = "An enneagram, each term an enneagram of enneagrams"; break;
        }
        ss << "<text x=\"" << text_x << "\" y=\"" << y+10 << "\" class=\"desc\">" << desc << "</text>\n";
    }
    
    ss << "</svg>\n";
    
    return ss.str();
}

} // namespace svg

} // namespace geometry
} // namespace cosmic
