/**
 * @file system1_system2_demo.cpp
 * @brief Demonstration of System 1 and System 2 models with Fisherman's Guide insights
 * 
 * This example demonstrates the detailed models of System 1 (Universal Wholeness)
 * and System 2 (Perceptive Wholeness) based on Robert Campbell's Fisherman's Guide
 * to the Cosmic Order.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "cosmic/system1.hpp"
#include "cosmic/system2.hpp"

using namespace cosmic;

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(70, '=') << "\n\n";
}

void printSubHeader(const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(50, '-') << "\n";
}

void demonstrateSystem1() {
    printSeparator("SYSTEM 1: UNIVERSAL WHOLENESS");
    
    std::cout << system1::System1::description() << "\n\n";
    
    // Create System 1
    system1::System1 sys1;
    
    printSubHeader("OEIS Alignment");
    std::cout << "  Terms: " << sys1.termCount() << " (OEIS A000081(2) = 1)\n";
    std::cout << "  Clusters: " << sys1.clusterCount() << " (OEIS A000055(2) = 1)\n";
    std::cout << "  Nodes: " << sys1.nodeCount() << "\n";
    std::cout << "  Canonical: " << sys1.canonical() << "\n";
    
    printSubHeader("Components (from Figure 11)");
    std::cout << "  Universal Center (Light): " << sys1.center().lightSymbol() 
              << " = " << sys1.center().intensity() << "\n";
    std::cout << "  Universal Periphery (Darkness): " << sys1.periphery().darknessSymbol() 
              << " = " << sys1.periphery().intensity() << "\n";
    std::cout << "  Light-Darkness Gradient: " << sys1.lightDarknessGradient() << "\n";
    
    printSubHeader("Active Interface");
    std::cout << "  Efflux Rate (outward to sky): " << sys1.interface().effluxRate() << "\n";
    std::cout << "  Reflux Rate (inward return): " << sys1.interface().refluxRate() << "\n";
    std::cout << "  Communicative Balance: " << sys1.interface().communicativeBalance() << "\n";
    std::cout << "  Is Equilibrium: " << (sys1.interface().isEquilibrium() ? "Yes" : "No") << "\n";
    
    printSubHeader("Perspectives");
    std::cout << "  Passive: From Darkness looking in (sphere view)\n";
    std::cout << "  Active: From Light looking out (cone/funnel view)\n";
    
    printSubHeader("The Loon Analogy");
    std::cout << system1::LoonAnalogy::description() << "\n\n";
    
    system1::LoonAnalogy loon;
    std::cout << "Simulating loon communication with sky (10 steps):\n";
    std::cout << std::setw(8) << "Step" << std::setw(20) << "Comm. Balance" << "\n";
    std::cout << std::string(28, '-') << "\n";
    for (int i = 0; i < 10; ++i) {
        loon.communicate(1.0);
        std::cout << std::setw(8) << i+1 
                  << std::setw(20) << std::fixed << std::setprecision(4) 
                  << loon.communicativeBalance() << "\n";
    }
    
    // Generate SVG
    std::ofstream svg_file("system1_diagram.svg");
    svg_file << sys1.toSVG();
    svg_file.close();
    std::cout << "\nGenerated: system1_diagram.svg\n";
}

void demonstrateSystem2() {
    printSeparator("SYSTEM 2: PERCEPTIVE WHOLENESS");
    
    std::cout << system2::System2::description() << "\n\n";
    
    // Create System 2
    system2::System2 sys2;
    
    printSubHeader("OEIS Alignment");
    std::cout << "  Terms: " << sys2.termCount() << " (OEIS A000081(3) = 2)\n";
    std::cout << "  Clusters: " << sys2.clusterCount() << " (OEIS A000055(3) = 1)\n";
    std::cout << "  Nodes: " << sys2.nodeCount() << "\n";
    auto canonical = sys2.canonicalTerms();
    std::cout << "  Canonical Terms: " << canonical[0] << " (serial), " 
              << canonical[1] << " (parallel)\n";
    
    printSubHeader("Two Centers (from Figures 12, 13, 19)");
    std::cout << "  Universal Center (1): " << sys2.universalCenter().lightSymbol() 
              << " = " << sys2.universalCenter().intensity() << "\n";
    std::cout << "  Particular Center (2): " << sys2.particularCenter().lightSymbol() 
              << " = " << sys2.particularCenter().intensity() << "\n";
    std::cout << "  Polarity (U-P): " << sys2.polarity() << "\n";
    
    printSubHeader("Relational Wholes");
    std::cout << "  " << sys2.universalCenter().relationalWhole().symbol() 
              << " (from Center 1): strength = " 
              << sys2.universalCenter().relationalWhole().identityStrength() << "\n";
    std::cout << "  " << sys2.particularCenter().relationalWhole().symbol() 
              << " (from Center 2): strength = " 
              << sys2.particularCenter().relationalWhole().identityStrength() << "\n";
    
    printSubHeader("Perceptual Transposition");
    std::cout << system2::PerceptualTransposition::description() << "\n";
    
    printSubHeader("Coalescence");
    std::cout << system2::Coalescence::description() << "\n";
    
    printSubHeader("Mode Alternation Simulation");
    std::cout << "Simulating Perceptual Transposition (20 steps):\n\n";
    std::cout << std::setw(6) << "Step" 
              << std::setw(12) << "Mode" 
              << std::setw(12) << "Obj.Wt" 
              << std::setw(12) << "Coal.Str"
              << std::setw(15) << "EM Freq" << "\n";
    std::cout << std::string(57, '-') << "\n";
    
    sys2 = system2::System2(0.6, 0.4, 0.5);  // Reset with specific parameters
    for (int i = 0; i < 20; ++i) {
        sys2.step(1.0);
        std::cout << std::setw(6) << i+1 
                  << std::setw(12) << system2::utils::modeToString(sys2.currentMode())
                  << std::setw(12) << std::fixed << std::setprecision(3) 
                  << sys2.transposition().objectiveWeight()
                  << std::setw(12) << sys2.coalescence().strength()
                  << std::setw(15) << sys2.electromagneticFrequency() << "\n";
    }
    
    printSubHeader("Electromagnetic Connection");
    std::cout << "From Fisherman's Guide: The wide variance of the electromagnetic\n";
    std::cout << "spectrum is determined by the relative partitioning characteristics\n";
    std::cout << "of the two centers of System 2.\n\n";
    std::cout << "  Current EM Frequency: " << sys2.electromagneticFrequency() << "\n";
    std::cout << "  Current EM Wavelength: " << sys2.electromagneticWavelength() << "\n";
    
    // Generate SVG for both modes
    sys2.reset();
    sys2.step(0.5);  // Objective mode
    std::ofstream svg_obj("system2_objective.svg");
    svg_obj << sys2.toSVG();
    svg_obj.close();
    std::cout << "\nGenerated: system2_objective.svg\n";
    
    sys2.step(3.0);  // Move to subjective mode
    std::ofstream svg_subj("system2_subjective.svg");
    svg_subj << sys2.toSVG();
    svg_subj.close();
    std::cout << "Generated: system2_subjective.svg\n";
}

void demonstrateFlashlightAnalogy() {
    printSeparator("FLASHLIGHT ANALOGY");
    
    std::cout << system2::FlashlightAnalogy::description() << "\n\n";
    
    system2::FlashlightAnalogy flashlights(1000);
    
    std::cout << "Simulating 1000 flashlights (50 steps):\n\n";
    std::cout << std::setw(8) << "Step" << std::setw(22) << "Total Light Output" << "\n";
    std::cout << std::string(30, '-') << "\n";
    
    for (int i = 0; i < 50; ++i) {
        flashlights.step(1.0);
        if (i % 5 == 0) {
            std::cout << std::setw(8) << i+1 
                      << std::setw(22) << std::fixed << std::setprecision(4) 
                      << flashlights.totalLightOutput() << "\n";
        }
    }
    
    std::cout << "\nNote: Despite individual flashlights flickering on and off,\n";
    std::cout << "the total light output remains relatively constant - demonstrating\n";
    std::cout << "how System 2's alternating modes are subsumed within System 1.\n";
}

void demonstrateThreeness() {
    printSeparator("THE EMERGENCE OF THREENESS");
    
    std::cout << "System 2 introduces 'threeness' through:\n";
    std::cout << "  1. Objective mode (sequential, expressive)\n";
    std::cout << "  2. Subjective mode (simultaneous, regenerative)\n";
    std::cout << "  3. Active transformation between them\n\n";
    
    std::cout << "This threeness requires System 3 to elaborate further.\n";
    std::cout << "System 3 will introduce the four terms (A000081(4) = 4):\n";
    std::cout << "  - Universal Discretion (Objective-Universal)\n";
    std::cout << "  - Universal Means (Subjective-Universal)\n";
    std::cout << "  - Particular Goals (Objective-Particular)\n";
    std::cout << "  - Particular Consequence (Subjective-Particular)\n\n";
    
    system2::System2 sys2(0.5, 0.5, 0.3);
    
    std::cout << "Threeness measure during transposition cycle:\n\n";
    std::cout << std::setw(8) << "Step" 
              << std::setw(15) << "Obj.Weight" 
              << std::setw(15) << "Threeness" << "\n";
    std::cout << std::string(38, '-') << "\n";
    
    for (int i = 0; i < 15; ++i) {
        sys2.step(1.0);
        double threeness = system2::utils::threeness(sys2);
        std::cout << std::setw(8) << i+1 
                  << std::setw(15) << std::fixed << std::setprecision(4) 
                  << sys2.transposition().objectiveWeight()
                  << std::setw(15) << threeness << "\n";
    }
    
    std::cout << "\nThreeness peaks at 1.0 when the system is mid-transition\n";
    std::cout << "(objective weight = 0.5), representing maximum transformation activity.\n";
}

void demonstrateSubsumption() {
    printSeparator("SUBSUMPTION: SYSTEM 2 CONTAINS SYSTEM 1");
    
    std::cout << "From Fisherman's Guide:\n";
    std::cout << "\"System 2 incorporates universal wholeness (System 1) and\n";
    std::cout << "perceptive wholeness (System 2). All terms in higher systems\n";
    std::cout << "are constituted of interacting centers or wholes.\"\n\n";
    
    std::cout << "Key principles:\n";
    std::cout << "  - Each higher system incorporates all properties of lower systems\n";
    std::cout << "  - All terms transform into one another through perceptual transpositions\n";
    std::cout << "  - Each higher system exhibits a master interface or axis\n";
    std::cout << "  - Systems 1 and 2 transcend what we perceive as space and time\n\n";
    
    std::cout << "OEIS Alignment:\n\n";
    std::cout << std::setw(10) << "System" 
              << std::setw(10) << "Nodes" 
              << std::setw(10) << "Terms" 
              << std::setw(12) << "Clusters" << "\n";
    std::cout << std::string(42, '-') << "\n";
    std::cout << std::setw(10) << "0" << std::setw(10) << "1" 
              << std::setw(10) << "1" << std::setw(12) << "1" << "\n";
    std::cout << std::setw(10) << "1" << std::setw(10) << "2" 
              << std::setw(10) << "1" << std::setw(12) << "1" << "\n";
    std::cout << std::setw(10) << "2" << std::setw(10) << "3" 
              << std::setw(10) << "2" << std::setw(12) << "1" << "\n";
    std::cout << std::setw(10) << "3" << std::setw(10) << "4" 
              << std::setw(10) << "4" << std::setw(12) << "2" << "\n";
    std::cout << std::setw(10) << "4" << std::setw(10) << "5" 
              << std::setw(10) << "9" << std::setw(12) << "3" << "\n";
}

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     COSMIC SYSTEM LIBRARY - System 1 & System 2 Demonstration        ║\n";
    std::cout << "║     Based on Robert Campbell's Fisherman's Guide to the Cosmic Order ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════╝\n";
    
    demonstrateSystem1();
    demonstrateSystem2();
    demonstrateFlashlightAnalogy();
    demonstrateThreeness();
    demonstrateSubsumption();
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  Demonstration complete. SVG diagrams generated.\n";
    std::cout << std::string(70, '=') << "\n";
    
    return 0;
}
