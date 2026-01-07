/**
 * @file basic_usage.cpp
 * @brief Basic usage example for the Cosmic System Library
 * 
 * This example demonstrates:
 * - Creating the System hierarchy
 * - Navigating through systems
 * - Working with triadic terms
 * - Using the enneagram structure
 * - Generating visualizations
 */

#include <iostream>
#include <fstream>
#include "cosmic/cosmic.hpp"

using namespace cosmic;

void printSystemInfo(const System& sys) {
    std::cout << "\n=== " << sys.name() << " ===" << std::endl;
    std::cout << "Description: " << sys.description() << std::endl;
    std::cout << "Term count: " << sys.termCount() << std::endl;
    std::cout << "Orientation: " << util::toString(sys.primaryInterface().orientation()) << std::endl;
}

void demonstrateTriad(const System& sys) {
    auto triad = sys.triad();
    if (!triad) {
        std::cout << "System " << sys.level() << " does not have a triadic structure." << std::endl;
        return;
    }
    
    std::cout << "\nTriadic Structure:" << std::endl;
    for (const auto& term : *triad) {
        if (term) {
            std::cout << "  - " << term->name();
            if (term->triadicType()) {
                std::cout << " (" << util::toString(*term->triadicType()) << ")";
            }
            std::cout << std::endl;
            
            // Show nested terms
            for (const auto& sub : term->subTerms()) {
                std::cout << "      * " << sub->name() << std::endl;
            }
        }
    }
}

void demonstrateEnneagram(const System& sys) {
    auto ennea = sys.enneagram();
    if (!ennea) {
        std::cout << "System " << sys.level() << " does not have an enneagram." << std::endl;
        return;
    }
    
    std::cout << "\nEnneagram: " << ennea->name() << std::endl;
    
    // Show triangle positions (3, 6, 9)
    std::cout << "Triangle (Law of Three):" << std::endl;
    auto tri = ennea->triad();
    for (int i = 0; i < 3; ++i) {
        int pos = (i == 0) ? 3 : (i == 1) ? 6 : 9;
        if (tri[i]) {
            std::cout << "  Position " << pos << ": " << tri[i]->name() << std::endl;
        }
    }
    
    // Show process positions (1, 4, 2, 8, 5, 7)
    std::cout << "Hexad (Law of Seven):" << std::endl;
    auto proc = ennea->process();
    int positions[] = {1, 2, 4, 5, 7, 8};
    for (int i = 0; i < 6; ++i) {
        if (proc[i]) {
            std::cout << "  Position " << positions[i] << ": " << proc[i]->name() << std::endl;
        }
    }
}

void demonstrateCreativeProcess() {
    std::cout << "\n=== Creative Process Demonstration ===" << std::endl;
    
    ops::CreativeProcess process;
    
    while (!process.isComplete()) {
        std::cout << "\nPosition " << process.position() << ": " 
                  << process.stateDescription() << std::endl;
        
        if (process.needsShock()) {
            std::cout << "  -> Applying external shock..." << std::endl;
            process.applyShock();
        }
        
        process.advance();
    }
    
    std::cout << "\nProcess complete at position " << process.position() << std::endl;
}

void generateVisualization() {
    std::cout << "\n=== Generating Visualizations ===" << std::endl;
    
    // Generate basic enneagram SVG
    geometry::EnneagramGeometry ennea(geometry::Circle({0, 0}, 1.0));
    std::string svg = geometry::svg::enneagramSVG(ennea, 400, 400, "#333", "none");
    
    std::ofstream file("enneagram.svg");
    if (file.is_open()) {
        file << svg;
        file.close();
        std::cout << "Generated: enneagram.svg" << std::endl;
    }
    
    // Generate nested enneagram SVG
    geometry::NestedEnneagramGeometry nested(1, geometry::Circle({0, 0}, 1.0));
    std::string nested_svg = geometry::svg::nestedEnneagramSVG(nested, 600, 600);
    
    std::ofstream nested_file("nested_enneagram.svg");
    if (nested_file.is_open()) {
        nested_file << nested_svg;
        nested_file.close();
        std::cout << "Generated: nested_enneagram.svg" << std::endl;
    }
    
    // Generate system hierarchy SVG
    std::string hierarchy_svg = geometry::svg::systemHierarchySVG(400, 1200);
    
    std::ofstream hierarchy_file("system_hierarchy.svg");
    if (hierarchy_file.is_open()) {
        hierarchy_file << hierarchy_svg;
        hierarchy_file.close();
        std::cout << "Generated: system_hierarchy.svg" << std::endl;
    }
}

void demonstrateNavigation() {
    std::cout << "\n=== Navigation Demonstration ===" << std::endl;
    
    auto hierarchy = System::createHierarchy();
    ops::SystemNavigator nav(hierarchy);
    
    std::cout << "Starting at: " << nav.current()->name() << std::endl;
    
    // Navigate through the hierarchy
    for (int level = 1; level <= 10; ++level) {
        if (nav.goToLevel(level)) {
            std::cout << "Navigated to: " << nav.current()->name() 
                      << " (terms: " << nav.current()->termCount() << ")" << std::endl;
        }
    }
}

void demonstrateSerialization() {
    std::cout << "\n=== Serialization Demonstration ===" << std::endl;
    
    auto hierarchy = System::createHierarchy();
    auto system3 = System::getSystem(hierarchy, 3);
    
    if (system3) {
        // JSON output
        std::string json = ops::Serializer::toJSON(*system3);
        std::cout << "\nSystem 3 as JSON:" << std::endl;
        std::cout << json << std::endl;
        
        // DOT output for Graphviz
        std::string dot = ops::Serializer::toDOT(*system3);
        std::ofstream dot_file("system3.dot");
        if (dot_file.is_open()) {
            dot_file << dot;
            dot_file.close();
            std::cout << "\nGenerated: system3.dot (for Graphviz)" << std::endl;
        }
    }
    
    // Enneagram DOT
    auto system4 = System::getSystem(hierarchy, 4);
    if (system4 && system4->enneagram()) {
        std::string ennea_dot = ops::Serializer::toDOT(*system4->enneagram());
        std::ofstream ennea_file("enneagram.dot");
        if (ennea_file.is_open()) {
            ennea_file << ennea_dot;
            ennea_file.close();
            std::cout << "Generated: enneagram.dot (for Graphviz)" << std::endl;
        }
    }
}

int main() {
    std::cout << "Cosmic System Library v" << Version::string() << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Create the complete hierarchy
    std::cout << "\nCreating System hierarchy..." << std::endl;
    auto hierarchy = System::createHierarchy();
    
    // Demonstrate each system level
    for (int level = 1; level <= 5; ++level) {
        auto sys = System::getSystem(hierarchy, level);
        if (sys) {
            printSystemInfo(*sys);
            
            if (level >= 3) {
                demonstrateTriad(*sys);
            }
            
            if (level >= 4) {
                demonstrateEnneagram(*sys);
            }
        }
    }
    
    // Demonstrate the creative process
    demonstrateCreativeProcess();
    
    // Demonstrate navigation
    demonstrateNavigation();
    
    // Demonstrate serialization
    demonstrateSerialization();
    
    // Generate visualizations
    generateVisualization();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Example complete!" << std::endl;
    
    return 0;
}
