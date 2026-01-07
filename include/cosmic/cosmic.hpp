/**
 * @file cosmic.hpp
 * @brief Main header for the Cosmic System Library
 * 
 * This library implements the nested enneagram system hierarchy (Systems 0-10)
 * based on Robert Campbell's theoretical framework of the Cosmic Order.
 * 
 * Term counts follow OEIS A000081 (number of rooted trees with n unlabeled nodes),
 * where System n has A000081(n+1) terms. Terms are grouped into clusters via the
 * flip transform, with cluster counts following OEIS A000055 (number of unrooted trees).
 * 
 * @author Based on Robert Campbell's theoretical framework
 * @see http://cosmic-mindreach.com
 * 
 * @example
 * @code
 * #include <cosmic/cosmic.hpp>
 * 
 * int main() {
 *     // Create the complete System 0-10 hierarchy
 *     auto hierarchy = cosmic::System::createHierarchy();
 *     
 *     // Navigate to System 4 (the enneagram)
 *     auto system4 = cosmic::System::getSystem(hierarchy, 4);
 *     
 *     // Check term and cluster counts
 *     std::cout << "Terms: " << system4->termCount() << std::endl;     // 9
 *     std::cout << "Clusters: " << system4->clusterCount() << std::endl; // 3
 *     
 *     // Generate rooted trees for a system level
 *     auto trees = cosmic::trees::SystemTreeMapping::getSystemTrees(4);
 *     auto clusters = cosmic::trees::SystemTreeMapping::getSystemClusters(4);
 *     
 *     return 0;
 * }
 * @endcode
 */

#ifndef COSMIC_HPP
#define COSMIC_HPP

// Core system hierarchy
#include "system.hpp"

// Detailed System 1 and System 2 models
#include "system1.hpp"
#include "system2.hpp"

// Complete system terms definitions
#include "terms.hpp"

// Rooted tree generation and flip transform
#include "trees.hpp"

// Geometric representations
#include "geometry.hpp"

// Operations and transformations
#include "operations.hpp"

/**
 * @namespace cosmic
 * @brief The Cosmic System Library namespace
 * 
 * Contains all classes and functions for working with the
 * nested enneagram system hierarchy.
 */
namespace cosmic {

/**
 * @brief Library version information
 */
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 2;
    static constexpr int PATCH = 0;
    
    static std::string string() {
        return std::to_string(MAJOR) + "." + 
               std::to_string(MINOR) + "." + 
               std::to_string(PATCH);
    }
};

/**
 * @brief Quick reference to the System hierarchy (OEIS A000081/A000055)
 * 
 * | System | Terms (A000081) | Clusters (A000055) | Nodes | Description |
 * |--------|-----------------|-------------------|-------|-------------|
 * | 0      | 1               | 1                 | 0     | The Void - root only |
 * | 1      | 1               | 1                 | 1     | Universal Wholeness |
 * | 2      | 2               | 1                 | 2     | Fundamental Dyad |
 * | 3      | 4               | 2                 | 3     | Primary Activity |
 * | 4      | 9               | 3                 | 4     | Enneagram |
 * | 5      | 20              | 6                 | 5     | Complementary Structures |
 * | 6      | 48              | 11                | 6     | Activity of Enneagrams |
 * | 7      | 115             | 23                | 7     | Enneagram of Enneagrams |
 * | 8      | 286             | 47                | 8     | Nested Complementarity |
 * | 9      | 719             | 106               | 9     | Deep Nesting |
 * | 10     | 1842            | 235               | 10    | Full Recursive Elaboration |
 */

} // namespace cosmic

#endif // COSMIC_HPP
