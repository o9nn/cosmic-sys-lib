/**
 * @file operations.hpp
 * @brief Operations and transformations for the System hierarchy
 * 
 * This file provides operations for working with Systems, including:
 * - Transformations between objective and subjective orientations
 * - Navigation through the nested hierarchy
 * - Pattern matching and term relationships
 * - The creative process cycle
 */

#ifndef COSMIC_OPERATIONS_HPP
#define COSMIC_OPERATIONS_HPP

#include "system.hpp"
#include <functional>
#include <optional>

namespace cosmic {
namespace ops {

// ============================================================================
// Transformation Operations
// ============================================================================

/**
 * @brief Transform between objective and subjective orientations
 * 
 * System 2 introduces the fundamental alternation between objective
 * (passive, from outside looking in) and subjective (active, from
 * inside looking out) orientations.
 */
class OrientationTransform {
public:
    /// Transform a single interface
    static void transform(Interface& interface);
    
    /// Transform all interfaces in a system
    static void transformSystem(System& system);
    
    /// Get the complementary orientation
    static Orientation complement(Orientation orient);
    
    /// Check if two orientations are complementary
    static bool areComplementary(Orientation a, Orientation b);
};

/**
 * @brief The triadic transformation cycle (Idea -> Routine -> Form -> Idea)
 * 
 * System 3 introduces the creative cycle where:
 * - Idea (universal) gives rise to
 * - Routine (particular processes) which manifest as
 * - Form (integrated patterns) which inform new Ideas
 */
class TriadicCycle {
public:
    /// Get the next term in the cycle
    static TriadicTerm next(TriadicTerm current);
    
    /// Get the previous term in the cycle
    static TriadicTerm previous(TriadicTerm current);
    
    /// Get the complementary term (opposite in the cycle)
    static TriadicTerm complement(TriadicTerm current);
    
    /// Get the position in the cycle (0, 1, 2)
    static int position(TriadicTerm term);
    
    /// Get term from position
    static TriadicTerm fromPosition(int pos);
    
    /// Execute a full cycle, calling the visitor for each term
    template<typename Visitor>
    static void cycle(Visitor&& visitor, TriadicTerm start = TriadicTerm::Idea) {
        TriadicTerm current = start;
        for (int i = 0; i < 3; ++i) {
            visitor(current, i);
            current = next(current);
        }
    }
};

/**
 * @brief The enneagram process sequence (1-4-2-8-5-7-1)
 * 
 * System 4 introduces the law of seven, represented by the internal
 * hexad of the enneagram. This sequence represents the stages of
 * any process or transformation.
 */
class EnneagramProcess {
public:
    /// The process sequence positions
    static constexpr std::array<int, 6> SEQUENCE = {1, 4, 2, 8, 5, 7};
    
    /// The triangle positions (law of three)
    static constexpr std::array<int, 3> TRIANGLE = {3, 6, 9};
    
    /// Get the next position in the process sequence
    static int nextInSequence(int current);
    
    /// Get the previous position in the process sequence
    static int previousInSequence(int current);
    
    /// Check if a position is on the triangle (3, 6, 9)
    static bool isTrianglePosition(int pos);
    
    /// Check if a position is on the hexad (1, 2, 4, 5, 7, 8)
    static bool isHexadPosition(int pos);
    
    /// Get the shock points (positions 3 and 6 where external input is needed)
    static std::pair<int, int> shockPoints();
    
    /// Execute the full process sequence
    template<typename Visitor>
    static void process(Visitor&& visitor) {
        for (int i = 0; i < 6; ++i) {
            visitor(SEQUENCE[i], i);
        }
    }
    
    /// Execute the triangle sequence
    template<typename Visitor>
    static void triangle(Visitor&& visitor) {
        for (int i = 0; i < 3; ++i) {
            visitor(TRIANGLE[i], i);
        }
    }
};

// ============================================================================
// Navigation Operations
// ============================================================================

/**
 * @brief Navigate through the System hierarchy
 */
class SystemNavigator {
public:
    using SystemPtr = System::SystemPtr;
    using TermPtr = Term::TermPtr;
    
    explicit SystemNavigator(SystemPtr root);
    
    /// Get the root system (System 1)
    SystemPtr root() const { return root_; }
    
    /// Get current system
    SystemPtr current() const { return current_; }
    
    /// Navigate to a specific system level
    bool goToLevel(int level);
    
    /// Navigate to parent (lower system number)
    bool goToParent();
    
    /// Navigate to child (higher system number)
    bool goToChild(int index = 0);
    
    /// Navigate to sibling
    bool goToSibling(int offset);
    
    /// Get system at specific level
    SystemPtr systemAt(int level) const;
    
    /// Get all systems as a list
    std::vector<SystemPtr> allSystems() const;
    
    /// Find systems matching a predicate
    std::vector<SystemPtr> findSystems(
        std::function<bool(const System&)> predicate) const;
    
private:
    SystemPtr root_;
    SystemPtr current_;
};

/**
 * @brief Navigate through Terms within a System
 */
class TermNavigator {
public:
    using TermPtr = Term::TermPtr;
    
    explicit TermNavigator(TermPtr root);
    
    /// Get the root term
    TermPtr root() const { return root_; }
    
    /// Get current term
    TermPtr current() const { return current_; }
    
    /// Navigate to parent term
    bool goToParent();
    
    /// Navigate to child term by index
    bool goToChild(int index);
    
    /// Navigate to sibling term
    bool goToSibling(int offset);
    
    /// Get the path from root to current
    std::vector<TermPtr> pathFromRoot() const;
    
    /// Get depth of current term
    int depth() const;
    
    /// Find terms matching a predicate
    std::vector<TermPtr> findTerms(
        std::function<bool(const Term&)> predicate) const;
    
    /// Find terms by triadic type
    std::vector<TermPtr> findByType(TriadicTerm type) const;
    
private:
    TermPtr root_;
    TermPtr current_;
    
    void findTermsRecursive(TermPtr term,
        std::function<bool(const Term&)>& predicate,
        std::vector<TermPtr>& results) const;
};

// ============================================================================
// Pattern Operations
// ============================================================================

/**
 * @brief Self-similarity patterns in the System
 * 
 * The System exhibits self-similarity at multiple levels, where
 * the same triadic and enneagram patterns repeat at different scales.
 */
class SelfSimilarity {
public:
    using TermPtr = Term::TermPtr;
    using EnneagramPtr = Enneagram::EnneagramPtr;
    
    /// Check if two terms have the same structure
    static bool sameStructure(const Term& a, const Term& b);
    
    /// Check if two enneagrams have the same structure
    static bool sameStructure(const Enneagram& a, const Enneagram& b);
    
    /// Get the fractal dimension of a nested structure
    static double fractalDimension(const Term& term);
    
    /// Count the number of self-similar levels
    static int selfSimilarLevels(const Term& term);
    
    /// Find all instances of a pattern within a term
    static std::vector<TermPtr> findPattern(
        const Term& root, const Term& pattern);
};

/**
 * @brief Relationships between terms and positions
 */
class Relationships {
public:
    using TermPtr = Term::TermPtr;
    
    /// The types of relationships in the System
    enum class RelationType {
        Transcends,      ///< Lower system transcends higher
        Subsumes,        ///< Lower system subsumes higher
        Complements,     ///< Objective/subjective complement
        Transforms,      ///< Process sequence connection
        Triangulates,    ///< Triangle (law of three) connection
        Contains,        ///< Parent contains child
        Elaborates       ///< Child elaborates on parent
    };
    
    /// Check if a relationship exists between two systems
    static bool hasRelation(const System& a, const System& b, RelationType type);
    
    /// Get all relationships between two systems
    static std::vector<RelationType> getRelations(
        const System& a, const System& b);
    
    /// Check if two enneagram positions are connected
    static bool areConnected(int pos1, int pos2);
    
    /// Get the type of connection between enneagram positions
    static std::optional<RelationType> connectionType(int pos1, int pos2);
    
    /// Get all positions connected to a given position
    static std::vector<int> connectedPositions(int pos);
};

// ============================================================================
// Creative Process Operations
// ============================================================================

/**
 * @brief The creative process as described in the System
 * 
 * The creative process follows the enneagram sequence, with
 * "shocks" at positions 3 and 6 where external input is required
 * to continue the process.
 */
class CreativeProcess {
public:
    /// Process state
    enum class State {
        Initiating,    ///< Position 1 - beginning
        Developing,    ///< Positions 2, 4 - development
        FirstShock,    ///< Position 3 - first external input needed
        Maturing,      ///< Positions 5, 7, 8 - maturation
        SecondShock,   ///< Position 6 - second external input needed
        Completing     ///< Position 9 - completion/integration
    };
    
    CreativeProcess();
    
    /// Get current position (1-9)
    int position() const { return position_; }
    
    /// Get current state
    State state() const;
    
    /// Advance to next position
    bool advance();
    
    /// Apply a shock (external input) at current position
    bool applyShock();
    
    /// Check if shock is needed at current position
    bool needsShock() const;
    
    /// Reset to beginning
    void reset();
    
    /// Check if process is complete
    bool isComplete() const { return position_ == 9; }
    
    /// Get description of current state
    std::string stateDescription() const;
    
private:
    int position_ = 1;
    bool shock_applied_3_ = false;
    bool shock_applied_6_ = false;
};

// ============================================================================
// Serialization Operations
// ============================================================================

/**
 * @brief Serialize and deserialize System structures
 */
class Serializer {
public:
    using SystemPtr = System::SystemPtr;
    using TermPtr = Term::TermPtr;
    using EnneagramPtr = Enneagram::EnneagramPtr;
    
    /// Serialize a System to JSON string
    static std::string toJSON(const System& system);
    
    /// Serialize a Term to JSON string
    static std::string toJSON(const Term& term);
    
    /// Serialize an Enneagram to JSON string
    static std::string toJSON(const Enneagram& ennea);
    
    /// Serialize the complete hierarchy to JSON
    static std::string hierarchyToJSON(SystemPtr root);
    
    /// Export System to DOT format (for Graphviz)
    static std::string toDOT(const System& system);
    
    /// Export Enneagram to DOT format
    static std::string toDOT(const Enneagram& ennea);
    
    /// Export hierarchy to DOT format
    static std::string hierarchyToDOT(SystemPtr root);
};

} // namespace ops
} // namespace cosmic

#endif // COSMIC_OPERATIONS_HPP
