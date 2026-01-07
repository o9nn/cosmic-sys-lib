/**
 * @file system.cpp
 * @brief Implementation of the System hierarchy classes
 */

#include "cosmic/system.hpp"
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace cosmic {

// ============================================================================
// Interface Implementation
// ============================================================================

Interface::Interface(const std::string& name, Orientation orient)
    : name_(name), orientation_(orient), active_(true) {}

void Interface::transform() {
    orientation_ = (orientation_ == Orientation::Objective) 
        ? Orientation::Subjective 
        : Orientation::Objective;
}

// ============================================================================
// Term Implementation
// ============================================================================

Term::Term(const std::string& name) : name_(name) {}

Term::Term(const std::string& name, TriadicTerm type)
    : name_(name), triadic_type_(type) {}

void Term::addSubTerm(TermPtr term) {
    term->parent_ = this;
    sub_terms_.push_back(term);
}

size_t Term::depth() const {
    size_t max_depth = 0;
    for (const auto& sub : sub_terms_) {
        max_depth = std::max(max_depth, sub->depth());
    }
    return max_depth + 1;
}

size_t Term::totalTermCount() const {
    size_t count = 1;
    for (const auto& sub : sub_terms_) {
        count += sub->totalTermCount();
    }
    return count;
}

// ============================================================================
// Enneagram Implementation
// ============================================================================

Enneagram::Enneagram(const std::string& name) : name_(name) {
    // Initialize all terms to nullptr
    terms_.fill(nullptr);
    nested_enneagrams_.fill(nullptr);
}

Term::TermPtr Enneagram::termAt(EnneagramPosition pos) const {
    int idx = static_cast<int>(pos) - 1;
    if (idx < 0 || idx >= 9) {
        throw std::out_of_range("Enneagram position must be 1-9");
    }
    return terms_[idx];
}

void Enneagram::setTermAt(EnneagramPosition pos, Term::TermPtr term) {
    int idx = static_cast<int>(pos) - 1;
    if (idx < 0 || idx >= 9) {
        throw std::out_of_range("Enneagram position must be 1-9");
    }
    terms_[idx] = term;
}

std::array<Term::TermPtr, 3> Enneagram::triad() const {
    return {
        terms_[2],  // Position 3
        terms_[5],  // Position 6
        terms_[8]   // Position 9
    };
}

std::array<Term::TermPtr, 6> Enneagram::process() const {
    return {
        terms_[0],  // Position 1
        terms_[1],  // Position 2
        terms_[3],  // Position 4
        terms_[4],  // Position 5
        terms_[6],  // Position 7
        terms_[7]   // Position 8
    };
}

void Enneagram::setNestedEnneagram(EnneagramPosition pos, EnneagramPtr ennea) {
    int idx = static_cast<int>(pos) - 1;
    if (idx < 0 || idx >= 9) {
        throw std::out_of_range("Enneagram position must be 1-9");
    }
    nested_enneagrams_[idx] = ennea;
    if (ennea) {
        nested_level_ = std::max(nested_level_, ennea->nested_level_ + 1);
    }
}

Enneagram::EnneagramPtr Enneagram::nestedEnneagramAt(EnneagramPosition pos) const {
    int idx = static_cast<int>(pos) - 1;
    if (idx < 0 || idx >= 9) {
        throw std::out_of_range("Enneagram position must be 1-9");
    }
    return nested_enneagrams_[idx];
}

std::vector<std::pair<int, int>> Enneagram::internalLines() {
    // The internal lines follow the sequence 1-4-2-8-5-7-1
    // This represents the law of seven (octave)
    return {
        {1, 4}, {4, 2}, {2, 8}, {8, 5}, {5, 7}, {7, 1}
    };
}

std::vector<std::pair<int, int>> Enneagram::triangleLines() {
    // The triangle connects positions 3-6-9
    // This represents the law of three (triad)
    return {
        {3, 6}, {6, 9}, {9, 3}
    };
}

// ============================================================================
// System Implementation
// ============================================================================

System::System(int level) : level_(level) {
    if (level < 0 || level > 10) {
        throw std::invalid_argument("System level must be between 0 and 10");
    }
    
    // Set system names and descriptions based on level
    // Term counts follow OEIS A000081(n+1), clusters follow OEIS A000055(n+1)
    switch (level) {
        case 0:
            name_ = "System 0";
            description_ = "The Void - root only, primordial unity before differentiation (1 term, 1 cluster)";
            break;
        case 1:
            name_ = "System 1";
            description_ = "Universal Wholeness - active interface between inside and outside (1 term, 1 cluster)";
            break;
        case 2:
            name_ = "System 2";
            description_ = "The Fundamental Dyad - objective and subjective modes (2 terms, 1 cluster)";
            break;
        case 3:
            name_ = "System 3";
            description_ = "The Primary Activity - four terms in two clusters (4 terms, 2 clusters)";
            break;
        case 4:
            name_ = "System 4";
            description_ = "The Enneagram - nine terms in three clusters (9 terms, 3 clusters)";
            break;
        case 5:
            name_ = "System 5";
            description_ = "Complementary Structures - twenty terms in six clusters (20 terms, 6 clusters)";
            break;
        case 6:
            name_ = "System 6";
            description_ = "Primary Activity of Enneagrams - 48 terms in 11 clusters";
            break;
        case 7:
            name_ = "System 7";
            description_ = "Enneagram of Enneagrams - 115 terms in 23 clusters";
            break;
        case 8:
            name_ = "System 8";
            description_ = "Nested Complementarity - 286 terms in 47 clusters";
            break;
        case 9:
            name_ = "System 9";
            description_ = "Deep Nesting - 719 terms in 106 clusters";
            break;
        case 10:
            name_ = "System 10";
            description_ = "Full Recursive Elaboration - 1842 terms in 235 clusters";
            break;
    }
}

std::optional<Interface> System::secondaryInterface() const {
    return secondary_interface_;
}

std::optional<std::array<Term::TermPtr, 3>> System::triad() const {
    if (level_ < 3) {
        return std::nullopt;
    }
    return triadic_terms_;
}

size_t System::termCount() const {
    return util::termCountForLevel(level_);
}

std::vector<Term::TermPtr> System::allTerms() const {
    std::vector<Term::TermPtr> result;
    
    // Collect from triadic terms
    for (const auto& term : triadic_terms_) {
        if (term) {
            term->accept([&result](const Term& t) {
                // Note: We need to store shared_ptr, this is a simplified version
            });
            result.push_back(term);
        }
    }
    
    // Collect from enneagram
    if (enneagram_) {
        for (const auto& term : enneagram_->terms()) {
            if (term) {
                result.push_back(term);
            }
        }
    }
    
    return result;
}

size_t System::clusterCount() const {
    return util::clusterCountForLevel(level_);
}

size_t System::nodeCount() const {
    return util::nodeCountForLevel(level_);
}

void System::build() {
    switch (level_) {
        case 0: buildSystem0(); break;
        case 1: buildSystem1(); break;
        case 2: buildSystem2(); break;
        case 3: buildSystem3(); break;
        case 4: buildSystem4(); break;
        case 5: buildSystem5(); break;
        case 6: buildSystem6(); break;
        case 7: buildSystem7(); break;
        case 8: buildSystem8(); break;
        case 9: buildSystem9(); break;
        case 10: buildSystem10(); break;
    }
}

void System::buildSystem0() {
    // System 0: The Void - root only, primordial unity
    // 1 term (the void itself), 1 cluster, 0 nodes
    primary_interface_ = Interface("Void Interface", Orientation::Objective);
    primary_interface_.setActive(false);  // Inactive - before differentiation
}

void System::buildSystem1() {
    // System 1: Universal wholeness with single active interface
    primary_interface_ = Interface("Universal Interface", Orientation::Objective);
    primary_interface_.setActive(true);
}

void System::buildSystem2() {
    // System 2: Two interfaces - objective and subjective
    buildSystem1();
    
    primary_interface_ = Interface("Universal Interface", Orientation::Objective);
    secondary_interface_ = Interface("Particular Interface", Orientation::Subjective);
}

void System::buildSystem3() {
    // System 3: The triadic structure - Idea, Routine, Form
    buildSystem2();
    
    // Create the three primary terms
    triadic_terms_[0] = createTriadicTerm(TriadicTerm::Idea, "Galaxy");
    triadic_terms_[1] = createTriadicTerm(TriadicTerm::Routine, "Sun");
    triadic_terms_[2] = createTriadicTerm(TriadicTerm::Form, "Planet");
    
    // Each triadic term contains nested Idea/Routine/Form
    for (auto& term : triadic_terms_) {
        auto idea = std::make_shared<Term>("Idea", TriadicTerm::Idea);
        auto routine = std::make_shared<Term>("Routine", TriadicTerm::Routine);
        auto form = std::make_shared<Term>("Form", TriadicTerm::Form);
        
        term->addSubTerm(idea);
        term->addSubTerm(routine);
        term->addSubTerm(form);
    }
    
    // Set descriptions from the cosmic movie
    auto descriptions = util::cosmicMovieDescriptions();
    
    // Galaxy-Idea descriptions
    if (triadic_terms_[0] && triadic_terms_[0]->subTerms().size() >= 3) {
        triadic_terms_[0]->subTerms()[0]->setDescription(descriptions["galaxy_idea"]);
        triadic_terms_[0]->subTerms()[1]->setDescription(descriptions["galaxy_routine"]);
        triadic_terms_[0]->subTerms()[2]->setDescription(descriptions["galaxy_form"]);
    }
    
    // Sun-Routine descriptions
    if (triadic_terms_[1] && triadic_terms_[1]->subTerms().size() >= 3) {
        triadic_terms_[1]->subTerms()[0]->setDescription(descriptions["sun_idea"]);
        triadic_terms_[1]->subTerms()[1]->setDescription(descriptions["sun_routine"]);
        triadic_terms_[1]->subTerms()[2]->setDescription(descriptions["sun_form"]);
    }
    
    // Planet-Form descriptions
    if (triadic_terms_[2] && triadic_terms_[2]->subTerms().size() >= 3) {
        triadic_terms_[2]->subTerms()[0]->setDescription(descriptions["planet_idea"]);
        triadic_terms_[2]->subTerms()[1]->setDescription(descriptions["planet_routine"]);
        triadic_terms_[2]->subTerms()[2]->setDescription(descriptions["planet_form"]);
    }
}

void System::buildSystem4() {
    // System 4: The enneagram - 9 terms in the creative process
    buildSystem3();
    
    enneagram_ = createEnneagram("Primary Enneagram", true);
}

void System::buildSystem5() {
    // System 5: Complementary objective and subjective enneagrams
    buildSystem4();
    
    complementary_enneagram_ = createEnneagram("Complementary Enneagram", true);
    
    // The complementary enneagram has opposite orientation
    // This represents the objective/subjective duality at the enneagram level
}

void System::buildSystem6() {
    // System 6: Primary activity of enneagrams
    // Each of the three triadic positions contains an enneagram
    buildSystem5();
    
    // Create three enneagrams for Idea, Routine, Form
    auto idea_ennea = createEnneagram("Idea Enneagram", true);
    auto routine_ennea = createEnneagram("Routine Enneagram", true);
    auto form_ennea = createEnneagram("Form Enneagram", true);
    
    // Store in the triadic positions of the main enneagram
    enneagram_->setNestedEnneagram(EnneagramPosition::Three, idea_ennea);
    enneagram_->setNestedEnneagram(EnneagramPosition::Six, routine_ennea);
    enneagram_->setNestedEnneagram(EnneagramPosition::Nine, form_ennea);
}

void System::buildSystem7() {
    // System 7: Enneagram of enneagrams
    // Each of the 9 positions contains an enneagram
    buildSystem6();
    
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        std::string name = "Enneagram " + std::to_string(i);
        auto nested = createEnneagram(name, true);
        enneagram_->setNestedEnneagram(pos, nested);
    }
}

void System::buildSystem8() {
    // System 8: Objective and subjective enneagrams of enneagrams
    buildSystem7();
    
    // Create complementary nested structure
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        std::string name = "Complementary Enneagram " + std::to_string(i);
        auto nested = createEnneagram(name, true);
        complementary_enneagram_->setNestedEnneagram(pos, nested);
    }
}

void System::buildSystem9() {
    // System 9: Each term is an enneagram of enneagrams
    buildSystem8();
    
    // Create deeper nesting - each position gets nested enneagrams
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        auto outer = enneagram_->nestedEnneagramAt(pos);
        if (outer) {
            for (int j = 1; j <= 9; ++j) {
                auto inner_pos = static_cast<EnneagramPosition>(j);
                std::string name = "Nested " + std::to_string(i) + "-" + std::to_string(j);
                auto inner = createEnneagram(name, false);
                outer->setNestedEnneagram(inner_pos, inner);
            }
        }
    }
}

void System::buildSystem10() {
    // System 10: Full recursive nesting - each term an enneagram of enneagrams
    buildSystem9();
    
    // This represents the maximum elaboration shown in the diagram
    // Further nesting could continue indefinitely, but System 10 represents
    // the practical limit of the visualization
}

Term::TermPtr System::createTriadicTerm(TriadicTerm type, const std::string& context) {
    std::string name = util::triadicTermName(type, context);
    return std::make_shared<Term>(name, type);
}

Enneagram::EnneagramPtr System::createEnneagram(const std::string& name, bool withSubTerms) {
    auto ennea = std::make_shared<Enneagram>(name);
    
    // Create terms for each position
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        std::string term_name = "Term " + std::to_string(i);
        auto term = std::make_shared<Term>(term_name);
        
        // Positions 3, 6, 9 are the triadic positions
        if (i == 3) {
            term = std::make_shared<Term>("Idea", TriadicTerm::Idea);
        } else if (i == 6) {
            term = std::make_shared<Term>("Routine", TriadicTerm::Routine);
        } else if (i == 9) {
            term = std::make_shared<Term>("Form", TriadicTerm::Form);
        }
        
        if (withSubTerms) {
            // Add nested triadic structure
            term->addSubTerm(std::make_shared<Term>("Sub-Idea", TriadicTerm::Idea));
            term->addSubTerm(std::make_shared<Term>("Sub-Routine", TriadicTerm::Routine));
            term->addSubTerm(std::make_shared<Term>("Sub-Form", TriadicTerm::Form));
        }
        
        ennea->setTermAt(pos, term);
    }
    
    return ennea;
}

Enneagram::EnneagramPtr System::createNestedEnneagram(const std::string& name, int depth) {
    auto ennea = createEnneagram(name, depth > 0);
    
    if (depth > 0) {
        for (int i = 1; i <= 9; ++i) {
            auto pos = static_cast<EnneagramPosition>(i);
            std::string nested_name = name + "." + std::to_string(i);
            auto nested = createNestedEnneagram(nested_name, depth - 1);
            ennea->setNestedEnneagram(pos, nested);
        }
    }
    
    return ennea;
}

System::SystemPtr System::createHierarchy() {
    // Create all 10 systems
    std::vector<SystemPtr> systems;
    for (int i = 1; i <= 10; ++i) {
        auto sys = std::make_shared<System>(i);
        sys->build();
        systems.push_back(sys);
    }
    
    // Link parent-child relationships
    // Lower systems transcend and subsume higher systems
    for (int i = 0; i < 9; ++i) {
        systems[i]->children_.push_back(systems[i + 1]);
        systems[i + 1]->parent_ = systems[i];
    }
    
    return systems[0];  // Return System 1 as root
}

System::SystemPtr System::getSystem(SystemPtr root, int level) {
    if (!root) return nullptr;
    if (root->level() == level) return root;
    
    for (const auto& child : root->children()) {
        auto found = getSystem(child, level);
        if (found) return found;
    }
    
    return nullptr;
}

// ============================================================================
// Utility Functions
// ============================================================================

namespace util {

std::string toString(TriadicTerm term) {
    switch (term) {
        case TriadicTerm::Idea: return "Idea";
        case TriadicTerm::Routine: return "Routine";
        case TriadicTerm::Form: return "Form";
    }
    return "Unknown";
}

std::string toString(Orientation orient) {
    switch (orient) {
        case Orientation::Objective: return "Objective";
        case Orientation::Subjective: return "Subjective";
    }
    return "Unknown";
}

std::string toString(EnneagramPosition pos) {
    return std::to_string(static_cast<int>(pos));
}

std::string triadicTermName(TriadicTerm term, const std::string& context) {
    std::string base = toString(term);
    if (context.empty()) {
        return base;
    }
    return context + " - " + base;
}

size_t termCountForLevel(int level) {
    // Term counts follow OEIS A000081 (number of rooted trees with n unlabeled nodes)
    // System n has A000081(n+1) terms
    // A000081: 0, 1, 1, 2, 4, 9, 20, 48, 115, 286, 719, 1842, ...
    
    static const size_t A000081[] = {
        0,      // a(0) - not used
        1,      // a(1) - System 0: 1 term (void)
        1,      // a(2) - System 1: 1 term
        2,      // a(3) - System 2: 2 terms
        4,      // a(4) - System 3: 4 terms
        9,      // a(5) - System 4: 9 terms
        20,     // a(6) - System 5: 20 terms
        48,     // a(7) - System 6: 48 terms
        115,    // a(8) - System 7: 115 terms
        286,    // a(9) - System 8: 286 terms
        719,    // a(10) - System 9: 719 terms
        1842    // a(11) - System 10: 1842 terms
    };
    
    if (level < 0 || level > 10) return 0;
    return A000081[level + 1];
}

size_t clusterCountForLevel(int level) {
    // Cluster counts follow OEIS A000055 (number of unrooted trees with n unlabeled nodes)
    // System n has A000055(n+1) clusters under flip transform
    // A000055: 1, 1, 1, 1, 2, 3, 6, 11, 23, 47, 106, 235, ...
    
    static const size_t A000055[] = {
        1,      // a(0) - empty tree
        1,      // a(1) - System 0: 1 cluster
        1,      // a(2) - System 1: 1 cluster
        1,      // a(3) - System 2: 1 cluster
        2,      // a(4) - System 3: 2 clusters
        3,      // a(5) - System 4: 3 clusters
        6,      // a(6) - System 5: 6 clusters
        11,     // a(7) - System 6: 11 clusters
        23,     // a(8) - System 7: 23 clusters
        47,     // a(9) - System 8: 47 clusters
        106,    // a(10) - System 9: 106 clusters
        235     // a(11) - System 10: 235 clusters
    };
    
    if (level < 0 || level > 10) return 0;
    return A000055[level + 1];
}

size_t nodeCountForLevel(int level) {
    // Node count (excluding root) = level
    if (level < 0) return 0;
    return static_cast<size_t>(level);
}

std::map<std::string, std::string> cosmicMovieDescriptions() {
    return {
        {"galaxy_idea", 
         "The integrating idea of a galaxy must retain synchronicity with the "
         "universal projection of hydrogen. This is done via black holes in their "
         "centers. This singular condition common to all galaxies links them by "
         "quantum forces. Integration regulates relative angular and linear motions."},
        
        {"galaxy_routine",
         "Routine cyclic motions in galaxies cause dissynchronicity with the primary "
         "projection of hydrogen. This space-time contraction in galactic interiors is "
         "partly offset by spatial contraction of hydrogen into heavy atoms by nuclear "
         "fusion in centers of stars. Space frame skipping leaves a central black hole."},
        
        {"galaxy_form",
         "Galactic integration, via angular momentum, winds up nuclear fusion in "
         "stars, as gravitational unit forms synchronous with the whole. Stars contract "
         "in clouds ejected from galactic centers, move out, then recycle back to the "
         "center, drawn by spatial contraction through maturing into heavy atoms."},
        
        {"sun_idea",
         "The integrating idea of stars retains synchronicity with the universal "
         "projection of hydrogen by contracting space into heavier elements. This "
         "partly offsets the skipping of space frames due to galactic rotation. Solar "
         "system momentum is likewise directed by quantum forces through reflux."},
        
        {"sun_routine",
         "Routines altering momentum in stars and planets adjust for spatial gaps due "
         "to atomic fusion in suns, radioactive decay in planets, & galactic motions. "
         "This maintains synchronous integrity in solar systems, always monitored by "
         "electromagnetic factors linked direct to the primary projection of hydrogen."},
        
        {"sun_form",
         "The patterned form of cyclic motions and electromagnetic order in suns and "
         "planets introduces less pronounced contractions in space & time. The "
         "cascading focus shifts to exploring many synchronous forms of molecular "
         "chemistry in widely varied planets and moons. Atoms marry up."},
        
        {"planet_idea",
         "The electromagnetic and gravitational form of the sun relates via cyclic "
         "routines to events in planets and moons, all linked to galactic order. This "
         "directs the chemical integration of planets as synchronous ideas consistent "
         "with the primary projection of hydrogen in the cosmic movie."},
        
        {"planet_routine",
         "Planets are bathed in solar electromagnetic energy, modulated in patterns by "
         "cyclic routines of rotation & lunar and solar revolutions. Cyclic routines, "
         "electromagnetic fields, core currents, and plate tectonics, are adjusted by "
         "reflux on a planetary scale to maintain synchronicity via quantum forces."},
        
        {"planet_form",
         "The diverse chemical integration of planets, via galactic, solar & planet "
         "routines, fosters biospheric evolution of life if possible. It is probably "
         "seeded by spores from an interstellar gene pool, eternally linked to the "
         "galaxy. Life evolves to transcending awareness of the eternal cosmic order."}
    };
}

std::map<std::string, std::string> biologicalHierarchyDescriptions() {
    return {
        {"humans_idea",
         "Future delegation of cosmic ideation will open the human mind to levels of "
         "realization as yet undreamed of, with a new balance throughout the hierarchy."},
        
        {"humans_knowledge",
         "Delegation of direct knowledge of cosmic order requires a new paradigm for "
         "science. The three focal points of mentation must balance in the biosphere."},
        
        {"humans_routine",
         "Expansionist empires fueled western science & industrial routines that now "
         "dominate the planet through huge corporations, threatening global resources."},
        
        {"humans_form",
         "Spirit cultures explored the planet. Cities brought division of labor & writing. "
         "Three forms of ideation focused through Eastern, Western & African cultures."},
        
        {"vertebrates_idea",
         "African primates evolved through anthropoids & hominids to humans. Speech "
         "polarizes left and right brain. Limbic emotion fuels abstract idea for behavior."},
        
        {"vertebrates_knowledge",
         "Higher mammals, dog, seal, etc., can select behavior. Topology of neocortex "
         "used to intuit action in knowledge. Ancient limbic system controls emotion."},
        
        {"vertebrates_routine",
         "Lower mammals, horse, cow, etc., have limited capacity to modulate emotive "
         "routines. Mesocortex blooms. Marsupial counterparts lack a corpus callosum."},
        
        {"vertebrates_form",
         "Reptiles explore quadruped form. Autonomic nervous system reflects emotive "
         "patterns specific to each species in cerebral awareness. Archicortex blooms."},
        
        {"invertebrates_idea",
         "Ants, bees, etc., use the idea of division of labor for their collective survival. "
         "The giant squid's developed brain employs ideas for its individual survival."},
        
        {"invertebrates_knowledge",
         "Flying insects rapidly integrate extended knowledge in flight routines. Most "
         "span time via metamorphosis. Spiders & some crustaceans span time & space."},
        
        {"invertebrates_routine",
         "Segmented worms integrate successive routines. Centipedes colonize land. "
         "Arthropods specialize body segments. Cephalopods & mollusks unsegmented."},
        
        {"invertebrates_form",
         "Sponges, jelly fish, coral, flatworms, nematodes, starfish, & chordates explore "
         "forms of routine in motor-sensory responses, with embryo developments."},
        
        {"plants_idea",
         "Flowering plants (angiosperms) with refined vascular systems, use extended "
         "ideas to attract animal pollinating vectors, and to produce fruit for dispersal."},
        
        {"plants_knowledge",
         "Gymnosperms integrate knowledge uniting the gametophyte generation within "
         "the sporophyte in pollen and seeds, allowing conifers to live in dry terrain."},
        
        {"plants_routine",
         "Giant horsetails & clubmosses on land explore routines with vascular systems "
         "and alternate sporophyte and gametophyte generations, leaving us coalbeds."},
        
        {"plants_form",
         "Algae, fungi, slime molds & lichens explore the forms of the eukaryotic cell, "
         "from microscopic to giant. Alternate sexual and asexual generations emerge."}
    };
}

} // namespace util

} // namespace cosmic
