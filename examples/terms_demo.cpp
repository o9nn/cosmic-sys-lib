/**
 * @file terms_demo.cpp
 * @brief Demonstration of the complete system terms from Fisherman's Guide
 * 
 * Updated to use OEIS A000081/A000055 aligned term counts.
 */

#include <cosmic/cosmic.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace cosmic;
using namespace cosmic::terms;

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(70, '=') << "\n\n";
}

// Helper function to generate nested term name
std::string makeNestedTermName(const std::vector<int>& positions) {
    auto sys4terms = getSystem4Terms();
    std::stringstream ss;
    
    for (size_t i = 0; i < positions.size(); ++i) {
        int pos = positions[i];
        for (const auto& term : sys4terms) {
            if (term.position == pos) {
                if (i > 0) ss << ".";
                ss << term.shortName;
                break;
            }
        }
    }
    
    return ss.str();
}

// Helper function to generate nested term description
std::string makeNestedTermDescription(const std::vector<int>& positions) {
    auto sys4terms = getSystem4Terms();
    std::stringstream ss;
    
    for (size_t i = 0; i < positions.size(); ++i) {
        int pos = positions[i];
        for (const auto& term : sys4terms) {
            if (term.position == pos) {
                if (i > 0) ss << " within ";
                ss << term.name;
                break;
            }
        }
    }
    
    return ss.str();
}

void demonstrateSystemHierarchy() {
    printSeparator("SYSTEM HIERARCHY (OEIS A000081/A000055)");
    
    auto definitions = getSystemDefinitions();
    
    std::cout << std::left << std::setw(10) << "System" 
              << std::setw(10) << "Terms"
              << std::setw(12) << "Clusters"
              << std::setw(8) << "Nodes"
              << "Description\n";
    std::cout << std::string(70, '-') << "\n";
    
    for (const auto& def : definitions) {
        std::cout << std::setw(10) << def.level
                  << std::setw(10) << def.termCount
                  << std::setw(12) << def.clusterCount
                  << std::setw(8) << def.nodeCount
                  << def.name << "\n";
    }
    
    std::cout << "\nNote: Terms = A000081(n+1), Clusters = A000055(n+1)\n";
}

void demonstrateSystem3Terms() {
    printSeparator("SYSTEM 3 - FOUR FUNDAMENTAL TERMS (2 Clusters)");
    
    auto terms = getSystem3Terms();
    
    std::cout << "System 3 has 4 terms grouped into 2 clusters:\n\n";
    
    std::cout << "CLUSTER 0 - Universal Terms:\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& term : terms) {
        if (term.cluster == 0) {
            std::cout << "  " << term.id << ". " << term.name << "\n";
            std::cout << "     Orientation: " << orientationToString(term.orientation);
            std::cout << " | " << (term.isUniversal ? "Universal" : "Particular") << "\n";
            std::cout << "     " << term.description << "\n\n";
        }
    }
    
    std::cout << "CLUSTER 1 - Particular Terms:\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& term : terms) {
        if (term.cluster == 1) {
            std::cout << "  " << term.id << ". " << term.name << "\n";
            std::cout << "     Orientation: " << orientationToString(term.orientation);
            std::cout << " | " << (term.isUniversal ? "Universal" : "Particular") << "\n";
            std::cout << "     " << term.description << "\n\n";
        }
    }
}

void demonstrateSystem4Terms() {
    printSeparator("SYSTEM 4 - NINE ENNEAGRAM TERMS (3 Clusters)");
    
    auto terms = getSystem4Terms();
    
    std::cout << "System 4 has 9 terms grouped into 3 clusters:\n\n";
    
    std::cout << "CLUSTER 0 - Triangle Terms (Law of Three):\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& term : terms) {
        if (term.cluster == 0) {
            std::cout << "  Position " << term.position << " (" << term.shortName << "): "
                      << term.name << "\n";
            std::cout << "    " << term.description << "\n";
        }
    }
    
    std::cout << "\nCLUSTER 1 - Upper Hexad (Initiating):\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& term : terms) {
        if (term.cluster == 1) {
            std::cout << "  Position " << term.position << " (" << term.shortName << "): "
                      << term.name << "\n";
            std::cout << "    " << term.description << "\n";
        }
    }
    
    std::cout << "\nCLUSTER 2 - Lower Hexad (Completing):\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& term : terms) {
        if (term.cluster == 2) {
            std::cout << "  Position " << term.position << " (" << term.shortName << "): "
                      << term.name << "\n";
            std::cout << "    " << term.description << "\n";
        }
    }
    
    // Show the process sequence
    std::cout << "\nCREATIVE PROCESS SEQUENCE:\n";
    auto sequence = getCreativeProcessSequence();
    std::cout << "  ";
    for (size_t i = 0; i < sequence.size(); ++i) {
        if (i > 0) std::cout << " -> ";
        std::cout << sequence[i];
        if (isShockPoint(sequence[i])) std::cout << "*";
    }
    std::cout << "\n  (* = shock point requiring external input)\n";
}

void demonstrateCosmicMovie() {
    printSeparator("COSMIC MOVIE (System 3 Application)");
    
    auto terms = getCosmicMovieTerms();
    
    std::cout << "The Cosmic Movie maps System 3's triadic structure to cosmic scales:\n\n";
    
    CosmicLevel currentLevel = static_cast<CosmicLevel>(-1);
    for (const auto& term : terms) {
        if (term.level != currentLevel) {
            currentLevel = term.level;
            std::cout << cosmicLevelToString(currentLevel) << ":\n";
            std::cout << std::string(50, '-') << "\n";
        }
        
        std::cout << "  " << std::left << std::setw(10) 
                  << triadicTypeToString(term.type) << ": ";
        std::cout << term.description << "\n";
    }
}

void demonstrateBiosphereHierarchy() {
    printSeparator("BIOSPHERE HIERARCHY");
    
    auto terms = getBiosphereTerms();
    
    std::cout << "The biosphere hierarchy maps triadic structure to life forms:\n\n";
    
    BiosphereTier currentTier = static_cast<BiosphereTier>(-1);
    for (const auto& term : terms) {
        if (term.tier != currentTier) {
            currentTier = term.tier;
            std::cout << biosphereTierToString(currentTier) << ":\n";
            std::cout << std::string(50, '-') << "\n";
        }
        
        std::cout << "  " << std::left << std::setw(10) 
                  << triadicTypeToString(term.type) << ": ";
        std::cout << term.description << "\n";
    }
}

void demonstrateNestedTermGeneration() {
    printSeparator("NESTED TERM GENERATION (Higher Systems)");
    
    std::cout << "System 7 Example (Enneagram of Enneagrams - 115 terms, 23 clusters):\n";
    std::cout << std::string(50, '-') << "\n";
    
    // Generate some example nested terms
    std::vector<std::vector<int>> examples = {
        {1, 1},  // Position 1.1
        {1, 4},  // Position 1.4
        {3, 6},  // Position 3.6
        {9, 9},  // Position 9.9
    };
    
    for (const auto& positions : examples) {
        std::string name = makeNestedTermName(positions);
        std::string desc = makeNestedTermDescription(positions);
        
        std::cout << "  Address " << positions[0] << "." << positions[1] 
                  << " (" << name << "):\n";
        std::cout << "    " << desc << "\n";
    }
    
    std::cout << "\nSystem 9 Example (719 terms, 106 clusters - Three-level nesting):\n";
    std::cout << std::string(50, '-') << "\n";
    
    std::vector<std::vector<int>> examples3 = {
        {1, 4, 2},
        {9, 3, 6},
        {5, 7, 8},
    };
    
    for (const auto& positions : examples3) {
        std::string name = makeNestedTermName(positions);
        std::string desc = makeNestedTermDescription(positions);
        
        std::cout << "  Address " << positions[0] << "." << positions[1] 
                  << "." << positions[2] << " (" << name << "):\n";
        std::cout << "    " << desc << "\n";
    }
}

int main() {
    std::cout << "\n";
    std::cout << "======================================================================\n";
    std::cout << "  COSMIC SYSTEM LIBRARY - System Terms Demonstration\n";
    std::cout << "  Version " << Version::string() << " (OEIS A000081/A000055 aligned)\n";
    std::cout << "======================================================================\n";
    
    demonstrateSystemHierarchy();
    demonstrateSystem3Terms();
    demonstrateSystem4Terms();
    demonstrateCosmicMovie();
    demonstrateBiosphereHierarchy();
    demonstrateNestedTermGeneration();
    
    printSeparator("END OF DEMONSTRATION");
    
    return 0;
}
