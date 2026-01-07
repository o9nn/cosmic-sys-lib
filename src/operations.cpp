/**
 * @file operations.cpp
 * @brief Implementation of System operations and transformations
 */

#include "cosmic/operations.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace cosmic {
namespace ops {

// ============================================================================
// OrientationTransform Implementation
// ============================================================================

void OrientationTransform::transform(Interface& interface) {
    interface.transform();
}

void OrientationTransform::transformSystem(System& system) {
    system.primaryInterface().transform();
    // Note: Secondary interface transformation would be handled similarly
}

Orientation OrientationTransform::complement(Orientation orient) {
    return (orient == Orientation::Objective) 
        ? Orientation::Subjective 
        : Orientation::Objective;
}

bool OrientationTransform::areComplementary(Orientation a, Orientation b) {
    return a != b;
}

// ============================================================================
// TriadicCycle Implementation
// ============================================================================

TriadicTerm TriadicCycle::next(TriadicTerm current) {
    switch (current) {
        case TriadicTerm::Idea: return TriadicTerm::Routine;
        case TriadicTerm::Routine: return TriadicTerm::Form;
        case TriadicTerm::Form: return TriadicTerm::Idea;
    }
    return TriadicTerm::Idea;
}

TriadicTerm TriadicCycle::previous(TriadicTerm current) {
    switch (current) {
        case TriadicTerm::Idea: return TriadicTerm::Form;
        case TriadicTerm::Routine: return TriadicTerm::Idea;
        case TriadicTerm::Form: return TriadicTerm::Routine;
    }
    return TriadicTerm::Idea;
}

TriadicTerm TriadicCycle::complement(TriadicTerm current) {
    // In the triadic structure, the complement is the term that
    // mediates between the other two
    switch (current) {
        case TriadicTerm::Idea: return TriadicTerm::Routine;  // Form mediates
        case TriadicTerm::Routine: return TriadicTerm::Form;  // Idea mediates
        case TriadicTerm::Form: return TriadicTerm::Idea;     // Routine mediates
    }
    return TriadicTerm::Idea;
}

int TriadicCycle::position(TriadicTerm term) {
    switch (term) {
        case TriadicTerm::Idea: return 0;
        case TriadicTerm::Routine: return 1;
        case TriadicTerm::Form: return 2;
    }
    return 0;
}

TriadicTerm TriadicCycle::fromPosition(int pos) {
    switch (pos % 3) {
        case 0: return TriadicTerm::Idea;
        case 1: return TriadicTerm::Routine;
        case 2: return TriadicTerm::Form;
    }
    return TriadicTerm::Idea;
}

// ============================================================================
// EnneagramProcess Implementation
// ============================================================================

int EnneagramProcess::nextInSequence(int current) {
    // Find current position in sequence
    for (size_t i = 0; i < SEQUENCE.size(); ++i) {
        if (SEQUENCE[i] == current) {
            return SEQUENCE[(i + 1) % SEQUENCE.size()];
        }
    }
    return current;  // Not in sequence
}

int EnneagramProcess::previousInSequence(int current) {
    for (size_t i = 0; i < SEQUENCE.size(); ++i) {
        if (SEQUENCE[i] == current) {
            return SEQUENCE[(i + SEQUENCE.size() - 1) % SEQUENCE.size()];
        }
    }
    return current;
}

bool EnneagramProcess::isTrianglePosition(int pos) {
    return pos == 3 || pos == 6 || pos == 9;
}

bool EnneagramProcess::isHexadPosition(int pos) {
    return !isTrianglePosition(pos) && pos >= 1 && pos <= 9;
}

std::pair<int, int> EnneagramProcess::shockPoints() {
    // Positions 3 and 6 are where "shocks" (external inputs) are needed
    // to continue the process
    return {3, 6};
}

// ============================================================================
// SystemNavigator Implementation
// ============================================================================

SystemNavigator::SystemNavigator(SystemPtr root) : root_(root), current_(root) {}

bool SystemNavigator::goToLevel(int level) {
    auto target = systemAt(level);
    if (target) {
        current_ = target;
        return true;
    }
    return false;
}

bool SystemNavigator::goToParent() {
    auto parent = current_->parent();
    if (parent) {
        current_ = parent;
        return true;
    }
    return false;
}

bool SystemNavigator::goToChild(int index) {
    const auto& children = current_->children();
    if (index >= 0 && static_cast<size_t>(index) < children.size()) {
        current_ = children[index];
        return true;
    }
    return false;
}

bool SystemNavigator::goToSibling(int offset) {
    auto parent = current_->parent();
    if (!parent) return false;
    
    const auto& siblings = parent->children();
    auto it = std::find(siblings.begin(), siblings.end(), current_);
    if (it == siblings.end()) return false;
    
    int current_idx = std::distance(siblings.begin(), it);
    int new_idx = current_idx + offset;
    
    if (new_idx >= 0 && static_cast<size_t>(new_idx) < siblings.size()) {
        current_ = siblings[new_idx];
        return true;
    }
    return false;
}

SystemNavigator::SystemPtr SystemNavigator::systemAt(int level) const {
    return System::getSystem(root_, level);
}

std::vector<SystemNavigator::SystemPtr> SystemNavigator::allSystems() const {
    std::vector<SystemPtr> result;
    
    std::function<void(SystemPtr)> collect = [&](SystemPtr sys) {
        if (sys) {
            result.push_back(sys);
            for (const auto& child : sys->children()) {
                collect(child);
            }
        }
    };
    
    collect(root_);
    return result;
}

std::vector<SystemNavigator::SystemPtr> SystemNavigator::findSystems(
    std::function<bool(const System&)> predicate) const {
    
    std::vector<SystemPtr> result;
    auto all = allSystems();
    
    for (const auto& sys : all) {
        if (predicate(*sys)) {
            result.push_back(sys);
        }
    }
    
    return result;
}

// ============================================================================
// TermNavigator Implementation
// ============================================================================

TermNavigator::TermNavigator(TermPtr root) : root_(root), current_(root) {}

bool TermNavigator::goToParent() {
    Term* parent = current_->parent();
    if (parent) {
        // Need to find the shared_ptr for the parent
        // This is a limitation of the current design
        return false;  // Would need parent as shared_ptr
    }
    return false;
}

bool TermNavigator::goToChild(int index) {
    const auto& children = current_->subTerms();
    if (index >= 0 && static_cast<size_t>(index) < children.size()) {
        current_ = children[index];
        return true;
    }
    return false;
}

bool TermNavigator::goToSibling(int offset) {
    Term* parent = current_->parent();
    if (!parent) return false;
    
    const auto& siblings = parent->subTerms();
    auto it = std::find(siblings.begin(), siblings.end(), current_);
    if (it == siblings.end()) return false;
    
    int current_idx = std::distance(siblings.begin(), it);
    int new_idx = current_idx + offset;
    
    if (new_idx >= 0 && static_cast<size_t>(new_idx) < siblings.size()) {
        current_ = siblings[new_idx];
        return true;
    }
    return false;
}

int TermNavigator::depth() const {
    int d = 0;
    Term* p = current_->parent();
    while (p) {
        ++d;
        p = p->parent();
    }
    return d;
}

std::vector<TermNavigator::TermPtr> TermNavigator::findTerms(
    std::function<bool(const Term&)> predicate) const {
    
    std::vector<TermPtr> results;
    findTermsRecursive(root_, predicate, results);
    return results;
}

std::vector<TermNavigator::TermPtr> TermNavigator::findByType(TriadicTerm type) const {
    return findTerms([type](const Term& t) {
        return t.triadicType() && *t.triadicType() == type;
    });
}

void TermNavigator::findTermsRecursive(TermPtr term,
    std::function<bool(const Term&)>& predicate,
    std::vector<TermPtr>& results) const {
    
    if (!term) return;
    
    if (predicate(*term)) {
        results.push_back(term);
    }
    
    for (const auto& child : term->subTerms()) {
        findTermsRecursive(child, predicate, results);
    }
}

// ============================================================================
// SelfSimilarity Implementation
// ============================================================================

bool SelfSimilarity::sameStructure(const Term& a, const Term& b) {
    // Check if both have same triadic type
    if (a.triadicType() != b.triadicType()) {
        return false;
    }
    
    // Check if both have same number of sub-terms
    if (a.subTerms().size() != b.subTerms().size()) {
        return false;
    }
    
    // Recursively check sub-terms
    for (size_t i = 0; i < a.subTerms().size(); ++i) {
        if (!sameStructure(*a.subTerms()[i], *b.subTerms()[i])) {
            return false;
        }
    }
    
    return true;
}

bool SelfSimilarity::sameStructure(const Enneagram& a, const Enneagram& b) {
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        auto term_a = a.termAt(pos);
        auto term_b = b.termAt(pos);
        
        if ((term_a == nullptr) != (term_b == nullptr)) {
            return false;
        }
        
        if (term_a && term_b && !sameStructure(*term_a, *term_b)) {
            return false;
        }
    }
    return true;
}

double SelfSimilarity::fractalDimension(const Term& term) {
    // Calculate approximate fractal dimension based on self-similar nesting
    // D = log(N) / log(S) where N is number of self-similar copies and S is scale factor
    
    if (term.subTerms().empty()) {
        return 0.0;
    }
    
    // For the triadic structure, each term contains 3 sub-terms
    // at approximately 1/3 the scale
    size_t n = term.subTerms().size();
    if (n == 0) return 0.0;
    
    // Assuming scale factor of 3 for triadic nesting
    return std::log(static_cast<double>(n)) / std::log(3.0);
}

int SelfSimilarity::selfSimilarLevels(const Term& term) {
    if (term.subTerms().empty()) {
        return 1;
    }
    
    int max_levels = 0;
    for (const auto& sub : term.subTerms()) {
        max_levels = std::max(max_levels, selfSimilarLevels(*sub));
    }
    
    return max_levels + 1;
}

// ============================================================================
// Relationships Implementation
// ============================================================================

bool Relationships::hasRelation(const System& a, const System& b, RelationType type) {
    switch (type) {
        case RelationType::Transcends:
            return a.level() < b.level();
        case RelationType::Subsumes:
            return a.level() < b.level();
        case RelationType::Complements:
            return a.level() == b.level();  // Same level, different orientation
        case RelationType::Contains:
            return a.level() < b.level() && (b.level() - a.level() == 1);
        case RelationType::Elaborates:
            return a.level() > b.level() && (a.level() - b.level() == 1);
        default:
            return false;
    }
}

std::vector<Relationships::RelationType> Relationships::getRelations(
    const System& a, const System& b) {
    
    std::vector<RelationType> relations;
    
    if (a.level() < b.level()) {
        relations.push_back(RelationType::Transcends);
        relations.push_back(RelationType::Subsumes);
        if (b.level() - a.level() == 1) {
            relations.push_back(RelationType::Contains);
        }
    } else if (a.level() > b.level()) {
        if (a.level() - b.level() == 1) {
            relations.push_back(RelationType::Elaborates);
        }
    } else {
        relations.push_back(RelationType::Complements);
    }
    
    return relations;
}

bool Relationships::areConnected(int pos1, int pos2) {
    if (pos1 < 1 || pos1 > 9 || pos2 < 1 || pos2 > 9) {
        return false;
    }
    
    // Check triangle connections (3-6-9)
    if (EnneagramProcess::isTrianglePosition(pos1) && 
        EnneagramProcess::isTrianglePosition(pos2)) {
        return true;
    }
    
    // Check hexad connections
    auto seq = EnneagramProcess::SEQUENCE;
    for (size_t i = 0; i < seq.size(); ++i) {
        if (seq[i] == pos1) {
            int next = seq[(i + 1) % seq.size()];
            int prev = seq[(i + seq.size() - 1) % seq.size()];
            if (pos2 == next || pos2 == prev) {
                return true;
            }
        }
    }
    
    return false;
}

std::optional<Relationships::RelationType> Relationships::connectionType(int pos1, int pos2) {
    if (!areConnected(pos1, pos2)) {
        return std::nullopt;
    }
    
    if (EnneagramProcess::isTrianglePosition(pos1) && 
        EnneagramProcess::isTrianglePosition(pos2)) {
        return RelationType::Triangulates;
    }
    
    return RelationType::Transforms;
}

std::vector<int> Relationships::connectedPositions(int pos) {
    std::vector<int> connected;
    
    for (int i = 1; i <= 9; ++i) {
        if (i != pos && areConnected(pos, i)) {
            connected.push_back(i);
        }
    }
    
    return connected;
}

// ============================================================================
// CreativeProcess Implementation
// ============================================================================

CreativeProcess::CreativeProcess() : position_(1) {}

CreativeProcess::State CreativeProcess::state() const {
    switch (position_) {
        case 1: return State::Initiating;
        case 2:
        case 4: return State::Developing;
        case 3: return shock_applied_3_ ? State::Developing : State::FirstShock;
        case 5:
        case 7:
        case 8: return State::Maturing;
        case 6: return shock_applied_6_ ? State::Maturing : State::SecondShock;
        case 9: return State::Completing;
        default: return State::Initiating;
    }
}

bool CreativeProcess::advance() {
    if (isComplete()) return false;
    
    // Check if shock is needed
    if (needsShock()) {
        return false;  // Cannot advance without shock
    }
    
    // The creative process follows a specific path through the enneagram:
    // 1 -> 4 -> 2 -> (shock at 3) -> 8 -> 5 -> 7 -> (shock at 6) -> 9
    // We need to integrate the triangle positions (3, 6, 9) with the hexad
    
    switch (position_) {
        case 1: position_ = 4; break;
        case 4: position_ = 2; break;
        case 2: position_ = 3; break;  // First shock point
        case 3: position_ = 8; break;  // After first shock
        case 8: position_ = 5; break;
        case 5: position_ = 7; break;
        case 7: position_ = 6; break;  // Second shock point
        case 6: position_ = 9; break;  // After second shock - completion
        case 9: break;  // Already complete
    }
    
    return true;
}

bool CreativeProcess::applyShock() {
    if (position_ == 3 && !shock_applied_3_) {
        shock_applied_3_ = true;
        return true;
    }
    if (position_ == 6 && !shock_applied_6_) {
        shock_applied_6_ = true;
        return true;
    }
    return false;
}

bool CreativeProcess::needsShock() const {
    return (position_ == 3 && !shock_applied_3_) ||
           (position_ == 6 && !shock_applied_6_);
}

void CreativeProcess::reset() {
    position_ = 1;
    shock_applied_3_ = false;
    shock_applied_6_ = false;
}

std::string CreativeProcess::stateDescription() const {
    switch (state()) {
        case State::Initiating:
            return "Initiating: The process begins with an initial impulse or intention.";
        case State::Developing:
            return "Developing: The process develops through internal dynamics.";
        case State::FirstShock:
            return "First Shock: External input is needed to continue development.";
        case State::Maturing:
            return "Maturing: The process matures toward completion.";
        case State::SecondShock:
            return "Second Shock: External input is needed to reach completion.";
        case State::Completing:
            return "Completing: The process reaches integration and completion.";
    }
    return "Unknown state";
}

// ============================================================================
// Serializer Implementation
// ============================================================================

std::string Serializer::toJSON(const System& system) {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"level\": " << system.level() << ",\n";
    ss << "  \"name\": \"" << system.name() << "\",\n";
    ss << "  \"description\": \"" << system.description() << "\",\n";
    ss << "  \"termCount\": " << system.termCount() << ",\n";
    ss << "  \"orientation\": \"" << util::toString(system.primaryInterface().orientation()) << "\"";
    
    auto triad = system.triad();
    if (triad) {
        ss << ",\n  \"triad\": [\n";
        for (size_t i = 0; i < 3; ++i) {
            if ((*triad)[i]) {
                ss << "    \"" << (*triad)[i]->name() << "\"";
                if (i < 2) ss << ",";
                ss << "\n";
            }
        }
        ss << "  ]";
    }
    
    ss << "\n}";
    return ss.str();
}

std::string Serializer::toJSON(const Term& term) {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"name\": \"" << term.name() << "\"";
    
    if (term.triadicType()) {
        ss << ",\n  \"type\": \"" << util::toString(*term.triadicType()) << "\"";
    }
    
    if (!term.description().empty()) {
        ss << ",\n  \"description\": \"" << term.description() << "\"";
    }
    
    if (!term.subTerms().empty()) {
        ss << ",\n  \"subTerms\": [\n";
        for (size_t i = 0; i < term.subTerms().size(); ++i) {
            ss << "    " << toJSON(*term.subTerms()[i]);
            if (i < term.subTerms().size() - 1) ss << ",";
            ss << "\n";
        }
        ss << "  ]";
    }
    
    ss << "\n}";
    return ss.str();
}

std::string Serializer::toJSON(const Enneagram& ennea) {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"name\": \"" << ennea.name() << "\",\n";
    ss << "  \"positions\": {\n";
    
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        auto term = ennea.termAt(pos);
        ss << "    \"" << i << "\": ";
        if (term) {
            ss << "\"" << term->name() << "\"";
        } else {
            ss << "null";
        }
        if (i < 9) ss << ",";
        ss << "\n";
    }
    
    ss << "  }\n}";
    return ss.str();
}

std::string Serializer::hierarchyToJSON(SystemPtr root) {
    if (!root) return "null";
    
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"system\": " << toJSON(*root);
    
    if (!root->children().empty()) {
        ss << ",\n  \"children\": [\n";
        for (size_t i = 0; i < root->children().size(); ++i) {
            ss << hierarchyToJSON(root->children()[i]);
            if (i < root->children().size() - 1) ss << ",";
            ss << "\n";
        }
        ss << "  ]";
    }
    
    ss << "\n}";
    return ss.str();
}

std::string Serializer::toDOT(const System& system) {
    std::ostringstream ss;
    ss << "digraph System" << system.level() << " {\n";
    ss << "  label=\"" << system.name() << "\";\n";
    ss << "  node [shape=ellipse];\n";
    
    auto triad = system.triad();
    if (triad) {
        for (const auto& term : *triad) {
            if (term) {
                ss << "  \"" << term->name() << "\";\n";
            }
        }
        
        // Add cycle edges
        if ((*triad)[0] && (*triad)[1]) {
            ss << "  \"" << (*triad)[0]->name() << "\" -> \"" << (*triad)[1]->name() << "\";\n";
        }
        if ((*triad)[1] && (*triad)[2]) {
            ss << "  \"" << (*triad)[1]->name() << "\" -> \"" << (*triad)[2]->name() << "\";\n";
        }
        if ((*triad)[2] && (*triad)[0]) {
            ss << "  \"" << (*triad)[2]->name() << "\" -> \"" << (*triad)[0]->name() << "\";\n";
        }
    }
    
    ss << "}\n";
    return ss.str();
}

std::string Serializer::toDOT(const Enneagram& ennea) {
    std::ostringstream ss;
    ss << "digraph Enneagram {\n";
    ss << "  label=\"" << ennea.name() << "\";\n";
    ss << "  node [shape=circle];\n";
    
    // Add nodes
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        auto term = ennea.termAt(pos);
        std::string label = term ? term->name() : std::to_string(i);
        ss << "  p" << i << " [label=\"" << label << "\"];\n";
    }
    
    // Add triangle edges
    ss << "  p3 -> p6 [color=red];\n";
    ss << "  p6 -> p9 [color=red];\n";
    ss << "  p9 -> p3 [color=red];\n";
    
    // Add hexad edges
    ss << "  p1 -> p4 [color=blue];\n";
    ss << "  p4 -> p2 [color=blue];\n";
    ss << "  p2 -> p8 [color=blue];\n";
    ss << "  p8 -> p5 [color=blue];\n";
    ss << "  p5 -> p7 [color=blue];\n";
    ss << "  p7 -> p1 [color=blue];\n";
    
    ss << "}\n";
    return ss.str();
}

std::string Serializer::hierarchyToDOT(SystemPtr root) {
    std::ostringstream ss;
    ss << "digraph SystemHierarchy {\n";
    ss << "  rankdir=TB;\n";
    ss << "  node [shape=box];\n";
    
    std::function<void(SystemPtr)> addNodes = [&](SystemPtr sys) {
        if (!sys) return;
        ss << "  s" << sys->level() << " [label=\"" << sys->name() << "\"];\n";
        for (const auto& child : sys->children()) {
            addNodes(child);
        }
    };
    
    std::function<void(SystemPtr)> addEdges = [&](SystemPtr sys) {
        if (!sys) return;
        for (const auto& child : sys->children()) {
            ss << "  s" << sys->level() << " -> s" << child->level() << ";\n";
            addEdges(child);
        }
    };
    
    addNodes(root);
    addEdges(root);
    
    ss << "}\n";
    return ss.str();
}

} // namespace ops
} // namespace cosmic
