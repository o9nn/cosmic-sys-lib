# Cosmic System Library

A C++ library implementing the nested enneagram system hierarchy (Systems 0-10) based on Robert Campbell's theoretical framework of the Cosmic Order.

**Version 1.2** - Term counts now aligned with OEIS A000081/A000055 sequences.

## Overview

The System represents a universal methodology for delineating the cosmic order through a progression of discrete sub-systems nested within each other. This library provides a complete implementation of the System hierarchy, from System 0 (The Void) through System 10 (full recursive elaboration), along with geometric representations, rooted tree generation, and operations for working with enneagram structures.

## Mathematical Foundation

Term counts follow **OEIS A000081** (number of rooted trees with n unlabeled nodes), where System n has A000081(n+1) terms. Terms are grouped into clusters via the **flip transform**, with cluster counts following **OEIS A000055** (number of unrooted trees with n nodes).

| System | Terms (A000081) | Clusters (A000055) | Nodes | Description |
|--------|-----------------|-------------------|-------|-------------|
| 0 | 1 | 1 | 0 | The Void - root only, primordial unity |
| 1 | 1 | 1 | 1 | Universal Wholeness - active interface |
| 2 | 2 | 1 | 2 | Fundamental Dyad - objective/subjective |
| 3 | 4 | 2 | 3 | Primary Activity - four terms |
| 4 | 9 | 3 | 4 | Enneagram - nine terms |
| 5 | 20 | 6 | 5 | Complementary Structures |
| 6 | 48 | 11 | 6 | Activity of Enneagrams |
| 7 | 115 | 23 | 7 | Enneagram of Enneagrams |
| 8 | 286 | 47 | 8 | Nested Complementarity |
| 9 | 719 | 106 | 9 | Deep Nesting |
| 10 | 1842 | 235 | 10 | Full Recursive Elaboration |

## Features

The library provides five main components:

**Core System Hierarchy** (`cosmic/system.hpp`): Classes for representing the complete System 0-10 hierarchy, including Terms, Interfaces, Triads, and Enneagrams. The hierarchy can be created and navigated programmatically.

**System Terms** (`cosmic/terms.hpp`): Complete term definitions for all systems, including the four fundamental terms of System 3, the nine enneagram positions of System 4, cosmic movie hierarchy, and biosphere levels.

**Rooted Trees** (`cosmic/trees.hpp`): Algorithms for generating all rooted trees with n nodes (A000081) and grouping them into unrooted equivalence classes via flip transform (A000055). Includes canonical form computation for tree comparison.

**Geometric Representations** (`cosmic/geometry.hpp`): 2D and 3D geometric primitives for visualizing enneagram structures, including circles, triangles, the nine-pointed enneagram figure, tetrahedra, and nested enneagram geometries. SVG export is supported.

**Operations and Transformations** (`cosmic/operations.hpp`): Operations for working with Systems, including orientation transformations, triadic cycle navigation, enneagram process sequences, system navigation, relationship queries, and the creative process simulation.

## Building

The library uses CMake for building:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `COSMIC_BUILD_EXAMPLES` | ON | Build example programs |
| `COSMIC_BUILD_TESTS` | ON | Build test programs |
| `COSMIC_BUILD_SHARED` | OFF | Build shared library instead of static |

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Usage

### Basic Example

```cpp
#include <cosmic/cosmic.hpp>
#include <iostream>

int main() {
    using namespace cosmic;
    
    // Create the complete System 0-10 hierarchy
    auto hierarchy = System::createHierarchy();
    
    // Navigate to System 4 (the enneagram)
    auto system4 = System::getSystem(hierarchy, 4);
    
    // Check term and cluster counts (OEIS aligned)
    std::cout << "Terms: " << system4->termCount() << std::endl;     // 9
    std::cout << "Clusters: " << system4->clusterCount() << std::endl; // 3
    
    return 0;
}
```

### Working with Rooted Trees

```cpp
#include <cosmic/cosmic.hpp>
#include <iostream>

int main() {
    using namespace cosmic::trees;
    
    // Generate all rooted trees for System 4 (5 nodes -> 9 trees)
    auto trees = RootedTreeGenerator::generate(5);
    std::cout << "System 4 has " << trees.size() << " terms" << std::endl;
    
    // Group into unrooted equivalence classes (3 clusters)
    auto clusters = FlipTransform::groupIntoClusters(trees);
    std::cout << "Grouped into " << clusters.size() << " clusters" << std::endl;
    
    // Display canonical forms
    for (const auto& tree : trees) {
        std::cout << tree.canonical() << std::endl;
    }
    
    // Verify against OEIS sequences
    bool valid = FlipTransform::verify(6);  // Verify up to 6 nodes
    
    return 0;
}
```

### System Terms

```cpp
#include <cosmic/cosmic.hpp>
#include <iostream>

int main() {
    using namespace cosmic::terms;
    
    // Get System 3's four fundamental terms
    auto sys3terms = getSystem3Terms();
    for (const auto& term : sys3terms) {
        std::cout << term.name << " (Cluster " << term.cluster << ")" << std::endl;
    }
    
    // Get System 4's nine enneagram terms
    auto sys4terms = getSystem4Terms();
    for (const auto& term : sys4terms) {
        std::cout << "Position " << term.position << ": " << term.name << std::endl;
    }
    
    return 0;
}
```

### Generating Visualizations

```cpp
#include <cosmic/cosmic.hpp>
#include <fstream>

int main() {
    using namespace cosmic::geometry;
    
    // Create an enneagram geometry
    EnneagramGeometry ennea(Circle({0, 0}, 1.0));
    
    // Generate SVG
    std::string svg = svg::enneagramSVG(ennea, 400, 400);
    
    // Save to file
    std::ofstream file("enneagram.svg");
    file << svg;
    
    return 0;
}
```

## API Reference

### Core Classes

**`System`**: Represents a single system level (0-10) in the hierarchy. Contains terms, interfaces, and optional triadic/enneagram structures. Now includes `clusterCount()` and `nodeCount()` methods.

**`Term`**: Represents a term within a system. Terms can have triadic types (Idea, Routine, Form) and can contain nested sub-terms.

**`Interface`**: Represents the interface between systems with an orientation (Objective or Subjective) and active/passive state.

**`Enneagram`**: Represents the nine-pointed enneagram structure with positions 1-9, the triangle (3-6-9), and the hexad process (1-4-2-8-5-7).

### Tree Classes

**`RootedTree`**: A rooted tree structure with canonical form computation for comparison.

**`RootedTreeGenerator`**: Generates all rooted trees with n nodes (A000081).

**`FlipTransform`**: Groups rooted trees into unrooted equivalence classes (A000055).

**`SystemTreeMapping`**: Maps system levels to their rooted tree representations.

### Geometry Classes

**`Point2D` / `Point3D`**: Basic point representations with arithmetic operations.

**`Circle`**: Circle with center and radius, supports point-at-angle calculations.

**`Triangle`**: Triangle with three vertices, supports inscribed equilateral construction.

**`EnneagramGeometry`**: Complete geometric representation of the enneagram figure.

**`TetrahedronGeometry`**: 3D tetrahedron for System 5+ structures.

**`NestedEnneagramGeometry`**: Recursive nesting of enneagrams for System 7+ structures.

### Operations Classes

**`OrientationTransform`**: Transform between objective and subjective orientations.

**`TriadicCycle`**: Navigate the Idea-Routine-Form cycle.

**`EnneagramProcess`**: Work with the enneagram process sequence.

**`SystemNavigator`**: Navigate through the System hierarchy.

**`TermNavigator`**: Navigate through Terms within a System.

**`CreativeProcess`**: Simulate the creative process through the enneagram.

**`Serializer`**: Export Systems, Terms, and Enneagrams to JSON and DOT formats.

## Theoretical Background

The System is based on Robert Campbell's work on the Cosmic Order, which describes a universal methodology for understanding reality through nested hierarchical structures. The key concepts include:

**OEIS A000081 Alignment**: The number of terms at each system level follows the sequence of rooted trees with n unlabeled nodes. This provides a rigorous mathematical foundation for the proliferation of terms through the system hierarchy.

**Flip Transform (A000055)**: Terms are grouped into clusters via the flip transform, which identifies rooted trees that represent the same underlying unrooted structure. This reduces the complexity while preserving essential distinctions.

**The Triad (Law of Three)**: Every process involves three forces - Idea (affirming), Routine (denying), and Form (reconciling). These correspond to positions 3, 6, and 9 on the enneagram.

**The Enneagram (Law of Seven)**: The internal hexad (1-4-2-8-5-7) represents the stages of any process, with "shocks" needed at positions 3 and 6 to continue the process.

**Self-Similarity**: The System exhibits fractal self-similarity, where the same patterns repeat at different scales of nesting.

## Related Projects

- **[cosysoc](https://github.com/o9nn/cosysoc)**: Documentation and Python implementation of Robert Campbell's Systems 1-5 framework with visualizations and mathematical formulations

## License

This library is provided for educational and research purposes. The theoretical framework is based on Robert Campbell's published works.

## References

1. Campbell, R. (1985). *Fisherman's Guide: A Systems Approach to Creativity and Organization*. New Science Library/Shambhala.
2. Campbell, R. *Science and Cosmic Order: A New Prospectus*.
3. Campbell, R. "The System" series of papers.
4. OEIS A000081: Number of rooted trees. https://oeis.org/A000081
5. OEIS A000055: Number of unrooted trees. https://oeis.org/A000055
6. http://cosmic-mindreach.com
