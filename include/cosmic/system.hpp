/**
 * @file system.hpp
 * @brief Core System hierarchy implementation based on Robert Campbell's Cosmic Order
 * 
 * This library implements the nested enneagram system hierarchy (Systems 1-10)
 * as described in "The Proliferation of the System" and related works.
 * 
 * The System represents a universal methodology for delineating the cosmic order
 * through a progression of discrete sub-systems nested within each other.
 * 
 * @author Based on Robert Campbell's theoretical framework
 * @see http://cosmic-mindreach.com
 */

#ifndef COSMIC_SYSTEM_HPP
#define COSMIC_SYSTEM_HPP

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <functional>
#include <optional>
#include <variant>
#include <map>

namespace cosmic {

// Forward declarations
class System;
class Term;
class Enneagram;
class Interface;

/**
 * @brief Orientation modes for System 2 and higher
 * 
 * The System alternates between objective and subjective orientations
 * with an active transformation between them.
 */
enum class Orientation {
    Objective,   ///< Passive orientation - from outside looking in
    Subjective   ///< Active orientation - from inside looking out
};

/**
 * @brief The three primary terms of System 3 (the triadic structure)
 * 
 * System 3 introduces the fundamental triad that underlies all
 * phenomenal experience and the creative process.
 */
enum class TriadicTerm {
    Idea,     ///< The integrating concept/principle (Universal aspect)
    Routine,  ///< The cyclic/repetitive processes (Particular aspect)
    Form      ///< The manifested pattern/structure (Integration)
};

/**
 * @brief The nine positions of the Enneagram (System 4)
 * 
 * The enneagram represents the primary creative process with
 * 9 terms arranged in a specific geometric pattern.
 */
enum class EnneagramPosition {
    One = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine
};

/**
 * @brief Represents an active interface between inside and outside
 * 
 * System 1 specifies that all phenomena consist of active interface
 * processes that share both a universal inside and a universal outside.
 * We can never know the inside to the exclusion of the outside or vice versa.
 */
class Interface {
public:
    Interface() = default;
    Interface(const std::string& name, Orientation orient = Orientation::Objective);
    
    /// Get the name/identifier of this interface
    const std::string& name() const { return name_; }
    
    /// Get the current orientation
    Orientation orientation() const { return orientation_; }
    
    /// Transform between objective and subjective orientations
    void transform();
    
    /// Check if this interface is active
    bool isActive() const { return active_; }
    
    /// Activate/deactivate the interface
    void setActive(bool active) { active_ = active; }
    
private:
    std::string name_;
    Orientation orientation_ = Orientation::Objective;
    bool active_ = true;
};

/**
 * @brief A Term represents a position in the System hierarchy
 * 
 * Terms can be simple (System 1-3) or compound (System 4+),
 * containing nested sub-terms that elaborate on the structure.
 */
class Term {
public:
    using TermPtr = std::shared_ptr<Term>;
    using TermList = std::vector<TermPtr>;
    
    Term() = default;
    explicit Term(const std::string& name);
    Term(const std::string& name, TriadicTerm type);
    
    /// Get the term name
    const std::string& name() const { return name_; }
    
    /// Get the triadic type (if applicable)
    std::optional<TriadicTerm> triadicType() const { return triadic_type_; }
    
    /// Get the description/content of this term
    const std::string& description() const { return description_; }
    void setDescription(const std::string& desc) { description_ = desc; }
    
    /// Get nested sub-terms (for System 3+ nesting)
    const TermList& subTerms() const { return sub_terms_; }
    TermList& subTerms() { return sub_terms_; }
    
    /// Add a sub-term
    void addSubTerm(TermPtr term);
    
    /// Get the nesting depth
    size_t depth() const;
    
    /// Count total terms including nested
    size_t totalTermCount() const;
    
    /// Check if this term has sub-terms
    bool hasSubTerms() const { return !sub_terms_.empty(); }
    
    /// Get parent term (if nested)
    Term* parent() const { return parent_; }
    
    /// Visitor pattern for traversal
    template<typename Visitor>
    void accept(Visitor&& visitor) const {
        visitor(*this);
        for (const auto& sub : sub_terms_) {
            sub->accept(std::forward<Visitor>(visitor));
        }
    }
    
private:
    std::string name_;
    std::string description_;
    std::optional<TriadicTerm> triadic_type_;
    TermList sub_terms_;
    Term* parent_ = nullptr;
};

/**
 * @brief The Enneagram structure (System 4)
 * 
 * The enneagram is a nine-pointed figure that represents
 * the primary creative process. It consists of a triangle
 * (representing the triad) inscribed in a circle with
 * specific internal connections.
 */
class Enneagram {
public:
    using TermPtr = std::shared_ptr<Term>;
    using EnneagramPtr = std::shared_ptr<Enneagram>;
    
    Enneagram() = default;
    explicit Enneagram(const std::string& name);
    
    /// Get the enneagram name
    const std::string& name() const { return name_; }
    
    /// Get term at a specific position (1-9)
    TermPtr termAt(EnneagramPosition pos) const;
    
    /// Set term at a specific position
    void setTermAt(EnneagramPosition pos, TermPtr term);
    
    /// Get the three triadic terms (positions 3, 6, 9)
    std::array<TermPtr, 3> triad() const;
    
    /// Get the six process terms (positions 1, 2, 4, 5, 7, 8)
    std::array<TermPtr, 6> process() const;
    
    /// Get all nine terms
    const std::array<TermPtr, 9>& terms() const { return terms_; }
    
    /// Check if this is a nested enneagram (contains sub-enneagrams)
    bool isNested() const { return nested_level_ > 0; }
    
    /// Get the nesting level
    size_t nestedLevel() const { return nested_level_; }
    
    /// Set nested enneagram at a position (for System 7+)
    void setNestedEnneagram(EnneagramPosition pos, EnneagramPtr ennea);
    
    /// Get nested enneagram at a position
    EnneagramPtr nestedEnneagramAt(EnneagramPosition pos) const;
    
    /// Get the internal lines of the enneagram (1-4-2-8-5-7-1 sequence)
    static std::vector<std::pair<int, int>> internalLines();
    
    /// Get the triangle lines (3-6-9-3)
    static std::vector<std::pair<int, int>> triangleLines();
    
private:
    std::string name_;
    std::array<TermPtr, 9> terms_;
    std::array<EnneagramPtr, 9> nested_enneagrams_;
    size_t nested_level_ = 0;
};

/**
 * @brief The System class represents a level in the cosmic hierarchy
 * 
 * Systems 0-10 form a nested hierarchy where term counts follow OEIS A000081
 * (number of rooted trees with n unlabeled nodes), where System n has A000081(n+1) terms.
 * Terms are grouped into clusters via flip transform, following OEIS A000055.
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
class System {
public:
    using SystemPtr = std::shared_ptr<System>;
    using TermPtr = std::shared_ptr<Term>;
    using EnneagramPtr = std::shared_ptr<Enneagram>;
    
    /// Create a system at the specified level (0-10)
    explicit System(int level);
    
    /// Get the system level
    int level() const { return level_; }
    
    /// Get the system name/title
    const std::string& name() const { return name_; }
    
    /// Get the system description
    const std::string& description() const { return description_; }
    
    /// Get the primary interface (System 1+)
    const Interface& primaryInterface() const { return primary_interface_; }
    Interface& primaryInterface() { return primary_interface_; }
    
    /// Get the secondary interface (System 2+)
    std::optional<Interface> secondaryInterface() const;
    
    /// Get the triadic terms (System 3+)
    std::optional<std::array<TermPtr, 3>> triad() const;
    
    /// Get the enneagram (System 4+)
    EnneagramPtr enneagram() const { return enneagram_; }
    
    /// Get the complementary enneagram (System 5+)
    EnneagramPtr complementaryEnneagram() const { return complementary_enneagram_; }
    
    /// Get the number of terms at this system level
    size_t termCount() const;
    
    /// Get all terms as a flat list
    std::vector<TermPtr> allTerms() const;
    
    /// Check if this system transcends another
    bool transcends(const System& other) const { return level_ < other.level_; }
    
    /// Check if this system subsumes another
    bool subsumes(const System& other) const { return level_ < other.level_; }
    
    /// Get the parent system (lower level that transcends this)
    SystemPtr parent() const { return parent_.lock(); }
    
    /// Get child systems (higher levels subsumed by this)
    const std::vector<SystemPtr>& children() const { return children_; }
    
    /// Build the complete system structure
    void build();
    
    /// Visitor pattern for traversal
    template<typename Visitor>
    void accept(Visitor&& visitor) const {
        visitor(*this);
        for (const auto& child : children_) {
            child->accept(std::forward<Visitor>(visitor));
        }
    }
    
    /// Factory method to create the complete System 0-10 hierarchy
    static SystemPtr createHierarchy();
    
    /// Get system by level from hierarchy
    static SystemPtr getSystem(SystemPtr root, int level);
    
    /// Get the number of clusters at this system level
    size_t clusterCount() const;
    
    /// Get the number of nodes (excluding root) at this system level
    size_t nodeCount() const;
    
private:
    void buildSystem0();
    void buildSystem1();
    void buildSystem2();
    void buildSystem3();
    void buildSystem4();
    void buildSystem5();
    void buildSystem6();
    void buildSystem7();
    void buildSystem8();
    void buildSystem9();
    void buildSystem10();
    
    TermPtr createTriadicTerm(TriadicTerm type, const std::string& context = "");
    EnneagramPtr createEnneagram(const std::string& name, bool withSubTerms = false);
    EnneagramPtr createNestedEnneagram(const std::string& name, int depth);
    
    int level_;
    std::string name_;
    std::string description_;
    Interface primary_interface_;
    std::optional<Interface> secondary_interface_;
    std::array<TermPtr, 3> triadic_terms_;
    EnneagramPtr enneagram_;
    EnneagramPtr complementary_enneagram_;
    std::weak_ptr<System> parent_;
    std::vector<SystemPtr> children_;
};

/**
 * @brief Utility functions for working with the System
 */
namespace util {

/// Convert TriadicTerm to string
std::string toString(TriadicTerm term);

/// Convert Orientation to string
std::string toString(Orientation orient);

/// Convert EnneagramPosition to string
std::string toString(EnneagramPosition pos);

/// Get the triadic term name for a given context
std::string triadicTermName(TriadicTerm term, const std::string& context);

/// Calculate the number of terms for a given system level (OEIS A000081)
size_t termCountForLevel(int level);

/// Calculate the number of clusters for a given system level (OEIS A000055)
size_t clusterCountForLevel(int level);

/// Calculate the number of nodes (excluding root) for a given system level
size_t nodeCountForLevel(int level);

/// Get the standard descriptions for System 3 cosmic movie
std::map<std::string, std::string> cosmicMovieDescriptions();

/// Get the standard descriptions for biological hierarchy
std::map<std::string, std::string> biologicalHierarchyDescriptions();

} // namespace util

} // namespace cosmic

#endif // COSMIC_SYSTEM_HPP
