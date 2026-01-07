/**
 * @file oeis_demo.cpp
 * @brief Demonstrates the OEIS A000081/A000055 alignment of system terms
 * 
 * This example shows how system term counts follow OEIS A000081 (rooted trees)
 * and cluster counts follow OEIS A000055 (unrooted trees via flip transform).
 */

#include <cosmic/cosmic.hpp>
#include <iostream>
#include <iomanip>

using namespace cosmic;
using namespace cosmic::trees;

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(70, '=') << "\n";
}

void printSubHeader(const std::string& title) {
    std::cout << "\n" << std::string(50, '-') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(50, '-') << "\n";
}

void demonstrateOEISSequences() {
    printHeader("OEIS SEQUENCES");
    
    std::cout << "\nA000081 - Number of rooted trees with n unlabeled nodes:\n";
    std::cout << "  ";
    for (int i = 0; i <= 11; ++i) {
        std::cout << cosmic::trees::A000081[i] << " ";
    }
    std::cout << "...\n";
    
    std::cout << "\nA000055 - Number of unrooted trees with n unlabeled nodes:\n";
    std::cout << "  ";
    for (int i = 0; i <= 11; ++i) {
        std::cout << cosmic::trees::A000055[i] << " ";
    }
    std::cout << "...\n";
}

void demonstrateSystemHierarchy() {
    printHeader("SYSTEM HIERARCHY (0-10)");
    
    std::cout << "\n";
    std::cout << std::setw(8) << "System"
              << std::setw(10) << "Terms"
              << std::setw(12) << "Clusters"
              << std::setw(8) << "Nodes"
              << "  Description\n";
    std::cout << std::string(70, '-') << "\n";
    
    auto definitions = cosmic::terms::getSystemDefinitions();
    for (const auto& def : definitions) {
        std::cout << std::setw(8) << def.level
                  << std::setw(10) << def.termCount
                  << std::setw(12) << def.clusterCount
                  << std::setw(8) << def.nodeCount
                  << "  " << def.name << "\n";
    }
    
    std::cout << "\nNote: Terms = A000081(n+1), Clusters = A000055(n+1), Nodes = n\n";
}

void demonstrateTreeGeneration() {
    printHeader("ROOTED TREE GENERATION");
    
    // Generate and display trees for small systems
    for (int level = 0; level <= 5; ++level) {
        auto summary = SystemTreeMapping::getSummary(level);
        
        printSubHeader("System " + std::to_string(level) + 
                      " (" + std::to_string(summary.termCount) + " terms, " +
                      std::to_string(summary.clusterCount) + " clusters)");
        
        std::cout << "Canonical forms of rooted trees:\n";
        for (size_t i = 0; i < summary.treeCanonicals.size(); ++i) {
            std::cout << "  Term " << (i + 1) << ": " 
                      << summary.treeCanonicals[i] << "\n";
        }
        
        std::cout << "\nCluster sizes: ";
        for (size_t size : summary.clusterSizes) {
            std::cout << size << " ";
        }
        std::cout << "\n";
    }
}

void demonstrateFlipTransform() {
    printHeader("FLIP TRANSFORM (Rooted -> Unrooted)");
    
    std::cout << "\nThe flip transform groups rooted trees into equivalence classes\n";
    std::cout << "where two trees are equivalent if one can be re-rooted to match the other.\n";
    
    // Show detailed clustering for System 4 (9 terms, 3 clusters)
    printSubHeader("System 4 Clustering Detail");
    
    auto trees = SystemTreeMapping::getSystemTrees(4);
    auto clusters = SystemTreeMapping::getSystemClusters(4);
    
    std::cout << "Total rooted trees: " << trees.size() << "\n";
    std::cout << "Unrooted equivalence classes: " << clusters.size() << "\n\n";
    
    for (size_t c = 0; c < clusters.size(); ++c) {
        std::cout << "Cluster " << (c + 1) << " (" << clusters[c].size() << " trees):\n";
        for (const auto& tree : clusters[c]) {
            std::cout << "  " << tree.canonical() << "\n";
        }
        std::cout << "\n";
    }
}

void demonstrateVerification() {
    printHeader("VERIFICATION");
    
    std::cout << "\nVerifying that generated counts match OEIS sequences...\n\n";
    
    std::cout << std::setw(8) << "Nodes"
              << std::setw(15) << "Generated"
              << std::setw(15) << "A000081"
              << std::setw(15) << "Clusters"
              << std::setw(15) << "A000055"
              << std::setw(10) << "Status\n";
    std::cout << std::string(78, '-') << "\n";
    
    bool allPassed = true;
    for (int n = 1; n <= 6; ++n) {
        auto trees = RootedTreeGenerator::generate(n);
        auto clusters = FlipTransform::groupIntoClusters(trees);
        
        bool termMatch = (trees.size() == cosmic::trees::A000081[n]);
        bool clusterMatch = (clusters.size() == cosmic::trees::A000055[n]);
        bool passed = termMatch && clusterMatch;
        allPassed = allPassed && passed;
        
        std::cout << std::setw(8) << n
                  << std::setw(15) << trees.size()
                  << std::setw(15) << cosmic::trees::A000081[n]
                  << std::setw(15) << clusters.size()
                  << std::setw(15) << cosmic::trees::A000055[n]
                  << std::setw(10) << (passed ? "PASS" : "FAIL") << "\n";
    }
    
    std::cout << "\n" << (allPassed ? "All verifications PASSED!" : "Some verifications FAILED!") << "\n";
}

void demonstrateSystem3Terms() {
    printHeader("SYSTEM 3 - FOUR FUNDAMENTAL TERMS");
    
    std::cout << "\nSystem 3 has 4 terms grouped into 2 clusters:\n\n";
    
    auto terms = cosmic::terms::getSystem3Terms();
    
    std::cout << std::setw(4) << "ID"
              << std::setw(25) << "Name"
              << std::setw(12) << "Orientation"
              << std::setw(12) << "Universal?"
              << std::setw(10) << "Cluster\n";
    std::cout << std::string(63, '-') << "\n";
    
    for (const auto& term : terms) {
        std::cout << std::setw(4) << term.id
                  << std::setw(25) << term.name
                  << std::setw(12) << cosmic::terms::orientationToString(term.orientation)
                  << std::setw(12) << (term.isUniversal ? "Yes" : "No")
                  << std::setw(10) << term.cluster << "\n";
    }
    
    std::cout << "\nCluster 0: Universal terms (trunk of tree)\n";
    std::cout << "Cluster 1: Particular terms (branches)\n";
}

void demonstrateSystem4Terms() {
    printHeader("SYSTEM 4 - NINE ENNEAGRAM TERMS");
    
    std::cout << "\nSystem 4 has 9 terms grouped into 3 clusters:\n\n";
    
    auto terms = cosmic::terms::getSystem4Terms();
    
    std::cout << std::setw(4) << "Pos"
              << std::setw(25) << "Name"
              << std::setw(8) << "Short"
              << std::setw(10) << "Triangle?"
              << std::setw(10) << "Cluster\n";
    std::cout << std::string(57, '-') << "\n";
    
    for (const auto& term : terms) {
        std::cout << std::setw(4) << term.position
                  << std::setw(25) << term.name
                  << std::setw(8) << term.shortName
                  << std::setw(10) << (term.isTriangle ? "Yes" : "No")
                  << std::setw(10) << term.cluster << "\n";
    }
    
    std::cout << "\nCluster 0: Triangle terms (Law of Three) - positions 3, 6, 9\n";
    std::cout << "Cluster 1: Upper hexad (Initiating) - positions 1, 2, 4\n";
    std::cout << "Cluster 2: Lower hexad (Completing) - positions 5, 7, 8\n";
}

void demonstrateTreeStructures() {
    printHeader("TREE STRUCTURE VISUALIZATION");
    
    std::cout << "\nCanonical tree representations use nested parentheses:\n";
    std::cout << "  () = single node (leaf or root with no children)\n";
    std::cout << "  (()) = root with one child\n";
    std::cout << "(()()) = root with two children\n";
    std::cout << "((()))  = linear chain of 3 nodes\n\n";
    
    printSubHeader("System 3 Trees (4 nodes)");
    auto trees3 = RootedTreeGenerator::generate(4);
    for (size_t i = 0; i < trees3.size(); ++i) {
        std::cout << "Tree " << (i + 1) << ": " << trees3[i].canonical() << "\n";
    }
    
    printSubHeader("System 4 Trees (5 nodes)");
    auto trees4 = RootedTreeGenerator::generate(5);
    for (size_t i = 0; i < trees4.size(); ++i) {
        std::cout << "Tree " << (i + 1) << ": " << trees4[i].canonical() << "\n";
    }
}

int main() {
    std::cout << "COSMIC SYSTEM LIBRARY - OEIS A000081/A000055 DEMONSTRATION\n";
    std::cout << "Version: " << Version::string() << "\n";
    
    demonstrateOEISSequences();
    demonstrateSystemHierarchy();
    demonstrateVerification();
    demonstrateTreeGeneration();
    demonstrateFlipTransform();
    demonstrateSystem3Terms();
    demonstrateSystem4Terms();
    demonstrateTreeStructures();
    
    printHeader("END OF DEMONSTRATION");
    
    return 0;
}
