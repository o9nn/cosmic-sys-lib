/**
 * @file test_system.cpp
 * @brief Tests for the System hierarchy classes
 */

#include <iostream>
#include <cassert>
#include "cosmic/cosmic.hpp"

using namespace cosmic;

void test_system_creation() {
    std::cout << "Testing System creation..." << std::endl;
    
    // Test valid system levels
    for (int level = 1; level <= 10; ++level) {
        System sys(level);
        assert(sys.level() == level);
        sys.build();
        assert(sys.termCount() > 0);
    }
    
    // Test invalid system level
    // System 0 is now valid (The Void)
    System sys0(0);
    assert(sys0.level() == 0);
    
    bool threw = false;
    try {
        System sys(11);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    
    std::cout << "  PASSED" << std::endl;
}

void test_hierarchy_creation() {
    std::cout << "Testing hierarchy creation..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    assert(hierarchy != nullptr);
    assert(hierarchy->level() == 1);
    
    // Check all levels are accessible
    for (int level = 1; level <= 10; ++level) {
        auto sys = System::getSystem(hierarchy, level);
        assert(sys != nullptr);
        assert(sys->level() == level);
    }
    
    // Check parent-child relationships
    for (int level = 1; level < 10; ++level) {
        auto sys = System::getSystem(hierarchy, level);
        assert(!sys->children().empty());
        assert(sys->children()[0]->level() == level + 1);
    }
    
    std::cout << "  PASSED" << std::endl;
}

void test_system_triad() {
    std::cout << "Testing System 3 triad..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    auto sys3 = System::getSystem(hierarchy, 3);
    
    assert(sys3 != nullptr);
    auto triad = sys3->triad();
    assert(triad.has_value());
    
    // Check all three terms exist
    for (const auto& term : *triad) {
        assert(term != nullptr);
        assert(term->triadicType().has_value());
    }
    
    // Check triadic types
    assert(*(*triad)[0]->triadicType() == TriadicTerm::Idea);
    assert(*(*triad)[1]->triadicType() == TriadicTerm::Routine);
    assert(*(*triad)[2]->triadicType() == TriadicTerm::Form);
    
    // Check nested structure
    for (const auto& term : *triad) {
        assert(term->subTerms().size() == 3);
    }
    
    std::cout << "  PASSED" << std::endl;
}

void test_system_enneagram() {
    std::cout << "Testing System 4 enneagram..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    auto sys4 = System::getSystem(hierarchy, 4);
    
    assert(sys4 != nullptr);
    auto ennea = sys4->enneagram();
    assert(ennea != nullptr);
    
    // Check all 9 positions have terms
    for (int i = 1; i <= 9; ++i) {
        auto pos = static_cast<EnneagramPosition>(i);
        auto term = ennea->termAt(pos);
        assert(term != nullptr);
    }
    
    // Check triangle positions (3, 6, 9)
    auto tri = ennea->triad();
    assert(tri[0] != nullptr);  // Position 3
    assert(tri[1] != nullptr);  // Position 6
    assert(tri[2] != nullptr);  // Position 9
    
    // Check process positions
    auto proc = ennea->process();
    for (const auto& term : proc) {
        assert(term != nullptr);
    }
    
    std::cout << "  PASSED" << std::endl;
}

void test_interface() {
    std::cout << "Testing Interface..." << std::endl;
    
    Interface iface("Test", Orientation::Objective);
    assert(iface.name() == "Test");
    assert(iface.orientation() == Orientation::Objective);
    assert(iface.isActive());
    
    iface.transform();
    assert(iface.orientation() == Orientation::Subjective);
    
    iface.transform();
    assert(iface.orientation() == Orientation::Objective);
    
    iface.setActive(false);
    assert(!iface.isActive());
    
    std::cout << "  PASSED" << std::endl;
}

void test_term() {
    std::cout << "Testing Term..." << std::endl;
    
    Term term("Test Term", TriadicTerm::Idea);
    assert(term.name() == "Test Term");
    assert(term.triadicType().has_value());
    assert(*term.triadicType() == TriadicTerm::Idea);
    
    // Test sub-terms
    auto sub1 = std::make_shared<Term>("Sub 1");
    auto sub2 = std::make_shared<Term>("Sub 2");
    term.addSubTerm(sub1);
    term.addSubTerm(sub2);
    
    assert(term.subTerms().size() == 2);
    assert(term.hasSubTerms());
    assert(sub1->parent() == &term);
    assert(sub2->parent() == &term);
    
    // Test depth
    assert(term.depth() == 2);
    assert(term.totalTermCount() == 3);
    
    std::cout << "  PASSED" << std::endl;
}

void test_term_count() {
    std::cout << "Testing term counts (OEIS A000081)..." << std::endl;
    
    // Term counts follow OEIS A000081(n+1)
    // System n has A000081(n+1) terms
    assert(util::termCountForLevel(0) == 1);    // A000081(1) = 1
    assert(util::termCountForLevel(1) == 1);    // A000081(2) = 1
    assert(util::termCountForLevel(2) == 2);    // A000081(3) = 2
    assert(util::termCountForLevel(3) == 4);    // A000081(4) = 4
    assert(util::termCountForLevel(4) == 9);    // A000081(5) = 9
    assert(util::termCountForLevel(5) == 20);   // A000081(6) = 20
    assert(util::termCountForLevel(6) == 48);   // A000081(7) = 48
    assert(util::termCountForLevel(7) == 115);  // A000081(8) = 115
    assert(util::termCountForLevel(8) == 286);  // A000081(9) = 286
    assert(util::termCountForLevel(9) == 719);  // A000081(10) = 719
    assert(util::termCountForLevel(10) == 1842); // A000081(11) = 1842
    
    std::cout << "  PASSED" << std::endl;
}

void test_util_functions() {
    std::cout << "Testing utility functions..." << std::endl;
    
    assert(util::toString(TriadicTerm::Idea) == "Idea");
    assert(util::toString(TriadicTerm::Routine) == "Routine");
    assert(util::toString(TriadicTerm::Form) == "Form");
    
    assert(util::toString(Orientation::Objective) == "Objective");
    assert(util::toString(Orientation::Subjective) == "Subjective");
    
    auto cosmic_desc = util::cosmicMovieDescriptions();
    assert(!cosmic_desc.empty());
    assert(cosmic_desc.count("galaxy_idea") > 0);
    
    std::cout << "  PASSED" << std::endl;
}

int main() {
    std::cout << "=== System Tests ===" << std::endl;
    
    test_system_creation();
    test_hierarchy_creation();
    test_system_triad();
    test_system_enneagram();
    test_interface();
    test_term();
    test_term_count();
    test_util_functions();
    
    std::cout << "\nAll tests PASSED!" << std::endl;
    return 0;
}
