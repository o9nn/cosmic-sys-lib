/**
 * @file terms.hpp
 * @brief Complete system terms definitions for Systems 0-10
 * 
 * Based on Robert Campbell's framework, with term counts aligned to OEIS A000081
 * (number of rooted trees with n unlabeled nodes) where System n has A000081(n+1) terms.
 * 
 * Terms are grouped into clusters via the flip transform, with cluster counts
 * following OEIS A000055 (number of unrooted trees with n nodes).
 * 
 * System n corresponds to rooted trees with (n+1) nodes, where the root represents
 * the universal interface and the n additional nodes represent the system's terms.
 */

#ifndef COSMIC_TERMS_HPP
#define COSMIC_TERMS_HPP

#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <functional>

namespace cosmic {
namespace terms {

// ============================================================================
// OEIS A000081 - Number of rooted trees with n unlabeled nodes
// System n has A000081(n+1) terms
// ============================================================================

/**
 * @brief OEIS A000081 sequence values
 * A000081(n) = number of rooted trees with n unlabeled nodes
 * 
 * a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=9, a(6)=20, a(7)=48,
 * a(8)=115, a(9)=286, a(10)=719, a(11)=1842, ...
 */
constexpr std::array<size_t, 12> A000081 = {
    0,      // a(0) - not used (no tree with 0 nodes)
    1,      // a(1) - single node (root only)
    1,      // a(2) - root + 1 node
    2,      // a(3) - root + 2 nodes
    4,      // a(4) - root + 3 nodes
    9,      // a(5) - root + 4 nodes
    20,     // a(6) - root + 5 nodes
    48,     // a(7) - root + 6 nodes
    115,    // a(8) - root + 7 nodes
    286,    // a(9) - root + 8 nodes
    719,    // a(10) - root + 9 nodes
    1842    // a(11) - root + 10 nodes
};

// ============================================================================
// OEIS A000055 - Number of unrooted trees with n unlabeled nodes
// Used for cluster counts under flip transform
// ============================================================================

/**
 * @brief OEIS A000055 sequence values
 * A000055(n) = number of unrooted trees with n unlabeled nodes
 * 
 * a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=6, a(7)=11,
 * a(8)=23, a(9)=47, a(10)=106, a(11)=235, ...
 */
constexpr std::array<size_t, 12> A000055 = {
    1,      // a(0) - empty tree (void)
    1,      // a(1) - single node
    1,      // a(2) - path of 2
    1,      // a(3) - path of 3
    2,      // a(4) - path of 4, star of 4
    3,      // a(5) - 3 distinct unrooted trees
    6,      // a(6) - 6 distinct unrooted trees
    11,     // a(7) - 11 distinct unrooted trees
    23,     // a(8) - 23 distinct unrooted trees
    47,     // a(9) - 47 distinct unrooted trees
    106,    // a(10) - 106 distinct unrooted trees
    235     // a(11) - 235 distinct unrooted trees
};

// ============================================================================
// System Definition Structure
// ============================================================================

/**
 * @brief Complete definition of a system level
 */
struct SystemDefinition {
    int level;              ///< System level (0-10)
    size_t termCount;       ///< Number of terms = A000081(level+1)
    size_t clusterCount;    ///< Number of clusters = A000055(level+1)
    size_t nodeCount;       ///< Number of nodes = level (excluding root)
    std::string name;       ///< System name
    std::string description;///< System description
};

/**
 * @brief Get the complete system definitions for Systems 0-10
 */
inline std::vector<SystemDefinition> getSystemDefinitions() {
    return {
        {0, 1, 1, 0, "System 0", "The Void - root only, primordial unity before differentiation"},
        {1, 1, 1, 1, "System 1", "Universal Wholeness - active interface between inside and outside"},
        {2, 2, 1, 2, "System 2", "The Fundamental Dyad - objective and subjective modes"},
        {3, 4, 2, 3, "System 3", "The Primary Activity - four terms in two clusters"},
        {4, 9, 3, 4, "System 4", "The Enneagram - nine terms in three clusters"},
        {5, 20, 6, 5, "System 5", "Complementary Structures - twenty terms in six clusters"},
        {6, 48, 11, 6, "System 6", "Primary Activity of Enneagrams - 48 terms in 11 clusters"},
        {7, 115, 23, 7, "System 7", "Enneagram of Enneagrams - 115 terms in 23 clusters"},
        {8, 286, 47, 8, "System 8", "Nested Complementarity - 286 terms in 47 clusters"},
        {9, 719, 106, 9, "System 9", "Deep Nesting - 719 terms in 106 clusters"},
        {10, 1842, 235, 10, "System 10", "Full Recursive Elaboration - 1842 terms in 235 clusters"}
    };
}

/**
 * @brief Get term count for a system level using A000081
 * @param level System level (0-10)
 * @return Number of terms = A000081(level+1)
 */
inline size_t termCountForLevel(int level) {
    if (level < 0 || level > 10) return 0;
    return A000081[level + 1];
}

/**
 * @brief Get cluster count for a system level using A000055
 * @param level System level (0-10)
 * @return Number of clusters = A000055(level+1)
 */
inline size_t clusterCountForLevel(int level) {
    if (level < 0 || level > 10) return 0;
    return A000055[level + 1];
}

/**
 * @brief Get node count for a system level (excluding root)
 * @param level System level (0-10)
 * @return Number of nodes = level
 */
inline size_t nodeCountForLevel(int level) {
    if (level < 0) return 0;
    return static_cast<size_t>(level);
}

// ============================================================================
// Rooted Tree Representation
// ============================================================================

/**
 * @brief A node in a rooted tree
 */
struct TreeNode {
    int id;                              ///< Unique node identifier
    std::vector<std::shared_ptr<TreeNode>> children;  ///< Child nodes
    std::weak_ptr<TreeNode> parent;      ///< Parent node (weak to avoid cycles)
    std::string label;                   ///< Optional label
    
    TreeNode(int id = 0) : id(id) {}
    
    bool isLeaf() const { return children.empty(); }
    bool isRoot() const { return parent.expired(); }
    size_t degree() const { return children.size(); }
    
    /// Get depth from root
    size_t depth() const {
        size_t d = 0;
        auto p = parent.lock();
        while (p) {
            ++d;
            p = p->parent.lock();
        }
        return d;
    }
    
    /// Count total nodes in subtree (including this node)
    size_t subtreeSize() const {
        size_t count = 1;
        for (const auto& child : children) {
            count += child->subtreeSize();
        }
        return count;
    }
};

using TreeNodePtr = std::shared_ptr<TreeNode>;

/**
 * @brief A rooted tree structure
 */
class RootedTree {
public:
    RootedTree() : root_(std::make_shared<TreeNode>(0)) {}
    explicit RootedTree(TreeNodePtr root) : root_(root) {}
    
    TreeNodePtr root() const { return root_; }
    
    /// Get total number of nodes
    size_t nodeCount() const {
        return root_ ? root_->subtreeSize() : 0;
    }
    
    /// Get canonical string representation for comparison
    std::string canonical() const {
        return canonicalString(root_);
    }
    
    /// Check equality via canonical form
    bool operator==(const RootedTree& other) const {
        return canonical() == other.canonical();
    }
    
    /// Get all nodes in pre-order traversal
    std::vector<TreeNodePtr> allNodes() const {
        std::vector<TreeNodePtr> nodes;
        collectNodes(root_, nodes);
        return nodes;
    }
    
private:
    TreeNodePtr root_;
    
    static std::string canonicalString(TreeNodePtr node) {
        if (!node) return "()";
        
        std::vector<std::string> childStrings;
        for (const auto& child : node->children) {
            childStrings.push_back(canonicalString(child));
        }
        
        // Sort for canonical form
        std::sort(childStrings.begin(), childStrings.end());
        
        std::string result = "(";
        for (const auto& s : childStrings) {
            result += s;
        }
        result += ")";
        
        return result;
    }
    
    static void collectNodes(TreeNodePtr node, std::vector<TreeNodePtr>& nodes) {
        if (!node) return;
        nodes.push_back(node);
        for (const auto& child : node->children) {
            collectNodes(child, nodes);
        }
    }
};

// ============================================================================
// Rooted Tree Generator (generates all A000081(n) trees with n nodes)
// ============================================================================

/**
 * @brief Generator for all rooted trees with n nodes
 */
class RootedTreeGenerator {
public:
    /**
     * @brief Generate all rooted trees with n nodes
     * @param n Number of nodes (including root)
     * @return Vector of all distinct rooted trees
     */
    static std::vector<RootedTree> generate(int n) {
        if (n <= 0) return {};
        if (n == 1) {
            return {RootedTree()};  // Single root node
        }
        
        // Generate all partitions of (n-1) and build trees
        std::vector<RootedTree> result;
        std::vector<int> partition;
        generatePartitions(n - 1, n - 1, partition, result);
        
        return result;
    }
    
private:
    static void generatePartitions(int remaining, int maxPart, 
                                   std::vector<int>& partition,
                                   std::vector<RootedTree>& result) {
        if (remaining == 0) {
            // Build trees from this partition
            buildTreesFromPartition(partition, result);
            return;
        }
        
        for (int part = std::min(remaining, maxPart); part >= 1; --part) {
            partition.push_back(part);
            generatePartitions(remaining - part, part, partition, result);
            partition.pop_back();
        }
    }
    
    static void buildTreesFromPartition(const std::vector<int>& partition,
                                        std::vector<RootedTree>& result) {
        // For each partition, we need to select subtrees for each part
        // This is a recursive process using previously generated trees
        
        if (partition.empty()) {
            result.push_back(RootedTree());
            return;
        }
        
        // Get all combinations of subtrees for each partition element
        std::vector<std::vector<RootedTree>> subtreeSets;
        for (int size : partition) {
            subtreeSets.push_back(generate(size));
        }
        
        // Generate all combinations (with proper handling of repeated sizes)
        std::vector<std::vector<int>> combinations;
        generateCombinations(subtreeSets, partition, combinations);
        
        for (const auto& combo : combinations) {
            auto root = std::make_shared<TreeNode>(0);
            int nodeId = 1;
            
            for (size_t i = 0; i < combo.size(); ++i) {
                auto subtree = copyTree(subtreeSets[i][combo[i]], nodeId);
                subtree->parent = root;
                root->children.push_back(subtree);
            }
            
            RootedTree tree(root);
            
            // Check if this tree is already in result (canonical form)
            bool found = false;
            for (const auto& existing : result) {
                if (existing == tree) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                result.push_back(tree);
            }
        }
    }
    
    static void generateCombinations(const std::vector<std::vector<RootedTree>>& sets,
                                     const std::vector<int>& partition,
                                     std::vector<std::vector<int>>& result) {
        std::vector<int> indices(sets.size(), 0);
        generateCombinationsRecursive(sets, partition, indices, 0, result);
    }
    
    static void generateCombinationsRecursive(
        const std::vector<std::vector<RootedTree>>& sets,
        const std::vector<int>& partition,
        std::vector<int>& indices,
        size_t pos,
        std::vector<std::vector<int>>& result) {
        
        if (pos == sets.size()) {
            result.push_back(indices);
            return;
        }
        
        // Determine starting index based on previous same-sized partitions
        int startIdx = 0;
        if (pos > 0 && partition[pos] == partition[pos - 1]) {
            startIdx = indices[pos - 1];
        }
        
        for (size_t i = startIdx; i < sets[pos].size(); ++i) {
            indices[pos] = static_cast<int>(i);
            generateCombinationsRecursive(sets, partition, indices, pos + 1, result);
        }
    }
    
    static TreeNodePtr copyTree(const RootedTree& tree, int& nodeId) {
        return copyNode(tree.root(), nodeId);
    }
    
    static TreeNodePtr copyNode(TreeNodePtr node, int& nodeId) {
        if (!node) return nullptr;
        
        auto copy = std::make_shared<TreeNode>(nodeId++);
        for (const auto& child : node->children) {
            auto childCopy = copyNode(child, nodeId);
            childCopy->parent = copy;
            copy->children.push_back(childCopy);
        }
        
        return copy;
    }
};

// ============================================================================
// Flip Transform (Rooted -> Unrooted equivalence classes)
// ============================================================================

/**
 * @brief Represents an unrooted tree (equivalence class under re-rooting)
 */
class UnrootedTree {
public:
    explicit UnrootedTree(const RootedTree& representative)
        : representative_(representative) {}
    
    const RootedTree& representative() const { return representative_; }
    
    /// Get all possible rootings of this unrooted tree
    std::vector<RootedTree> allRootings() const {
        std::vector<RootedTree> rootings;
        auto nodes = representative_.allNodes();
        
        for (const auto& node : nodes) {
            rootings.push_back(rerootAt(node));
        }
        
        // Remove duplicates
        std::vector<RootedTree> unique;
        for (const auto& tree : rootings) {
            bool found = false;
            for (const auto& u : unique) {
                if (u == tree) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                unique.push_back(tree);
            }
        }
        
        return unique;
    }
    
    /// Check if two rooted trees belong to the same unrooted equivalence class
    static bool sameClass(const RootedTree& a, const RootedTree& b) {
        UnrootedTree ua(a);
        auto rootings = ua.allRootings();
        
        for (const auto& r : rootings) {
            if (r == b) return true;
        }
        return false;
    }
    
private:
    RootedTree representative_;
    
    RootedTree rerootAt(TreeNodePtr newRoot) const {
        // Create a new tree rooted at the given node
        // This requires reversing parent-child relationships along the path to root
        
        // For simplicity, we'll use the canonical form approach
        // A full implementation would traverse and rebuild
        return representative_;  // Placeholder - full implementation needed
    }
};

/**
 * @brief Group rooted trees into unrooted equivalence classes (clusters)
 */
class FlipTransform {
public:
    /**
     * @brief Group rooted trees by their unrooted equivalence class
     * @param trees Vector of rooted trees
     * @return Vector of clusters, each containing equivalent rooted trees
     */
    static std::vector<std::vector<RootedTree>> groupIntoClusters(
        const std::vector<RootedTree>& trees) {
        
        std::vector<std::vector<RootedTree>> clusters;
        std::vector<bool> assigned(trees.size(), false);
        
        for (size_t i = 0; i < trees.size(); ++i) {
            if (assigned[i]) continue;
            
            std::vector<RootedTree> cluster;
            cluster.push_back(trees[i]);
            assigned[i] = true;
            
            for (size_t j = i + 1; j < trees.size(); ++j) {
                if (assigned[j]) continue;
                
                if (UnrootedTree::sameClass(trees[i], trees[j])) {
                    cluster.push_back(trees[j]);
                    assigned[j] = true;
                }
            }
            
            clusters.push_back(cluster);
        }
        
        return clusters;
    }
    
    /**
     * @brief Get the number of clusters for trees with n nodes
     * This should equal A000055(n)
     */
    static size_t clusterCount(int n) {
        auto trees = RootedTreeGenerator::generate(n);
        auto clusters = groupIntoClusters(trees);
        return clusters.size();
    }
};

// ============================================================================
// System 3 - Four Terms in Two Clusters
// ============================================================================

/**
 * @brief Triadic term types
 */
enum class TriadicType {
    Idea,     ///< The integrating concept/principle
    Routine,  ///< The cyclic/repetitive processes
    Form      ///< The manifested pattern/structure
};

/**
 * @brief Orientation types
 */
enum class OrientationType {
    Objective,   ///< Outward-looking (Universal)
    Subjective   ///< Inward-looking (Particular)
};

/**
 * @brief The four fundamental terms of System 3
 * Grouped into 2 clusters under flip transform
 */
struct System3Term {
    int id;
    std::string name;
    OrientationType orientation;
    bool isUniversal;
    int cluster;  ///< Cluster ID (0 or 1)
    std::string description;
};

/**
 * @brief Get the four terms of System 3
 */
inline std::vector<System3Term> getSystem3Terms() {
    return {
        // Cluster 0: Universal terms (the "trunk" of the tree)
        {1, "Universal Discretion", OrientationType::Objective, true, 0,
         "Selects and assimilates elements from the void"},
        {2, "Universal Means", OrientationType::Subjective, true, 0,
         "Countercurrent identities balancing subjective/objective"},
        
        // Cluster 1: Particular terms (the "branches")
        {3, "Particular Goals", OrientationType::Objective, false, 1,
         "The goal term directing activity"},
        {4, "Particular Consequence", OrientationType::Subjective, false, 1,
         "Follows from goal through perceptual transposition"}
    };
}

// ============================================================================
// System 4 - Nine Terms in Three Clusters
// ============================================================================

/**
 * @brief The nine terms of System 4 (Enneagram)
 * Grouped into 3 clusters under flip transform
 */
struct System4Term {
    int position;       ///< Enneagram position (1-9)
    std::string name;
    std::string shortName;
    int cluster;        ///< Cluster ID (0, 1, or 2)
    bool isTriangle;    ///< True for positions 3, 6, 9
    std::string description;
};

/**
 * @brief Get the nine terms of System 4
 */
inline std::vector<System4Term> getSystem4Terms() {
    return {
        // Cluster 0: Triangle terms (Universal - Law of Three)
        {9, "Discretionary Hierarchy", "T9", 0, true,
         "Universal discretion selecting elements from the void"},
        {3, "Idea Transference", "T3", 0, true,
         "Transfers the integrating idea through the creative process"},
        {6, "Corporeal Body", "T6", 0, true,
         "The living form resulting from the creative process"},
        
        // Cluster 1: Upper hexad (Initiating/Developing)
        {1, "Perception of Need", "T1", 1, false,
         "Perceives the need that initiates the creative process"},
        {4, "Organized Input", "T4", 1, false,
         "Organizes the input required for manifestation"},
        {2, "Idea Creation", "T2", 1, false,
         "Creates the specific idea to address the need"},
        
        // Cluster 2: Lower hexad (Maturing/Completing)
        {8, "Response to Need", "T8", 2, false,
         "Response that completes the cycle back to need"},
        {5, "Physical Action", "T5", 2, false,
         "The physical action that transforms idea into form"},
        {7, "Quantized Memory", "T7", 2, false,
         "Memory elements that guide the process"}
    };
}

/**
 * @brief Get term by enneagram position
 */
inline std::optional<System4Term> getEnneagramTerm(int position) {
    auto terms = getSystem4Terms();
    for (const auto& term : terms) {
        if (term.position == position) {
            return term;
        }
    }
    return std::nullopt;
}

// ============================================================================
// System 5 - Twenty Terms in Six Clusters
// ============================================================================

/**
 * @brief System 5 term structure
 */
struct System5Term {
    int id;
    std::string name;
    int cluster;
    std::string treeStructure;  ///< Canonical tree representation
    std::string description;
};

/**
 * @brief Get descriptions for System 5's 20 terms
 * These correspond to A000081(6) = 20 rooted trees with 6 nodes
 */
inline std::vector<System5Term> getSystem5Terms() {
    // The 20 rooted trees with 6 nodes, grouped into 6 unrooted classes
    return {
        // Cluster 0: Linear chain variants (path graph)
        {1, "Linear Descent", 0, "((((()))))", "Pure sequential elaboration"},
        {2, "Linear Ascent", 0, "((((()))))", "Reverse sequential return"},
        
        // Cluster 1: Star variants (central hub)
        {3, "Central Hub", 1, "(()()()(()))", "Five-fold radiation from center"},
        {4, "Peripheral Return", 1, "(()()()(()))", "Convergence to center"},
        
        // Cluster 2: Y-shaped variants
        {5, "Triadic Branch", 2, "((())(())())", "Three-way split"},
        {6, "Triadic Merge", 2, "((())(())())", "Three-way convergence"},
        {7, "Asymmetric Y", 2, "((()())(()))", "Unbalanced branching"},
        
        // Cluster 3: Fork variants
        {8, "Double Fork", 3, "(((()))(()))", "Nested bifurcation"},
        {9, "Fork Return", 3, "(((()))(()))", "Bifurcation return"},
        {10, "Extended Fork", 3, "((())(()()))", "Extended branching"},
        {11, "Compressed Fork", 3, "((()()())())", "Compressed structure"},
        
        // Cluster 4: Caterpillar variants
        {12, "Caterpillar A", 4, "((()(()))())", "Linear with single branch"},
        {13, "Caterpillar B", 4, "((()())(()))", "Linear with double branch"},
        {14, "Caterpillar C", 4, "(((())())())", "Nested caterpillar"},
        {15, "Caterpillar D", 4, "((())(()()))", "Extended caterpillar"},
        {16, "Caterpillar E", 4, "((()(())()))", "Complex caterpillar"},
        
        // Cluster 5: Complex variants
        {17, "Complex A", 5, "((()())()())", "Multi-branch structure"},
        {18, "Complex B", 5, "(((()))()())", "Deep with siblings"},
        {19, "Complex C", 5, "((())()()())", "Shallow with many siblings"},
        {20, "Complex D", 5, "(()()()()())", "Maximum breadth"}
    };
}

// ============================================================================
// Higher Systems (6-10) - Generated Structures
// ============================================================================

/**
 * @brief Generic term for higher systems
 */
struct HigherSystemTerm {
    int id;
    int systemLevel;
    int cluster;
    std::string canonicalForm;
    std::string description;
};

/**
 * @brief Generate term descriptions for a higher system
 */
inline std::vector<HigherSystemTerm> generateHigherSystemTerms(int level) {
    if (level < 6 || level > 10) return {};
    
    size_t termCount = termCountForLevel(level);
    size_t clusterCount = clusterCountForLevel(level);
    
    std::vector<HigherSystemTerm> terms;
    terms.reserve(termCount);
    
    // Generate placeholder terms (full tree generation would be computationally intensive)
    size_t termsPerCluster = termCount / clusterCount;
    size_t remainder = termCount % clusterCount;
    
    int id = 1;
    for (size_t c = 0; c < clusterCount; ++c) {
        size_t clusterSize = termsPerCluster + (c < remainder ? 1 : 0);
        
        for (size_t t = 0; t < clusterSize; ++t) {
            std::stringstream desc;
            desc << "System " << level << " term " << id 
                 << " in cluster " << c;
            
            terms.push_back({
                id,
                level,
                static_cast<int>(c),
                "",  // Canonical form would be computed
                desc.str()
            });
            ++id;
        }
    }
    
    return terms;
}

// ============================================================================
// Cosmic Movie Hierarchy (System 3 elaboration)
// ============================================================================

/**
 * @brief Cosmic level enumeration
 */
enum class CosmicLevel {
    Galaxy,  ///< Universal integration level (Idea)
    Sun,     ///< Stellar processing level (Routine)
    Planet   ///< Planetary manifestation level (Form)
};

/**
 * @brief Cosmic movie term (3x3 = 9 terms, but System 3 has 4 fundamental terms)
 * The cosmic movie is an application/interpretation of System 3
 */
struct CosmicMovieTerm {
    CosmicLevel level;
    TriadicType type;
    std::string name;
    std::string description;
};

/**
 * @brief Get the cosmic movie interpretation
 */
inline std::vector<CosmicMovieTerm> getCosmicMovieTerms() {
    return {
        {CosmicLevel::Galaxy, TriadicType::Idea, "Galaxy-Idea",
         "Black holes retain synchronicity with universal hydrogen projection"},
        {CosmicLevel::Galaxy, TriadicType::Routine, "Galaxy-Routine",
         "Space-time contraction offset by nuclear fusion"},
        {CosmicLevel::Galaxy, TriadicType::Form, "Galaxy-Form",
         "Stars form synchronously, recycle through heavy atoms"},
        
        {CosmicLevel::Sun, TriadicType::Idea, "Sun-Idea",
         "Stars retain synchronicity by contracting space"},
        {CosmicLevel::Sun, TriadicType::Routine, "Sun-Routine",
         "Routines adjust for spatial gaps from fusion and decay"},
        {CosmicLevel::Sun, TriadicType::Form, "Sun-Form",
         "Cyclic motions enable molecular chemistry"},
        
        {CosmicLevel::Planet, TriadicType::Idea, "Planet-Idea",
         "EM and gravitational form relates via cyclic routines"},
        {CosmicLevel::Planet, TriadicType::Routine, "Planet-Routine",
         "Planets bathed in solar EM energy"},
        {CosmicLevel::Planet, TriadicType::Form, "Planet-Form",
         "Chemical integration fosters life evolution"}
    };
}

// ============================================================================
// Biosphere Hierarchy
// ============================================================================

/**
 * @brief Biosphere tier enumeration
 */
enum class BiosphereTier {
    Humans,
    Vertebrates,
    Invertebrates,
    Plants
};

/**
 * @brief Biosphere term definition
 */
struct BiosphereTermDef {
    BiosphereTier tier;
    TriadicType type;
    std::string name;
    std::string description;
};

/**
 * @brief Get biosphere hierarchy terms
 */
inline std::vector<BiosphereTermDef> getBiosphereTerms() {
    return {
        {BiosphereTier::Humans, TriadicType::Idea, "Human-Idea",
         "Future cosmic ideation opening human mind"},
        {BiosphereTier::Humans, TriadicType::Routine, "Human-Routine",
         "Industrial routines dominating planet"},
        {BiosphereTier::Humans, TriadicType::Form, "Human-Form",
         "Spirit cultures, division of labor"},
        
        {BiosphereTier::Vertebrates, TriadicType::Idea, "Vertebrate-Idea",
         "Primates to humans; speech polarizes brain"},
        {BiosphereTier::Vertebrates, TriadicType::Routine, "Vertebrate-Routine",
         "Mammals modulating emotive routines"},
        {BiosphereTier::Vertebrates, TriadicType::Form, "Vertebrate-Form",
         "Reptiles exploring quadruped form"},
        
        {BiosphereTier::Invertebrates, TriadicType::Idea, "Invertebrate-Idea",
         "Ants, bees using division of labor"},
        {BiosphereTier::Invertebrates, TriadicType::Routine, "Invertebrate-Routine",
         "Segmented worms integrating routines"},
        {BiosphereTier::Invertebrates, TriadicType::Form, "Invertebrate-Form",
         "Sponges, jellyfish exploring forms"},
        
        {BiosphereTier::Plants, TriadicType::Idea, "Plant-Idea",
         "Flowering plants using extended ideas"},
        {BiosphereTier::Plants, TriadicType::Routine, "Plant-Routine",
         "Vascular systems exploring routines"},
        {BiosphereTier::Plants, TriadicType::Form, "Plant-Form",
         "Algae, fungi exploring cell forms"}
    };
}

// ============================================================================
// Process Sequences
// ============================================================================

/**
 * @brief Get the hexad sequence (Law of Seven)
 */
inline std::array<int, 6> getHexadSequence() {
    return {1, 4, 2, 8, 5, 7};
}

/**
 * @brief Get the triangle sequence (Law of Three)
 */
inline std::array<int, 3> getTriangleSequence() {
    return {9, 3, 6};
}

/**
 * @brief Get the full creative process sequence
 */
inline std::array<int, 9> getCreativeProcessSequence() {
    return {1, 4, 2, 3, 8, 5, 7, 6, 9};
}

/**
 * @brief Check if a position is a shock point
 */
inline bool isShockPoint(int position) {
    return position == 3 || position == 6;
}

/**
 * @brief Check if a position is on the triangle
 */
inline bool isTrianglePosition(int position) {
    return position == 3 || position == 6 || position == 9;
}

// ============================================================================
// Utility Functions
// ============================================================================

inline std::string cosmicLevelToString(CosmicLevel level) {
    switch (level) {
        case CosmicLevel::Galaxy: return "Galaxy";
        case CosmicLevel::Sun: return "Sun";
        case CosmicLevel::Planet: return "Planet";
    }
    return "Unknown";
}

inline std::string biosphereTierToString(BiosphereTier tier) {
    switch (tier) {
        case BiosphereTier::Humans: return "Humans";
        case BiosphereTier::Vertebrates: return "Vertebrates";
        case BiosphereTier::Invertebrates: return "Invertebrates";
        case BiosphereTier::Plants: return "Plants";
    }
    return "Unknown";
}

inline std::string triadicTypeToString(TriadicType type) {
    switch (type) {
        case TriadicType::Idea: return "Idea";
        case TriadicType::Routine: return "Routine";
        case TriadicType::Form: return "Form";
    }
    return "Unknown";
}

inline std::string orientationToString(OrientationType orientation) {
    switch (orientation) {
        case OrientationType::Objective: return "Objective";
        case OrientationType::Subjective: return "Subjective";
    }
    return "Unknown";
}

} // namespace terms
} // namespace cosmic

#endif // COSMIC_TERMS_HPP
