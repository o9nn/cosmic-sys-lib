/**
 * @file test_operations.cpp
 * @brief Tests for the operations and transformations
 */

#include <iostream>
#include <cassert>
#include "cosmic/cosmic.hpp"

using namespace cosmic;
using namespace cosmic::ops;

void test_orientation_transform() {
    std::cout << "Testing OrientationTransform..." << std::endl;
    
    // Test complement
    assert(OrientationTransform::complement(Orientation::Objective) == Orientation::Subjective);
    assert(OrientationTransform::complement(Orientation::Subjective) == Orientation::Objective);
    
    // Test areComplementary
    assert(OrientationTransform::areComplementary(Orientation::Objective, Orientation::Subjective));
    assert(!OrientationTransform::areComplementary(Orientation::Objective, Orientation::Objective));
    
    // Test interface transform
    Interface iface("Test", Orientation::Objective);
    OrientationTransform::transform(iface);
    assert(iface.orientation() == Orientation::Subjective);
    
    std::cout << "  PASSED" << std::endl;
}

void test_triadic_cycle() {
    std::cout << "Testing TriadicCycle..." << std::endl;
    
    // Test next
    assert(TriadicCycle::next(TriadicTerm::Idea) == TriadicTerm::Routine);
    assert(TriadicCycle::next(TriadicTerm::Routine) == TriadicTerm::Form);
    assert(TriadicCycle::next(TriadicTerm::Form) == TriadicTerm::Idea);
    
    // Test previous
    assert(TriadicCycle::previous(TriadicTerm::Idea) == TriadicTerm::Form);
    assert(TriadicCycle::previous(TriadicTerm::Routine) == TriadicTerm::Idea);
    assert(TriadicCycle::previous(TriadicTerm::Form) == TriadicTerm::Routine);
    
    // Test position
    assert(TriadicCycle::position(TriadicTerm::Idea) == 0);
    assert(TriadicCycle::position(TriadicTerm::Routine) == 1);
    assert(TriadicCycle::position(TriadicTerm::Form) == 2);
    
    // Test fromPosition
    assert(TriadicCycle::fromPosition(0) == TriadicTerm::Idea);
    assert(TriadicCycle::fromPosition(1) == TriadicTerm::Routine);
    assert(TriadicCycle::fromPosition(2) == TriadicTerm::Form);
    assert(TriadicCycle::fromPosition(3) == TriadicTerm::Idea);  // Wraps around
    
    // Test cycle
    int count = 0;
    TriadicCycle::cycle([&count](TriadicTerm term, int idx) {
        count++;
        assert(idx == TriadicCycle::position(term));
    });
    assert(count == 3);
    
    std::cout << "  PASSED" << std::endl;
}

void test_enneagram_process() {
    std::cout << "Testing EnneagramProcess..." << std::endl;
    
    // Test sequence
    assert(EnneagramProcess::SEQUENCE[0] == 1);
    assert(EnneagramProcess::SEQUENCE[1] == 4);
    assert(EnneagramProcess::SEQUENCE[2] == 2);
    assert(EnneagramProcess::SEQUENCE[3] == 8);
    assert(EnneagramProcess::SEQUENCE[4] == 5);
    assert(EnneagramProcess::SEQUENCE[5] == 7);
    
    // Test nextInSequence
    assert(EnneagramProcess::nextInSequence(1) == 4);
    assert(EnneagramProcess::nextInSequence(4) == 2);
    assert(EnneagramProcess::nextInSequence(7) == 1);  // Wraps around
    
    // Test previousInSequence
    assert(EnneagramProcess::previousInSequence(4) == 1);
    assert(EnneagramProcess::previousInSequence(1) == 7);  // Wraps around
    
    // Test triangle positions
    assert(EnneagramProcess::isTrianglePosition(3));
    assert(EnneagramProcess::isTrianglePosition(6));
    assert(EnneagramProcess::isTrianglePosition(9));
    assert(!EnneagramProcess::isTrianglePosition(1));
    assert(!EnneagramProcess::isTrianglePosition(5));
    
    // Test hexad positions
    assert(EnneagramProcess::isHexadPosition(1));
    assert(EnneagramProcess::isHexadPosition(2));
    assert(EnneagramProcess::isHexadPosition(4));
    assert(!EnneagramProcess::isHexadPosition(3));
    assert(!EnneagramProcess::isHexadPosition(9));
    
    // Test shock points
    auto shocks = EnneagramProcess::shockPoints();
    assert(shocks.first == 3);
    assert(shocks.second == 6);
    
    std::cout << "  PASSED" << std::endl;
}

void test_system_navigator() {
    std::cout << "Testing SystemNavigator..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    SystemNavigator nav(hierarchy);
    
    // Test initial state
    assert(nav.root() == hierarchy);
    assert(nav.current() == hierarchy);
    assert(nav.current()->level() == 1);
    
    // Test goToLevel
    assert(nav.goToLevel(5));
    assert(nav.current()->level() == 5);
    
    assert(nav.goToLevel(1));
    assert(nav.current()->level() == 1);
    
    // Test goToChild
    assert(nav.goToChild(0));
    assert(nav.current()->level() == 2);
    
    // Test goToParent
    assert(nav.goToParent());
    assert(nav.current()->level() == 1);
    
    // Test systemAt
    auto sys7 = nav.systemAt(7);
    assert(sys7 != nullptr);
    assert(sys7->level() == 7);
    
    // Test allSystems
    auto all = nav.allSystems();
    assert(all.size() == 10);
    
    // Test findSystems
    auto found = nav.findSystems([](const System& s) {
        return s.level() > 5;
    });
    assert(found.size() == 5);  // Systems 6-10
    
    std::cout << "  PASSED" << std::endl;
}

void test_term_navigator() {
    std::cout << "Testing TermNavigator..." << std::endl;
    
    auto root = std::make_shared<Term>("Root", TriadicTerm::Idea);
    auto child1 = std::make_shared<Term>("Child1", TriadicTerm::Routine);
    auto child2 = std::make_shared<Term>("Child2", TriadicTerm::Form);
    auto grandchild = std::make_shared<Term>("Grandchild", TriadicTerm::Idea);
    
    root->addSubTerm(child1);
    root->addSubTerm(child2);
    child1->addSubTerm(grandchild);
    
    TermNavigator nav(root);
    
    // Test initial state
    assert(nav.root() == root);
    assert(nav.current() == root);
    
    // Test goToChild
    assert(nav.goToChild(0));
    assert(nav.current() == child1);
    
    // Test depth
    assert(nav.depth() == 1);
    
    // Test goToChild again
    assert(nav.goToChild(0));
    assert(nav.current() == grandchild);
    assert(nav.depth() == 2);
    
    // Test findByType
    nav = TermNavigator(root);
    auto ideas = nav.findByType(TriadicTerm::Idea);
    assert(ideas.size() == 2);  // Root and Grandchild
    
    std::cout << "  PASSED" << std::endl;
}

void test_relationships() {
    std::cout << "Testing Relationships..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    auto sys1 = System::getSystem(hierarchy, 1);
    auto sys2 = System::getSystem(hierarchy, 2);
    auto sys5 = System::getSystem(hierarchy, 5);
    
    // Test hasRelation
    assert(Relationships::hasRelation(*sys1, *sys2, Relationships::RelationType::Transcends));
    assert(Relationships::hasRelation(*sys1, *sys2, Relationships::RelationType::Contains));
    assert(!Relationships::hasRelation(*sys2, *sys1, Relationships::RelationType::Transcends));
    
    // Test getRelations
    auto relations = Relationships::getRelations(*sys1, *sys5);
    assert(!relations.empty());
    
    // Test areConnected
    assert(Relationships::areConnected(3, 6));  // Triangle
    assert(Relationships::areConnected(6, 9));  // Triangle
    assert(Relationships::areConnected(1, 4));  // Hexad
    assert(Relationships::areConnected(4, 2));  // Hexad
    assert(!Relationships::areConnected(1, 3)); // Not connected
    
    // Test connectionType
    auto conn_type = Relationships::connectionType(3, 6);
    assert(conn_type.has_value());
    assert(*conn_type == Relationships::RelationType::Triangulates);
    
    conn_type = Relationships::connectionType(1, 4);
    assert(conn_type.has_value());
    assert(*conn_type == Relationships::RelationType::Transforms);
    
    // Test connectedPositions
    auto connected = Relationships::connectedPositions(3);
    assert(connected.size() == 2);  // 6 and 9
    
    connected = Relationships::connectedPositions(1);
    assert(connected.size() == 2);  // 4 and 7
    
    std::cout << "  PASSED" << std::endl;
}

void test_creative_process() {
    std::cout << "Testing CreativeProcess..." << std::endl;
    
    CreativeProcess process;
    
    // Test initial state
    assert(process.position() == 1);
    assert(process.state() == CreativeProcess::State::Initiating);
    assert(!process.isComplete());
    assert(!process.needsShock());
    
    // Advance through the process (with shock points at 3 and 6)
    process.advance();  // 1 -> 4
    assert(process.position() == 4);
    
    process.advance();  // 4 -> 2
    assert(process.position() == 2);
    
    process.advance();  // 2 -> 3 (first shock point)
    assert(process.position() == 3);
    assert(process.needsShock());
    
    // Apply first shock to continue
    process.applyShock();
    assert(!process.needsShock());
    
    process.advance();  // 3 -> 8
    assert(process.position() == 8);
    
    process.advance();  // 8 -> 5
    assert(process.position() == 5);
    
    process.advance();  // 5 -> 7
    assert(process.position() == 7);
    
    process.advance();  // 7 -> 6 (second shock point)
    assert(process.position() == 6);
    assert(process.needsShock());
    
    // Apply second shock to continue
    process.applyShock();
    assert(!process.needsShock());
    
    process.advance();  // 6 -> 9 (completion)
    assert(process.position() == 9);
    assert(process.isComplete());
    
    // Test reset
    process.reset();
    assert(process.position() == 1);
    
    // Test state description
    std::string desc = process.stateDescription();
    assert(!desc.empty());
    assert(desc.find("Initiating") != std::string::npos);
    
    std::cout << "  PASSED" << std::endl;
}

void test_serializer() {
    std::cout << "Testing Serializer..." << std::endl;
    
    auto hierarchy = System::createHierarchy();
    auto sys3 = System::getSystem(hierarchy, 3);
    
    // Test System JSON
    std::string json = Serializer::toJSON(*sys3);
    assert(!json.empty());
    assert(json.find("level") != std::string::npos);
    assert(json.find("System 3") != std::string::npos);
    
    // Test Term JSON
    auto triad = sys3->triad();
    if (triad && (*triad)[0]) {
        std::string term_json = Serializer::toJSON(*(*triad)[0]);
        assert(!term_json.empty());
        assert(term_json.find("name") != std::string::npos);
    }
    
    // Test Enneagram JSON
    auto sys4 = System::getSystem(hierarchy, 4);
    if (sys4 && sys4->enneagram()) {
        std::string ennea_json = Serializer::toJSON(*sys4->enneagram());
        assert(!ennea_json.empty());
        assert(ennea_json.find("positions") != std::string::npos);
    }
    
    // Test DOT output
    std::string dot = Serializer::toDOT(*sys3);
    assert(!dot.empty());
    assert(dot.find("digraph") != std::string::npos);
    
    // Test hierarchy DOT
    std::string hier_dot = Serializer::hierarchyToDOT(hierarchy);
    assert(!hier_dot.empty());
    assert(hier_dot.find("digraph") != std::string::npos);
    
    std::cout << "  PASSED" << std::endl;
}

void test_self_similarity() {
    std::cout << "Testing SelfSimilarity..." << std::endl;
    
    auto term1 = std::make_shared<Term>("A", TriadicTerm::Idea);
    auto term2 = std::make_shared<Term>("B", TriadicTerm::Idea);
    auto term3 = std::make_shared<Term>("C", TriadicTerm::Routine);
    
    // Same structure (both empty)
    assert(SelfSimilarity::sameStructure(*term1, *term2));
    
    // Different type
    assert(!SelfSimilarity::sameStructure(*term1, *term3));
    
    // Add sub-terms
    term1->addSubTerm(std::make_shared<Term>("A1"));
    assert(!SelfSimilarity::sameStructure(*term1, *term2));
    
    term2->addSubTerm(std::make_shared<Term>("B1"));
    assert(SelfSimilarity::sameStructure(*term1, *term2));
    
    // Test self-similar levels
    int levels = SelfSimilarity::selfSimilarLevels(*term1);
    assert(levels == 2);
    
    std::cout << "  PASSED" << std::endl;
}

int main() {
    std::cout << "=== Operations Tests ===" << std::endl;
    
    test_orientation_transform();
    test_triadic_cycle();
    test_enneagram_process();
    test_system_navigator();
    test_term_navigator();
    test_relationships();
    test_creative_process();
    test_serializer();
    test_self_similarity();
    
    std::cout << "\nAll tests PASSED!" << std::endl;
    return 0;
}
