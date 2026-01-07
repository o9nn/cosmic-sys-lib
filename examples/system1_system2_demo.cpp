/**
 * @file system1_system2_demo.cpp
 * @brief Demonstration of detailed System 1 and System 2 models
 * 
 * This example shows:
 * - System 1: Universal Wholeness with efflux/reflux dynamics
 * - System 2: Perceptive Wholeness with mode transformation
 * - OEIS alignment verification
 */

#include <iostream>
#include <iomanip>
#include <cmath>

#include "cosmic/system1.hpp"
#include "cosmic/system2.hpp"

using namespace cosmic;

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void printSubHeader(const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(40, '-') << "\n";
}

void demonstrateSystem1() {
    printHeader("SYSTEM 1: UNIVERSAL WHOLENESS");
    
    std::cout << system1::System1::description() << "\n";
    
    printSubHeader("OEIS Alignment");
    std::cout << "Node count:    " << system1::System1::nodeCount() << " (n=2 for System 1)\n";
    std::cout << "Term count:    " << system1::System1::termCount() << " (A000081(2) = 1)\n";
    std::cout << "Cluster count: " << system1::System1::clusterCount() << " (A000055(2) = 1)\n";
    
    printSubHeader("System 1 Structure");
    
    // Create System 1 with default parameters
    system1::System1 sys1;
    
    std::cout << "Center intensity:    " << sys1.center().intensity() << "\n";
    std::cout << "Periphery bounded:   " << (sys1.periphery().isBounded() ? "Yes" : "No (infinite)") << "\n";
    std::cout << "Interface ratio:     " << sys1.interfaceRatio() << "\n";
    std::cout << "Total energy:        " << sys1.energy() << "\n";
    std::cout << "Canonical form:      " << sys1.canonical() << "\n";
    
    printSubHeader("Active Interface Dynamics");
    
    // Create System 1 with non-equilibrium interface
    system1::System1 sys1_dynamic(1.0, 1.2, 0.8);  // More efflux than reflux
    
    std::cout << "Initial state:\n";
    std::cout << "  Efflux rate:  " << sys1_dynamic.interface().effluxRate() << "\n";
    std::cout << "  Reflux rate:  " << sys1_dynamic.interface().refluxRate() << "\n";
    std::cout << "  Net flow:     " << sys1_dynamic.interface().netFlow() << "\n";
    std::cout << "  Equilibrium:  " << (sys1_dynamic.interface().isEquilibrium() ? "Yes" : "No") << "\n";
    
    std::cout << "\nSimulating 10 time steps:\n";
    std::cout << std::setw(6) << "Step" << std::setw(12) << "Time" 
              << std::setw(15) << "Center Int." << std::setw(15) << "Acc. Efflux"
              << std::setw(15) << "Acc. Reflux" << "\n";
    
    for (int i = 0; i <= 10; ++i) {
        std::cout << std::setw(6) << i 
                  << std::setw(12) << std::fixed << std::setprecision(2) << sys1_dynamic.time()
                  << std::setw(15) << std::setprecision(4) << sys1_dynamic.center().intensity()
                  << std::setw(15) << sys1_dynamic.interface().accumulatedEfflux()
                  << std::setw(15) << sys1_dynamic.interface().accumulatedReflux()
                  << "\n";
        sys1_dynamic.step(1.0);
    }
    
    printSubHeader("Perspective Views");
    
    sys1.setPerspective(system1::Perspective::PASSIVE);
    std::cout << "Passive perspective (outside looking in):\n";
    std::cout << "  We perceive the active interface as the surface of phenomena\n";
    
    sys1.setPerspective(system1::Perspective::ACTIVE);
    std::cout << "Active perspective (inside looking out):\n";
    std::cout << "  We experience the center as the source of all activity\n";
}

void demonstrateSystem2() {
    printHeader("SYSTEM 2: PERCEPTIVE WHOLENESS");
    
    std::cout << system2::System2::description() << "\n";
    
    printSubHeader("OEIS Alignment");
    std::cout << "Node count:    " << system2::System2::nodeCount() << " (n=3 for System 2)\n";
    std::cout << "Term count:    " << system2::System2::termCount() << " (A000081(3) = 2)\n";
    std::cout << "Cluster count: " << system2::System2::clusterCount() << " (A000055(3) = 1)\n";
    
    printSubHeader("System 2 Structure");
    
    // Create System 2 with default parameters
    system2::System2 sys2;
    
    std::cout << "Universal center intensity:  " << sys2.universalCenter().intensity() << "\n";
    std::cout << "Particular center intensity: " << sys2.particularCenter().intensity() << "\n";
    std::cout << "Polarity (U/P):              " << sys2.polarity() << "\n";
    std::cout << "Current mode:                " << system2::utils::modeToString(sys2.currentMode()) << "\n";
    
    printSubHeader("The Two Terms of System 2");
    
    auto canonical_terms = sys2.canonicalTerms();
    std::cout << "Term 0 (Serial/Nested):   " << canonical_terms[0] << "\n";
    std::cout << "  Represents: Universal subsumes Particular\n";
    std::cout << "  Structure:  ( ( ) ) - nested chaining\n\n";
    
    std::cout << "Term 1 (Parallel/Product): " << canonical_terms[1] << "\n";
    std::cout << "  Represents: Universal alongside Particular\n";
    std::cout << "  Structure:  ( ) ( ) - replicated product\n";
    
    printSubHeader("Mode Transformation Dynamics");
    
    // Create System 2 with faster transformation
    system2::System2 sys2_dynamic(0.5, 0.5, 0.5);
    
    std::cout << "Simulating mode transformation cycle:\n\n";
    std::cout << std::setw(6) << "Step" << std::setw(10) << "Phase" 
              << std::setw(12) << "Obj Weight" << std::setw(12) << "Subj Weight"
              << std::setw(12) << "Mode" << std::setw(12) << "Threeness" << "\n";
    std::cout << std::string(64, '-') << "\n";
    
    for (int i = 0; i <= 12; ++i) {
        double phase = sys2_dynamic.transformation().phase();
        double obj = sys2_dynamic.transformation().objectiveWeight();
        double subj = sys2_dynamic.transformation().subjectiveWeight();
        double three = system2::utils::threeness(sys2_dynamic);
        
        std::cout << std::setw(6) << i 
                  << std::setw(10) << std::fixed << std::setprecision(2) << phase
                  << std::setw(12) << std::setprecision(3) << obj
                  << std::setw(12) << subj
                  << std::setw(12) << system2::utils::modeToString(sys2_dynamic.currentMode())
                  << std::setw(12) << three
                  << "\n";
        
        sys2_dynamic.step(1.0);
    }
    
    printSubHeader("The Threeness of System 2");
    
    std::cout << "System 2 introduces 'threeness' through:\n";
    std::cout << "  1. Objective mode (outside looking in)\n";
    std::cout << "  2. Subjective mode (inside looking out)\n";
    std::cout << "  3. Active transformation between them\n\n";
    std::cout << "This threeness requires System 3 to elaborate further.\n";
    
    printSubHeader("Discontinuity Detection");
    
    sys2_dynamic.reset();
    std::cout << "Detecting discontinuity points (mode transitions):\n\n";
    
    int discontinuity_count = 0;
    for (int i = 0; i < 100; ++i) {
        if (system2::utils::isDiscontinuity(sys2_dynamic, 0.05)) {
            std::cout << "  Discontinuity at step " << i 
                      << " (phase = " << std::setprecision(2) 
                      << sys2_dynamic.transformation().phase() << ")\n";
            discontinuity_count++;
        }
        sys2_dynamic.step(0.1);
    }
    std::cout << "\nTotal discontinuities detected: " << discontinuity_count << "\n";
    std::cout << "These represent fundamental discontinuities in space-time projection.\n";
}

void demonstrateHierarchy() {
    printHeader("SYSTEM HIERARCHY: 0 → 1 → 2");
    
    std::cout << "The progression from System 0 to System 2:\n\n";
    
    std::cout << "SYSTEM 0: The Void (1 term, 1 cluster)\n";
    std::cout << "  - Root only, primordial potential\n";
    std::cout << "  - Canonical: ()\n";
    std::cout << "  - The unmarked state from which all content emerges\n\n";
    
    std::cout << "SYSTEM 1: Universal Wholeness (1 term, 1 cluster)\n";
    std::cout << "  - Single active interface between inside and outside\n";
    std::cout << "  - Canonical: (())\n";
    std::cout << "  - All phenomena share universal inside and outside\n\n";
    
    std::cout << "SYSTEM 2: Perceptive Wholeness (2 terms, 1 cluster)\n";
    std::cout << "  - Two centers: Universal (unique) and Particular (many)\n";
    std::cout << "  - Canonical: ((())) and (()())\n";
    std::cout << "  - Introduces objective/subjective duality\n\n";
    
    std::cout << "OEIS Alignment Summary:\n";
    std::cout << std::setw(10) << "System" << std::setw(10) << "Nodes" 
              << std::setw(10) << "Terms" << std::setw(12) << "Clusters" << "\n";
    std::cout << std::string(42, '-') << "\n";
    std::cout << std::setw(10) << "0" << std::setw(10) << "1" 
              << std::setw(10) << "1" << std::setw(12) << "1" << "\n";
    std::cout << std::setw(10) << "1" << std::setw(10) << "2" 
              << std::setw(10) << "1" << std::setw(12) << "1" << "\n";
    std::cout << std::setw(10) << "2" << std::setw(10) << "3" 
              << std::setw(10) << "2" << std::setw(12) << "1" << "\n";
}

int main() {
    std::cout << "COSMIC SYSTEM LIBRARY - System 1 & System 2 Demonstration\n";
    std::cout << "Based on Robert Campbell's 'Physics & Cosmic Order II'\n";
    
    demonstrateSystem1();
    demonstrateSystem2();
    demonstrateHierarchy();
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Demonstration complete.\n";
    
    return 0;
}
