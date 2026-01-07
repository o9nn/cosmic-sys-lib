/**
 * @file trees.hpp
 * @brief Rooted tree generation and flip transform for OEIS A000081/A000055
 * 
 * This module provides algorithms for:
 * - Generating all rooted trees with n nodes (OEIS A000081)
 * - Grouping rooted trees into unrooted equivalence classes via flip transform (OEIS A000055)
 * - Computing canonical forms for tree comparison
 * 
 * The flip transform maps rooted trees to unrooted trees by considering all
 * possible rootings of the same underlying tree structure as equivalent.
 */

#ifndef COSMIC_TREES_HPP
#define COSMIC_TREES_HPP

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <functional>
#include <map>
#include <set>

namespace cosmic {
namespace trees {

// ============================================================================
// OEIS Sequences
// ============================================================================

/**
 * @brief OEIS A000081 - Number of rooted trees with n unlabeled nodes
 */
constexpr std::array<size_t, 12> A000081 = {
    0, 1, 1, 2, 4, 9, 20, 48, 115, 286, 719, 1842
};

/**
 * @brief OEIS A000055 - Number of unrooted trees with n unlabeled nodes
 */
constexpr std::array<size_t, 12> A000055 = {
    1, 1, 1, 1, 2, 3, 6, 11, 23, 47, 106, 235
};

/**
 * @brief Get A000081(n) for system level calculations
 */
inline size_t a000081(int n) {
    if (n < 0 || n > 11) return 0;
    return A000081[n];
}

/**
 * @brief Get A000055(n) for cluster calculations
 */
inline size_t a000055(int n) {
    if (n < 0 || n > 11) return 0;
    return A000055[n];
}

// ============================================================================
// Tree Node Structure
// ============================================================================

/**
 * @brief A node in a rooted tree
 */
class TreeNode {
public:
    using Ptr = std::shared_ptr<TreeNode>;
    using WeakPtr = std::weak_ptr<TreeNode>;
    
    TreeNode() = default;
    explicit TreeNode(int id) : id_(id) {}
    
    int id() const { return id_; }
    void setId(int id) { id_ = id; }
    
    const std::string& label() const { return label_; }
    void setLabel(const std::string& label) { label_ = label; }
    
    const std::vector<Ptr>& children() const { return children_; }
    std::vector<Ptr>& children() { return children_; }
    
    Ptr parent() const { return parent_.lock(); }
    void setParent(Ptr p) { parent_ = p; }
    
    bool isLeaf() const { return children_.empty(); }
    bool isRoot() const { return parent_.expired(); }
    
    size_t degree() const { return children_.size(); }
    
    size_t depth() const {
        size_t d = 0;
        auto p = parent_.lock();
        while (p) {
            ++d;
            p = p->parent_.lock();
        }
        return d;
    }
    
    size_t subtreeSize() const {
        size_t count = 1;
        for (const auto& child : children_) {
            count += child->subtreeSize();
        }
        return count;
    }
    
    void addChild(Ptr child) {
        children_.push_back(child);
    }
    
private:
    int id_ = 0;
    std::string label_;
    std::vector<Ptr> children_;
    WeakPtr parent_;
};

// ============================================================================
// Rooted Tree Class
// ============================================================================

/**
 * @brief A rooted tree structure
 */
class RootedTree {
public:
    using NodePtr = TreeNode::Ptr;
    
    RootedTree() : root_(std::make_shared<TreeNode>(0)) {}
    explicit RootedTree(NodePtr root) : root_(root) {}
    
    NodePtr root() const { return root_; }
    
    size_t nodeCount() const {
        return root_ ? root_->subtreeSize() : 0;
    }
    
    /**
     * @brief Get canonical string representation
     * 
     * The canonical form is a nested parenthesis representation where
     * children are sorted lexicographically. This allows comparison
     * of trees for equality.
     */
    std::string canonical() const {
        return canonicalString(root_);
    }
    
    bool operator==(const RootedTree& other) const {
        return canonical() == other.canonical();
    }
    
    bool operator<(const RootedTree& other) const {
        return canonical() < other.canonical();
    }
    
    /**
     * @brief Get all nodes in pre-order traversal
     */
    std::vector<NodePtr> allNodes() const {
        std::vector<NodePtr> nodes;
        collectNodes(root_, nodes);
        return nodes;
    }
    
    /**
     * @brief Create a deep copy of this tree
     */
    RootedTree copy() const {
        int nextId = 0;
        return RootedTree(copyNode(root_, nextId));
    }
    
    /**
     * @brief Create tree from canonical string
     */
    static RootedTree fromCanonical(const std::string& canonical) {
        int pos = 0;
        int nextId = 0;
        auto root = parseCanonical(canonical, pos, nextId);
        return RootedTree(root);
    }
    
private:
    NodePtr root_;
    
    static std::string canonicalString(NodePtr node) {
        if (!node) return "()";
        
        std::vector<std::string> childStrings;
        for (const auto& child : node->children()) {
            childStrings.push_back(canonicalString(child));
        }
        
        std::sort(childStrings.begin(), childStrings.end());
        
        std::string result = "(";
        for (const auto& s : childStrings) {
            result += s;
        }
        result += ")";
        
        return result;
    }
    
    static void collectNodes(NodePtr node, std::vector<NodePtr>& nodes) {
        if (!node) return;
        nodes.push_back(node);
        for (const auto& child : node->children()) {
            collectNodes(child, nodes);
        }
    }
    
    static NodePtr copyNode(NodePtr node, int& nextId) {
        if (!node) return nullptr;
        
        auto copy = std::make_shared<TreeNode>(nextId++);
        copy->setLabel(node->label());
        
        for (const auto& child : node->children()) {
            auto childCopy = copyNode(child, nextId);
            childCopy->setParent(copy);
            copy->addChild(childCopy);
        }
        
        return copy;
    }
    
    static NodePtr parseCanonical(const std::string& s, int& pos, int& nextId) {
        if (pos >= static_cast<int>(s.length()) || s[pos] != '(') {
            return nullptr;
        }
        
        auto node = std::make_shared<TreeNode>(nextId++);
        ++pos;  // Skip '('
        
        while (pos < static_cast<int>(s.length()) && s[pos] == '(') {
            auto child = parseCanonical(s, pos, nextId);
            if (child) {
                child->setParent(node);
                node->addChild(child);
            }
        }
        
        if (pos < static_cast<int>(s.length()) && s[pos] == ')') {
            ++pos;  // Skip ')'
        }
        
        return node;
    }
};

// ============================================================================
// Rooted Tree Generator
// ============================================================================

/**
 * @brief Generator for all rooted trees with n nodes
 * 
 * Uses the recursive structure: a rooted tree with n nodes consists of
 * a root with subtrees whose sizes partition (n-1).
 */
class RootedTreeGenerator {
public:
    /**
     * @brief Generate all rooted trees with n nodes
     * @param n Number of nodes (including root)
     * @return Vector of all distinct rooted trees (should have A000081(n) elements)
     */
    static std::vector<RootedTree> generate(int n) {
        if (n <= 0) return {};
        
        // Use memoization for efficiency
        static std::map<int, std::vector<RootedTree>> cache;
        
        auto it = cache.find(n);
        if (it != cache.end()) {
            return it->second;
        }
        
        std::vector<RootedTree> result;
        
        if (n == 1) {
            result.push_back(RootedTree());
        } else {
            // Generate all partitions of (n-1) and build trees
            std::vector<int> partition;
            generateFromPartitions(n - 1, n - 1, partition, result);
        }
        
        cache[n] = result;
        return result;
    }
    
    /**
     * @brief Clear the generation cache
     */
    static void clearCache() {
        // Would need to make cache non-static or add a clear method
    }
    
private:
    static void generateFromPartitions(int remaining, int maxPart,
                                       std::vector<int>& partition,
                                       std::vector<RootedTree>& result) {
        if (remaining == 0) {
            buildTreesFromPartition(partition, result);
            return;
        }
        
        for (int part = std::min(remaining, maxPart); part >= 1; --part) {
            partition.push_back(part);
            generateFromPartitions(remaining - part, part, partition, result);
            partition.pop_back();
        }
    }
    
    static void buildTreesFromPartition(const std::vector<int>& partition,
                                        std::vector<RootedTree>& result) {
        if (partition.empty()) {
            result.push_back(RootedTree());
            return;
        }
        
        // Get subtrees for each partition element
        std::vector<std::vector<RootedTree>> subtreeSets;
        for (int size : partition) {
            subtreeSets.push_back(generate(size));
        }
        
        // Generate all valid combinations
        std::vector<int> indices(partition.size(), 0);
        generateCombinations(subtreeSets, partition, indices, 0, result);
    }
    
    static void generateCombinations(
        const std::vector<std::vector<RootedTree>>& sets,
        const std::vector<int>& partition,
        std::vector<int>& indices,
        size_t pos,
        std::vector<RootedTree>& result) {
        
        if (pos == sets.size()) {
            // Build tree from this combination
            auto root = std::make_shared<TreeNode>(0);
            int nodeId = 1;
            
            for (size_t i = 0; i < indices.size(); ++i) {
                auto subtreeCopy = sets[i][indices[i]].copy();
                auto subtreeRoot = subtreeCopy.root();
                renumberNodes(subtreeRoot, nodeId);
                subtreeRoot->setParent(root);
                root->addChild(subtreeRoot);
            }
            
            RootedTree tree(root);
            
            // Check for duplicates using canonical form
            bool isDuplicate = false;
            for (const auto& existing : result) {
                if (existing == tree) {
                    isDuplicate = true;
                    break;
                }
            }
            
            if (!isDuplicate) {
                result.push_back(tree);
            }
            return;
        }
        
        // Determine starting index (for non-increasing constraint on same-sized parts)
        int startIdx = 0;
        if (pos > 0 && partition[pos] == partition[pos - 1]) {
            startIdx = indices[pos - 1];
        }
        
        for (size_t i = startIdx; i < sets[pos].size(); ++i) {
            indices[pos] = static_cast<int>(i);
            generateCombinations(sets, partition, indices, pos + 1, result);
        }
    }
    
    static void renumberNodes(TreeNode::Ptr node, int& nextId) {
        if (!node) return;
        node->setId(nextId++);
        for (const auto& child : node->children()) {
            renumberNodes(child, nextId);
        }
    }
};

// ============================================================================
// Flip Transform (Rooted -> Unrooted equivalence)
// ============================================================================

/**
 * @brief Represents an unrooted tree as an equivalence class of rooted trees
 */
class UnrootedTree {
public:
    explicit UnrootedTree(const RootedTree& representative)
        : representative_(representative) {
        canonical_ = computeUnrootedCanonical(representative);
    }
    
    const RootedTree& representative() const { return representative_; }
    const std::string& canonical() const { return canonical_; }
    
    bool operator==(const UnrootedTree& other) const {
        return canonical_ == other.canonical_;
    }
    
    bool operator<(const UnrootedTree& other) const {
        return canonical_ < other.canonical_;
    }
    
    /**
     * @brief Check if two rooted trees represent the same unrooted tree
     */
    static bool sameUnrootedClass(const RootedTree& a, const RootedTree& b) {
        return computeUnrootedCanonical(a) == computeUnrootedCanonical(b);
    }
    
private:
    RootedTree representative_;
    std::string canonical_;
    
    /**
     * @brief Compute canonical form for unrooted tree
     * 
     * This considers all possible rootings and takes the lexicographically
     * smallest canonical form.
     */
    static std::string computeUnrootedCanonical(const RootedTree& tree) {
        auto nodes = tree.allNodes();
        std::string minCanonical = tree.canonical();
        
        // Try rooting at each node and find minimum canonical form
        for (const auto& node : nodes) {
            auto rerooted = rerootAt(tree, node);
            std::string canon = rerooted.canonical();
            if (canon < minCanonical) {
                minCanonical = canon;
            }
        }
        
        return minCanonical;
    }
    
    /**
     * @brief Create a new tree rooted at the given node
     */
    static RootedTree rerootAt(const RootedTree& tree, TreeNode::Ptr newRoot) {
        if (newRoot == tree.root()) {
            return tree.copy();
        }
        
        // Build path from newRoot to current root
        std::vector<TreeNode::Ptr> path;
        auto current = newRoot;
        while (current) {
            path.push_back(current);
            current = current->parent();
        }
        
        // Create new tree with reversed parent-child relationships along path
        int nextId = 0;
        auto newRootCopy = std::make_shared<TreeNode>(nextId++);
        
        // Copy subtrees of newRoot (excluding path to old root)
        TreeNode::Ptr pathNext = (path.size() > 1) ? path[1] : nullptr;
        for (const auto& child : newRoot->children()) {
            if (child != pathNext) {
                auto childCopy = copySubtree(child, nextId);
                childCopy->setParent(newRootCopy);
                newRootCopy->addChild(childCopy);
            }
        }
        
        // Add reversed path as a subtree
        if (path.size() > 1) {
            auto reversedPath = buildReversedPath(path, 1, nextId);
            if (reversedPath) {
                reversedPath->setParent(newRootCopy);
                newRootCopy->addChild(reversedPath);
            }
        }
        
        return RootedTree(newRootCopy);
    }
    
    static TreeNode::Ptr copySubtree(TreeNode::Ptr node, int& nextId) {
        if (!node) return nullptr;
        
        auto copy = std::make_shared<TreeNode>(nextId++);
        for (const auto& child : node->children()) {
            auto childCopy = copySubtree(child, nextId);
            childCopy->setParent(copy);
            copy->addChild(childCopy);
        }
        
        return copy;
    }
    
    static TreeNode::Ptr buildReversedPath(const std::vector<TreeNode::Ptr>& path,
                                           size_t idx, int& nextId) {
        if (idx >= path.size()) return nullptr;
        
        auto node = std::make_shared<TreeNode>(nextId++);
        
        // Add children of path[idx] except path[idx-1] and path[idx+1]
        TreeNode::Ptr prev = (idx > 0) ? path[idx - 1] : nullptr;
        TreeNode::Ptr next = (idx + 1 < path.size()) ? path[idx + 1] : nullptr;
        
        for (const auto& child : path[idx]->children()) {
            if (child != prev && child != next) {
                auto childCopy = copySubtree(child, nextId);
                childCopy->setParent(node);
                node->addChild(childCopy);
            }
        }
        
        // Add rest of reversed path
        auto restOfPath = buildReversedPath(path, idx + 1, nextId);
        if (restOfPath) {
            restOfPath->setParent(node);
            node->addChild(restOfPath);
        }
        
        return node;
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
        
        std::map<std::string, std::vector<RootedTree>> clusterMap;
        
        for (const auto& tree : trees) {
            UnrootedTree unrooted(tree);
            clusterMap[unrooted.canonical()].push_back(tree);
        }
        
        std::vector<std::vector<RootedTree>> clusters;
        for (auto& pair : clusterMap) {
            clusters.push_back(std::move(pair.second));
        }
        
        return clusters;
    }
    
    /**
     * @brief Get cluster count for trees with n nodes
     * Should equal A000055(n)
     */
    static size_t clusterCount(int n) {
        auto trees = RootedTreeGenerator::generate(n);
        auto clusters = groupIntoClusters(trees);
        return clusters.size();
    }
    
    /**
     * @brief Verify that generation matches OEIS sequences
     */
    static bool verify(int maxN = 6) {
        for (int n = 1; n <= maxN; ++n) {
            auto trees = RootedTreeGenerator::generate(n);
            if (trees.size() != A000081[n]) {
                return false;
            }
            
            auto clusters = groupIntoClusters(trees);
            if (clusters.size() != A000055[n]) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================================
// System Term Mapping
// ============================================================================

/**
 * @brief Maps system levels to their rooted tree representations
 */
class SystemTreeMapping {
public:
    /**
     * @brief Get all rooted trees for a system level
     * System n has A000081(n+1) trees (terms)
     */
    static std::vector<RootedTree> getSystemTrees(int systemLevel) {
        if (systemLevel < 0 || systemLevel > 10) return {};
        return RootedTreeGenerator::generate(systemLevel + 1);
    }
    
    /**
     * @brief Get clusters for a system level
     * System n has A000055(n+1) clusters
     */
    static std::vector<std::vector<RootedTree>> getSystemClusters(int systemLevel) {
        auto trees = getSystemTrees(systemLevel);
        return FlipTransform::groupIntoClusters(trees);
    }
    
    /**
     * @brief Get summary for a system level
     */
    struct SystemSummary {
        int level;
        size_t termCount;
        size_t clusterCount;
        size_t nodeCount;
        std::vector<std::string> treeCanonicals;
        std::vector<size_t> clusterSizes;
    };
    
    static SystemSummary getSummary(int systemLevel) {
        SystemSummary summary;
        summary.level = systemLevel;
        summary.nodeCount = systemLevel;
        
        auto trees = getSystemTrees(systemLevel);
        summary.termCount = trees.size();
        
        for (const auto& tree : trees) {
            summary.treeCanonicals.push_back(tree.canonical());
        }
        
        auto clusters = FlipTransform::groupIntoClusters(trees);
        summary.clusterCount = clusters.size();
        
        for (const auto& cluster : clusters) {
            summary.clusterSizes.push_back(cluster.size());
        }
        
        return summary;
    }
};

} // namespace trees
} // namespace cosmic

#endif // COSMIC_TREES_HPP
