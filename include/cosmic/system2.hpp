/**
 * @file system2.hpp
 * @brief Detailed C++ model of System 2: Perceptive Wholeness
 * 
 * System 2 elaborates upon System 1 with two related active interfaces to
 * account for multiplicity. One interface is universal and unique, while
 * the other is particular and many.
 * 
 * Based on Robert Campbell's "Fisherman's Guide to the Cosmic Order" and
 * "Physics & Cosmic Order II: Introducing the System"
 * 
 * Key concepts from Fisherman's Guide:
 * - Two centers: Universal (unique, Center 1) and Particular (many, Center 2)
 * - Two modes: Objective (sequential, expressive) and Subjective (simultaneous, regenerative)
 * - Perceptual Transposition: Center 2 alternately looks out to sky, then turns to face Center 1
 * - Coalescence: Two centers achieve mutual identity in light (Z arrow in diagrams)
 * - Relational Wholes: R₁ and R₂ formed by each center's perception of identity
 * - Vesica Piscis: The geometric form of two overlapping circles
 * - Electromagnetic connection: Frequency/wavelength determined by center partitioning
 * 
 * Diagram Elements (Figures 12, 13, 19 from Fisherman's Guide):
 * - L₀: Light at universal center
 * - L₁: Light at particular center  
 * - D: Darkness at shared periphery
 * - R₁, R₂: Relational wholes
 * - Z arrow: Coalescence (mutual identity)
 * 
 * Mathematical alignment: System 2 has 2 terms (A000081(3) = 2)
 */

#pragma once

#include <cmath>
#include <string>
#include <functional>
#include <vector>
#include <array>
#include <stdexcept>
#include <sstream>

namespace cosmic {
namespace system2 {

/**
 * @brief Light intensity levels in System 2
 * 
 * From Fisherman's Guide: Different gradations of energy exist between
 * the universal center (L₀) and the particular center (L₁).
 */
struct LightLevel {
    static constexpr double L0 = 1.0;   ///< Universal light (maximum intensity)
    static constexpr double L1 = 0.8;   ///< Particular light (reduced intensity)
    static constexpr double D = 0.0;    ///< Darkness (zero intensity)
};

/**
 * @brief The two modes of perception in System 2
 * 
 * From Fisherman's Guide:
 * - OBJECTIVE: Sequential energy processes, expressive, timelike succession to sky
 * - SUBJECTIVE: Simultaneous and countercurrent, regenerative, mutual coalescence
 */
enum class Mode {
    OBJECTIVE,   ///< Sequential, expressive - Center 2 looks out to sky
    SUBJECTIVE   ///< Simultaneous, regenerative - Center 2 faces Center 1
};

/**
 * @brief The two orientations in System 2
 */
enum class Orientation {
    UNIVERSAL,   ///< Universal aspect - unique, one (Center 1)
    PARTICULAR   ///< Particular aspect - many, diverse (Center 2)
};

/**
 * @brief Represents a relational whole formed by a center
 * 
 * From Fisherman's Guide Figure 19: "Each center, through its perception
 * of identity, forms what will be called a relational whole. The relational
 * whole formed by Center 1 is designated as R₁; that formed by Center 2 as R₂."
 */
class RelationalWhole {
public:
    RelationalWhole(Orientation origin, double identity_strength = 0.5)
        : origin_(origin)
        , identity_strength_(identity_strength) {}
    
    /// Get the origin center
    Orientation origin() const { return origin_; }
    
    /// Get the identity strength (how strongly the center perceives identity)
    double identityStrength() const { return identity_strength_; }
    
    /// Set the identity strength
    void setIdentityStrength(double value) { identity_strength_ = std::clamp(value, 0.0, 1.0); }
    
    /// Get symbol (R₁ or R₂)
    std::string symbol() const {
        return origin_ == Orientation::UNIVERSAL ? "R₁" : "R₂";
    }

private:
    Orientation origin_;
    double identity_strength_;
};

/**
 * @brief Represents a center in System 2
 * 
 * System 2 has two centers: one universal (unique) and one particular (many).
 * Each center has both objective and subjective aspects.
 * 
 * From Fisherman's Guide: "If there were no particular aspects, there could
 * be no universal aspects, nor vice versa."
 */
class Center {
public:
    /**
     * @brief Construct a center
     * @param orientation Whether this is the universal or particular center
     * @param intensity The active intensity (default: 0.5 for balanced dyad)
     */
    Center(Orientation orientation, double intensity = 0.5)
        : orientation_(orientation)
        , intensity_(intensity)
        , objective_weight_(0.5)
        , subjective_weight_(0.5)
        , relational_whole_(orientation) {}
    
    /// Get the orientation (universal or particular)
    Orientation orientation() const { return orientation_; }
    
    /// Get the intensity (Light level)
    double intensity() const { return intensity_; }
    
    /// Get the Light symbol
    std::string lightSymbol() const {
        return orientation_ == Orientation::UNIVERSAL ? "L₀" : "L₁";
    }
    
    /// Set the intensity
    void setIntensity(double value) { intensity_ = value; }
    
    /// Get the objective weight (0 to 1)
    double objectiveWeight() const { return objective_weight_; }
    
    /// Get the subjective weight (0 to 1)
    double subjectiveWeight() const { return subjective_weight_; }
    
    /**
     * @brief Set the mode balance
     * @param objective_weight Weight of objective mode (0 to 1)
     * 
     * Subjective weight is automatically set to (1 - objective_weight)
     */
    void setModeBalance(double objective_weight) {
        objective_weight_ = std::clamp(objective_weight, 0.0, 1.0);
        subjective_weight_ = 1.0 - objective_weight_;
    }
    
    /// Get the dominant mode
    Mode dominantMode() const {
        return objective_weight_ > subjective_weight_ ? Mode::OBJECTIVE : Mode::SUBJECTIVE;
    }
    
    /// Get the mode polarity (-1 = fully subjective, +1 = fully objective)
    double modePolarity() const {
        return objective_weight_ - subjective_weight_;
    }
    
    /// Get the relational whole formed by this center
    RelationalWhole& relationalWhole() { return relational_whole_; }
    const RelationalWhole& relationalWhole() const { return relational_whole_; }
    
    /// Get canonical representation
    std::string canonical() const {
        return orientation_ == Orientation::UNIVERSAL ? "U" : "P";
    }

private:
    Orientation orientation_;
    double intensity_;
    double objective_weight_;
    double subjective_weight_;
    RelationalWhole relational_whole_;
};

/**
 * @brief Represents the coalescence between two centers
 * 
 * From Fisherman's Guide Figure 19: "Two centers exist independently of one
 * another, they mutually achieve identity through mutual perception. They are
 * mutually perceived as one but are two. It will be said that the two are
 * mutually coalesced as one, and the coalescence is designated as shown by
 * the large Z arrow."
 */
class Coalescence {
public:
    Coalescence() : strength_(0.0), active_(false) {}
    
    /// Get the coalescence strength (0 = none, 1 = full mutual identity)
    double strength() const { return strength_; }
    
    /// Check if coalescence is active (subjective mode)
    bool isActive() const { return active_; }
    
    /**
     * @brief Update coalescence based on mode
     * 
     * Coalescence is active in subjective mode (centers face each other)
     * and inactive in objective mode (centers look outward to sky)
     */
    void update(Mode mode, double center1_intensity, double center2_intensity) {
        active_ = (mode == Mode::SUBJECTIVE);
        if (active_) {
            // Coalescence strength based on mutual intensity
            strength_ = std::sqrt(center1_intensity * center2_intensity);
        } else {
            strength_ = 0.0;
        }
    }
    
    /// Get the Z arrow symbol
    std::string symbol() const { return "Z"; }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "Coalescence represents the mutual identity achieved by two centers "
               "in the subjective mode. The two relational wholes are simultaneous "
               "and countercurrent. Just as centers have a common periphery in darkness, "
               "they also find a mutual identity in light.";
    }

private:
    double strength_;
    bool active_;
};

/**
 * @brief Represents the perceptual transposition of Center 2
 * 
 * From Fisherman's Guide: "The two modes to the perceptive-wholeness term are
 * maintained in a state of dynamic balance through what will be called the
 * perceptual transposition of Center 2. Center 2 alternately contains Center 1
 * to look out directly to sky, then turns around and faces Center 1 to achieve
 * an identity with it in light."
 */
class PerceptualTransposition {
public:
    /**
     * @brief Construct perceptual transposition
     * @param rate Rate of transposition (frequency of mode alternation)
     */
    explicit PerceptualTransposition(double rate = 0.1)
        : rate_(rate)
        , phase_(0.0)
        , cycle_count_(0) {}
    
    /// Get the transposition rate
    double rate() const { return rate_; }
    
    /// Set the transposition rate
    void setRate(double rate) { rate_ = rate; }
    
    /// Get the current phase (0 to 2π)
    double phase() const { return phase_; }
    
    /// Get the number of complete cycles
    int cycleCount() const { return cycle_count_; }
    
    /**
     * @brief Advance the transposition by one step
     * @param dt Time step
     * @return The new phase
     */
    double step(double dt = 1.0) {
        phase_ += rate_ * dt;
        while (phase_ >= 2.0 * M_PI) {
            phase_ -= 2.0 * M_PI;
            cycle_count_++;
        }
        return phase_;
    }
    
    /**
     * @brief Get the objective weight based on current phase
     * 
     * Uses sinusoidal oscillation between objective and subjective
     */
    double objectiveWeight() const {
        return 0.5 * (1.0 + std::cos(phase_));
    }
    
    /**
     * @brief Get the subjective weight based on current phase
     */
    double subjectiveWeight() const {
        return 1.0 - objectiveWeight();
    }
    
    /**
     * @brief Check if Center 2 is looking outward (objective) or inward (subjective)
     */
    bool isLookingOutward() const {
        return objectiveWeight() > 0.5;
    }
    
    /// Reset the transposition
    void reset() {
        phase_ = 0.0;
        cycle_count_ = 0;
    }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "Perceptual transposition is the alternation of Center 2 between "
               "looking outward to sky (objective mode) and turning to face Center 1 "
               "(subjective mode). This maintains dynamic balance between expression "
               "and regeneration.";
    }

private:
    double rate_;
    double phase_;
    int cycle_count_;
};

/**
 * @brief Represents a "term" in System 2
 * 
 * A term is the relationship between two centers - the fundamental unit
 * of perceptive wholeness. In System 2, the term relates the universal
 * center to the particular center.
 */
class Term {
public:
    /**
     * @brief Construct a term from two centers
     * @param universal The universal center
     * @param particular The particular center
     */
    Term(const Center& universal, const Center& particular)
        : universal_(universal)
        , particular_(particular) {}
    
    /// Get the universal center
    const Center& universal() const { return universal_; }
    Center& universal() { return universal_; }
    
    /// Get the particular center
    const Center& particular() const { return particular_; }
    Center& particular() { return particular_; }
    
    /**
     * @brief Get the term value (relationship strength)
     * 
     * The term value represents the strength of the relationship
     * between universal and particular aspects.
     */
    double value() const {
        return universal_.intensity() * particular_.intensity();
    }
    
    /**
     * @brief Get the polarity between universal and particular
     * 
     * -1 = fully particular, +1 = fully universal
     */
    double polarity() const {
        double total = universal_.intensity() + particular_.intensity();
        if (total < 1e-10) return 0.0;
        return (universal_.intensity() - particular_.intensity()) / total;
    }
    
    /**
     * @brief Check if the term is balanced
     * 
     * A balanced term has equal universal and particular intensities
     */
    bool isBalanced() const {
        return std::abs(polarity()) < 0.01;
    }
    
    /// Get canonical representation
    std::string canonical() const {
        return "(" + universal_.canonical() + particular_.canonical() + ")";
    }

private:
    Center universal_;
    Center particular_;
};

/**
 * @brief Complete model of System 2: Perceptive Wholeness
 * 
 * System 2 elaborates upon System 1 with two related active interfaces.
 * It introduces the fundamental duality of universal/particular and
 * objective/subjective that underlies all subsequent systems.
 * 
 * From Fisherman's Guide:
 * "System 2 concerns the active interdependence between universal and
 * particular aspects. If there were no particular aspects, there could
 * be no universal aspects, nor vice versa."
 * 
 * Properties:
 * - Two centers define two modes related together as a "term"
 * - Fundamental interdependence of particular and universal aspects
 * - Perceptual transposition maintains dynamic balance
 * - Coalescence achieves mutual identity in subjective mode
 * - Introduces "threeness" requiring System 3 elaboration
 * 
 * The two terms of System 2 correspond to the two rooted trees with 3 nodes:
 * - Term 1: ((())) - Linear/serial (nested chaining) - U subsumes P
 * - Term 2: (()()) - Parallel/replicated (product structure) - U alongside P
 */
class System2 {
public:
    /**
     * @brief Construct System 2 with default parameters
     */
    System2()
        : universal_center_(Orientation::UNIVERSAL, 0.5)
        , particular_center_(Orientation::PARTICULAR, 0.5)
        , transposition_()
        , coalescence_()
        , current_mode_(Mode::OBJECTIVE)
        , time_(0.0) {}
    
    /**
     * @brief Construct System 2 with specified parameters
     * @param universal_intensity Intensity of universal center (L₀)
     * @param particular_intensity Intensity of particular center (L₁)
     * @param transposition_rate Rate of perceptual transposition
     */
    System2(double universal_intensity, double particular_intensity, 
            double transposition_rate = 0.1)
        : universal_center_(Orientation::UNIVERSAL, universal_intensity)
        , particular_center_(Orientation::PARTICULAR, particular_intensity)
        , transposition_(transposition_rate)
        , coalescence_()
        , current_mode_(Mode::OBJECTIVE)
        , time_(0.0) {
        normalize();
    }
    
    // Center accessors
    Center& universalCenter() { return universal_center_; }
    const Center& universalCenter() const { return universal_center_; }
    
    Center& particularCenter() { return particular_center_; }
    const Center& particularCenter() const { return particular_center_; }
    
    // Transposition accessor (renamed from transformation)
    PerceptualTransposition& transposition() { return transposition_; }
    const PerceptualTransposition& transposition() const { return transposition_; }
    
    // Legacy accessor for compatibility
    PerceptualTransposition& transformation() { return transposition_; }
    const PerceptualTransposition& transformation() const { return transposition_; }
    
    // Coalescence accessor
    Coalescence& coalescence() { return coalescence_; }
    const Coalescence& coalescence() const { return coalescence_; }
    
    /// Get the current dominant mode
    Mode currentMode() const { return current_mode_; }
    
    /**
     * @brief Get the primary term (universal-particular relationship)
     */
    Term primaryTerm() const {
        return Term(universal_center_, particular_center_);
    }
    
    /**
     * @brief Get both terms of System 2
     * 
     * System 2 has exactly 2 terms (A000081(3) = 2):
     * - Term 0: Serial/nested structure (universal subsumes particular)
     * - Term 1: Parallel/product structure (universal alongside particular)
     */
    std::array<Term, 2> terms() const {
        return {
            Term(universal_center_, particular_center_),  // Serial: U contains P
            Term(particular_center_, universal_center_)   // Parallel: P alongside U
        };
    }
    
    /**
     * @brief Get the polarity between universal and particular
     * 
     * -1 = fully particular, +1 = fully universal
     */
    double polarity() const {
        double total = universal_center_.intensity() + particular_center_.intensity();
        if (total < 1e-10) return 0.0;
        return (universal_center_.intensity() - particular_center_.intensity()) / total;
    }
    
    /**
     * @brief Get the mode polarity
     * 
     * -1 = fully subjective, +1 = fully objective
     */
    double modePolarity() const {
        return transposition_.objectiveWeight() - transposition_.subjectiveWeight();
    }
    
    /**
     * @brief Normalize the center intensities to sum to 1
     */
    void normalize() {
        double total = universal_center_.intensity() + particular_center_.intensity();
        if (total > 1e-10) {
            universal_center_.setIntensity(universal_center_.intensity() / total);
            particular_center_.setIntensity(particular_center_.intensity() / total);
        }
    }
    
    /**
     * @brief Advance the system by one time step
     * @param dt Time step duration
     * 
     * The perceptual transposition cycles between objective and subjective modes.
     * Coalescence is updated based on the current mode.
     */
    void step(double dt = 1.0) {
        // Advance the perceptual transposition
        transposition_.step(dt);
        
        // Update mode balance on both centers
        double obj_weight = transposition_.objectiveWeight();
        universal_center_.setModeBalance(obj_weight);
        particular_center_.setModeBalance(1.0 - obj_weight);  // Complementary
        
        // Update current dominant mode
        Mode old_mode = current_mode_;
        current_mode_ = obj_weight > 0.5 ? Mode::OBJECTIVE : Mode::SUBJECTIVE;
        
        // Update coalescence based on mode
        coalescence_.update(current_mode_, 
                          universal_center_.intensity(), 
                          particular_center_.intensity());
        
        // Update relational whole identity strengths
        if (current_mode_ == Mode::SUBJECTIVE) {
            // In subjective mode, relational wholes strengthen
            universal_center_.relationalWhole().setIdentityStrength(
                coalescence_.strength());
            particular_center_.relationalWhole().setIdentityStrength(
                coalescence_.strength());
        }
        
        time_ += dt;
    }
    
    /**
     * @brief Get the electromagnetic frequency analog
     * 
     * From Fisherman's Guide: "The wide variance of the electromagnetic spectrum,
     * from low frequencies with long wavelengths to high frequencies with short
     * wavelengths, is determined by the relative partitioning characteristics
     * of the two centers of System 2."
     */
    double electromagneticFrequency() const {
        // Frequency is proportional to particular center intensity
        // (higher particular intensity = faster transposition cycles)
        return particular_center_.intensity() * transposition_.rate();
    }
    
    /**
     * @brief Get the electromagnetic wavelength analog
     */
    double electromagneticWavelength() const {
        double freq = electromagneticFrequency();
        if (freq < 1e-10) return std::numeric_limits<double>::infinity();
        return 1.0 / freq;
    }
    
    /// Get current simulation time
    double time() const { return time_; }
    
    /// Reset the system to initial state
    void reset() {
        universal_center_.setIntensity(0.5);
        particular_center_.setIntensity(0.5);
        transposition_.reset();
        coalescence_ = Coalescence();
        current_mode_ = Mode::OBJECTIVE;
        time_ = 0.0;
    }
    
    /**
     * @brief Get the canonical tree representations for System 2 terms
     * 
     * System 2 has 2 terms corresponding to 2 rooted trees with 3 nodes:
     * - ((())) - Linear chain (serial/nested)
     * - (()()) - Binary split (parallel/product)
     */
    std::array<std::string, 2> canonicalTerms() const {
        return { "((()))", "(()())" };
    }
    
    /**
     * @brief Get the number of terms in System 2
     * 
     * System 2 has exactly 2 terms (A000081(3) = 2)
     */
    static constexpr int termCount() { return 2; }
    
    /**
     * @brief Get the number of clusters in System 2
     * 
     * System 2 has exactly 1 cluster (A000055(3) = 1)
     * Both terms belong to the same equivalence class under flip transform.
     */
    static constexpr int clusterCount() { return 1; }
    
    /**
     * @brief Get the node count (for OEIS alignment)
     * 
     * System 2 corresponds to n=3 nodes in the rooted tree representation
     */
    static constexpr int nodeCount() { return 3; }
    
    /// Get a description of System 2
    static std::string description() {
        return "System 2: Perceptive Wholeness - Two centers (Universal and Particular) "
               "defining two modes (Objective and Subjective) related together as a term. "
               "Perceptual transposition maintains dynamic balance between expression "
               "(outward to sky) and regeneration (mutual coalescence in light).";
    }
    
    /**
     * @brief Generate SVG diagram of System 2
     * 
     * Creates a visual representation based on Figures 12, 13, 19 from Fisherman's Guide,
     * showing the vesica piscis and the two modes.
     */
    std::string toSVG(int width = 800, int height = 400) const {
        std::ostringstream svg;
        svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width 
            << "\" height=\"" << height << "\">\n";
        svg << "  <defs>\n";
        svg << "    <radialGradient id=\"lightGrad1\" cx=\"30%\" cy=\"50%\" r=\"50%\">\n";
        svg << "      <stop offset=\"0%\" style=\"stop-color:#FFD700;stop-opacity:1\" />\n";
        svg << "      <stop offset=\"100%\" style=\"stop-color:#1a1a2e;stop-opacity:0.8\" />\n";
        svg << "    </radialGradient>\n";
        svg << "    <radialGradient id=\"lightGrad2\" cx=\"70%\" cy=\"50%\" r=\"50%\">\n";
        svg << "      <stop offset=\"0%\" style=\"stop-color:#FFA500;stop-opacity:1\" />\n";
        svg << "      <stop offset=\"100%\" style=\"stop-color:#1a1a2e;stop-opacity:0.8\" />\n";
        svg << "    </radialGradient>\n";
        svg << "  </defs>\n";
        
        // Background
        svg << "  <rect width=\"100%\" height=\"100%\" fill=\"#1a1a2e\"/>\n";
        
        // Title
        svg << "  <text x=\"" << width/2 << "\" y=\"25\" text-anchor=\"middle\" "
            << "fill=\"white\" font-size=\"14\" font-weight=\"bold\">System 2: Perceptive Wholeness</text>\n";
        
        // Vesica Piscis (left side)
        int vx = width / 4;
        int vy = height / 2;
        int r = 70;
        int offset = 40;
        
        svg << "  <circle cx=\"" << (vx - offset/2) << "\" cy=\"" << vy << "\" r=\"" << r 
            << "\" fill=\"url(#lightGrad1)\" fill-opacity=\"0.7\" stroke=\"#FFD700\" stroke-width=\"2\"/>\n";
        svg << "  <circle cx=\"" << (vx + offset/2) << "\" cy=\"" << vy << "\" r=\"" << r 
            << "\" fill=\"url(#lightGrad2)\" fill-opacity=\"0.7\" stroke=\"#FFA500\" stroke-width=\"2\"/>\n";
        
        // Center labels
        svg << "  <text x=\"" << (vx - offset/2 - 20) << "\" y=\"" << vy << "\" "
            << "fill=\"white\" font-size=\"14\" font-weight=\"bold\">1</text>\n";
        svg << "  <text x=\"" << (vx + offset/2 + 15) << "\" y=\"" << vy << "\" "
            << "fill=\"white\" font-size=\"14\" font-weight=\"bold\">2</text>\n";
        
        svg << "  <text x=\"" << vx << "\" y=\"" << (vy + r + 25) << "\" text-anchor=\"middle\" "
            << "fill=\"#888\" font-size=\"11\">Vesica Piscis</text>\n";
        svg << "  <text x=\"" << vx << "\" y=\"" << (vy + r + 40) << "\" text-anchor=\"middle\" "
            << "fill=\"#666\" font-size=\"10\">(From Darkness)</text>\n";
        
        // Active mode diagram (right side)
        int ax = 3 * width / 4;
        int ay = height / 2;
        
        if (current_mode_ == Mode::OBJECTIVE) {
            // Objective mode: sequential, arrows pointing outward
            svg << "  <ellipse cx=\"" << (ax - 50) << "\" cy=\"" << ay << "\" rx=\"40\" ry=\"60\" "
                << "fill=\"url(#lightGrad1)\" stroke=\"#FFD700\" stroke-width=\"2\"/>\n";
            svg << "  <ellipse cx=\"" << (ax + 50) << "\" cy=\"" << ay << "\" rx=\"40\" ry=\"60\" "
                << "fill=\"url(#lightGrad2)\" stroke=\"#FFA500\" stroke-width=\"2\"/>\n";
            // Arrow showing sequence
            svg << "  <path d=\"M " << (ax - 10) << " " << (ay - 30) << " L " << (ax + 10) << " " << (ay - 30) 
                << "\" stroke=\"white\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n";
            svg << "  <text x=\"" << ax << "\" y=\"" << (ay + 80) << "\" text-anchor=\"middle\" "
                << "fill=\"#FFD700\" font-size=\"12\">OBJECTIVE MODE</text>\n";
            svg << "  <text x=\"" << ax << "\" y=\"" << (ay + 95) << "\" text-anchor=\"middle\" "
                << "fill=\"#888\" font-size=\"10\">(Sequential, Expressive)</text>\n";
        } else {
            // Subjective mode: simultaneous, Z arrow for coalescence
            svg << "  <ellipse cx=\"" << (ax - 40) << "\" cy=\"" << ay << "\" rx=\"35\" ry=\"55\" "
                << "fill=\"url(#lightGrad1)\" stroke=\"#FFD700\" stroke-width=\"2\"/>\n";
            svg << "  <ellipse cx=\"" << (ax + 40) << "\" cy=\"" << ay << "\" rx=\"35\" ry=\"55\" "
                << "fill=\"url(#lightGrad2)\" stroke=\"#FFA500\" stroke-width=\"2\"/>\n";
            // Z arrow for coalescence
            svg << "  <path d=\"M " << (ax - 20) << " " << (ay - 40) << " L " << (ax + 20) << " " << (ay - 40) 
                << " L " << (ax - 20) << " " << (ay + 40) << " L " << (ax + 20) << " " << (ay + 40) 
                << "\" stroke=\"#00FF00\" stroke-width=\"3\" fill=\"none\"/>\n";
            svg << "  <text x=\"" << ax << "\" y=\"" << (ay + 80) << "\" text-anchor=\"middle\" "
                << "fill=\"#00FF00\" font-size=\"12\">SUBJECTIVE MODE</text>\n";
            svg << "  <text x=\"" << ax << "\" y=\"" << (ay + 95) << "\" text-anchor=\"middle\" "
                << "fill=\"#888\" font-size=\"10\">(Simultaneous, Regenerative)</text>\n";
        }
        
        // Labels
        svg << "  <text x=\"" << (ax - 50) << "\" y=\"" << (ay - 70) << "\" text-anchor=\"middle\" "
            << "fill=\"#FFD700\" font-size=\"10\">L₀</text>\n";
        svg << "  <text x=\"" << (ax + 50) << "\" y=\"" << (ay - 70) << "\" text-anchor=\"middle\" "
            << "fill=\"#FFA500\" font-size=\"10\">L₁</text>\n";
        
        // Relational whole labels
        svg << "  <text x=\"" << (ax - 80) << "\" y=\"" << (ay - 50) << "\" "
            << "fill=\"#888\" font-size=\"9\">R₁</text>\n";
        svg << "  <text x=\"" << (ax + 75) << "\" y=\"" << (ay - 50) << "\" "
            << "fill=\"#888\" font-size=\"9\">R₂</text>\n";
        
        svg << "</svg>\n";
        return svg.str();
    }

private:
    Center universal_center_;
    Center particular_center_;
    PerceptualTransposition transposition_;
    Coalescence coalescence_;
    Mode current_mode_;
    double time_;
};

/**
 * @brief Observer for System 2 dynamics
 * 
 * Allows monitoring of mode transitions and polarity changes.
 */
class System2Observer {
public:
    using Callback = std::function<void(const System2&, double)>;
    using ModeChangeCallback = std::function<void(const System2&, Mode, Mode)>;
    
    void onStep(Callback callback) {
        step_callbacks_.push_back(callback);
    }
    
    void onModeChange(ModeChangeCallback callback) {
        mode_callbacks_.push_back(callback);
    }
    
    void notifyStep(const System2& system, double dt) {
        for (auto& cb : step_callbacks_) {
            cb(system, dt);
        }
    }
    
    void notifyModeChange(const System2& system, Mode old_mode, Mode new_mode) {
        for (auto& cb : mode_callbacks_) {
            cb(system, old_mode, new_mode);
        }
    }

private:
    std::vector<Callback> step_callbacks_;
    std::vector<ModeChangeCallback> mode_callbacks_;
};

/**
 * @brief The Flashlight Analogy from Fisherman's Guide
 * 
 * From the text: "Imagine yourself to be hovering in a flying saucer at nighttime
 * a few miles above the earth, which is covered with people standing shoulder to
 * shoulder, each flicking a large flashlight on and off up into the sky."
 * 
 * This demonstrates how System 2's alternating modes are subsumed within System 1.
 */
class FlashlightAnalogy {
public:
    struct Flashlight {
        double brightness;      ///< Current brightness (0 = off, 1 = max)
        double battery_level;   ///< Battery charge (0 to 1)
        double recharge_rate;   ///< How fast battery recharges when off
        bool is_on;
        
        Flashlight(double brightness = 0.5, double recharge = 0.1)
            : brightness(brightness), battery_level(1.0), recharge_rate(recharge), is_on(true) {}
    };
    
    FlashlightAnalogy(int count = 100) {
        flashlights_.resize(count);
        // Randomize initial states
        for (auto& f : flashlights_) {
            f.brightness = 0.1 + 0.9 * (rand() % 100) / 100.0;
            f.recharge_rate = 0.05 + 0.1 * (rand() % 100) / 100.0;
        }
    }
    
    /**
     * @brief Simulate one time step
     * 
     * Each flashlight alternates between on (expressive) and off (regenerative)
     */
    void step(double dt = 1.0) {
        for (auto& f : flashlights_) {
            if (f.is_on) {
                // Expressive mode: drain battery
                f.battery_level -= f.brightness * dt * 0.1;
                if (f.battery_level <= 0.0) {
                    f.is_on = false;
                    f.battery_level = 0.0;
                }
            } else {
                // Regenerative mode: recharge battery
                f.battery_level += f.recharge_rate * dt;
                if (f.battery_level >= 1.0) {
                    f.is_on = true;
                    f.battery_level = 1.0;
                }
            }
        }
    }
    
    /**
     * @brief Get the total light output (as seen from space)
     * 
     * From the text: "From several miles up, individual lights will not stand out;
     * the whole earth will appear as a universal active interface of light."
     */
    double totalLightOutput() const {
        double total = 0.0;
        for (const auto& f : flashlights_) {
            if (f.is_on) {
                total += f.brightness * f.battery_level;
            }
        }
        return total / flashlights_.size();
    }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "The flashlight analogy demonstrates how System 2's alternating modes "
               "are subsumed within System 1. Each flashlight flicks on (expressive) "
               "and off (regenerative), but from space the whole earth appears as a "
               "universal active interface of light - a constant unbroken glow.";
    }

private:
    std::vector<Flashlight> flashlights_;
};

/**
 * @brief Utility functions for System 2 analysis
 */
namespace utils {

/**
 * @brief Calculate the "threeness" measure
 * 
 * System 2 introduces threeness through:
 * 1. Objective mode
 * 2. Subjective mode
 * 3. Transformation between them
 * 
 * This function returns a measure of how active the transformation is.
 */
inline double threeness(const System2& system) {
    double obj = system.transposition().objectiveWeight();
    return 4.0 * obj * (1.0 - obj);  // Peaks at 1.0 when obj = 0.5
}

/**
 * @brief Check if the system exhibits discontinuity
 * 
 * The fundamental discontinuity in space-time projection occurs
 * at mode transition points.
 */
inline bool isDiscontinuity(const System2& system, double threshold = 0.01) {
    double obj = system.transposition().objectiveWeight();
    return std::abs(obj - 0.5) < threshold;
}

/**
 * @brief Get the mode as a string
 */
inline std::string modeToString(Mode mode) {
    return mode == Mode::OBJECTIVE ? "Objective" : "Subjective";
}

/**
 * @brief Get the orientation as a string
 */
inline std::string orientationToString(Orientation orientation) {
    return orientation == Orientation::UNIVERSAL ? "Universal" : "Particular";
}

/**
 * @brief Check if coalescence is active
 */
inline bool isCoalesced(const System2& system) {
    return system.coalescence().isActive() && system.coalescence().strength() > 0.5;
}

} // namespace utils

} // namespace system2
} // namespace cosmic
