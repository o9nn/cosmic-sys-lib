/**
 * @file system2.hpp
 * @brief Detailed C++ model of System 2: Perceptive Wholeness
 * 
 * System 2 elaborates upon System 1 with two related active interfaces to
 * account for multiplicity. One interface is universal and unique, while
 * the other is particular and many.
 * 
 * Based on Robert Campbell's "Physics & Cosmic Order II: Introducing the System"
 * 
 * Key concepts:
 * - Two centers: Universal (unique) and Particular (many)
 * - Two modes: Objective (outside looking in) and Subjective (inside looking out)
 * - Active transformation between objective and subjective orientations
 * - The "term" as the relationship between two centers
 * - Fundamental discontinuity in projection of space-time phenomena
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

namespace cosmic {
namespace system2 {

/**
 * @brief The two modes of perception in System 2
 */
enum class Mode {
    OBJECTIVE,   ///< Perception from outside looking in (passive)
    SUBJECTIVE   ///< Perception from inside looking out (active)
};

/**
 * @brief The two orientations in System 2
 */
enum class Orientation {
    UNIVERSAL,   ///< Universal aspect - unique, one
    PARTICULAR   ///< Particular aspect - many, diverse
};

/**
 * @brief Represents a center in System 2
 * 
 * System 2 has two centers: one universal (unique) and one particular (many).
 * Each center has both objective and subjective aspects.
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
        , subjective_weight_(0.5) {}
    
    /// Get the orientation (universal or particular)
    Orientation orientation() const { return orientation_; }
    
    /// Get the intensity
    double intensity() const { return intensity_; }
    
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
    
    /// Get canonical representation
    std::string canonical() const {
        return orientation_ == Orientation::UNIVERSAL ? "U" : "P";
    }

private:
    Orientation orientation_;
    double intensity_;
    double objective_weight_;
    double subjective_weight_;
};

/**
 * @brief Represents the transformation between modes
 * 
 * The active transformation between objective and subjective orientations
 * introduces "threeness" - the two modes plus the transformation itself.
 * This threeness requires System 3 to elaborate further.
 */
class ModeTransformation {
public:
    /**
     * @brief Construct a mode transformation
     * @param rate Rate of transformation between modes
     */
    explicit ModeTransformation(double rate = 0.1)
        : rate_(rate)
        , phase_(0.0)
        , cycle_count_(0) {}
    
    /// Get the transformation rate
    double rate() const { return rate_; }
    
    /// Set the transformation rate
    void setRate(double rate) { rate_ = rate; }
    
    /// Get the current phase (0 to 2π)
    double phase() const { return phase_; }
    
    /// Get the number of complete cycles
    int cycleCount() const { return cycle_count_; }
    
    /**
     * @brief Advance the transformation by one step
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
    
    /// Reset the transformation
    void reset() {
        phase_ = 0.0;
        cycle_count_ = 0;
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
 * Properties:
 * - Two centers define two modes related together as a "term"
 * - Fundamental interdependence of particular and universal aspects
 * - Alternating objective and subjective orientations
 * - Active transformation between modes introduces "threeness"
 * 
 * The two terms of System 2 correspond to the two rooted trees with 3 nodes:
 * - Term 1: ((())) - Linear/serial (nested chaining)
 * - Term 2: (()()) - Parallel/replicated (product structure)
 */
class System2 {
public:
    /**
     * @brief Construct System 2 with default parameters
     */
    System2()
        : universal_center_(Orientation::UNIVERSAL, 0.5)
        , particular_center_(Orientation::PARTICULAR, 0.5)
        , transformation_()
        , current_mode_(Mode::OBJECTIVE)
        , time_(0.0) {}
    
    /**
     * @brief Construct System 2 with specified parameters
     * @param universal_intensity Intensity of universal center
     * @param particular_intensity Intensity of particular center
     * @param transformation_rate Rate of mode transformation
     */
    System2(double universal_intensity, double particular_intensity, 
            double transformation_rate = 0.1)
        : universal_center_(Orientation::UNIVERSAL, universal_intensity)
        , particular_center_(Orientation::PARTICULAR, particular_intensity)
        , transformation_(transformation_rate)
        , current_mode_(Mode::OBJECTIVE)
        , time_(0.0) {
        normalize();
    }
    
    // Center accessors
    Center& universalCenter() { return universal_center_; }
    const Center& universalCenter() const { return universal_center_; }
    
    Center& particularCenter() { return particular_center_; }
    const Center& particularCenter() const { return particular_center_; }
    
    // Transformation accessor
    ModeTransformation& transformation() { return transformation_; }
    const ModeTransformation& transformation() const { return transformation_; }
    
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
        // Both terms use the same centers but represent different relationships
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
        return transformation_.objectiveWeight() - transformation_.subjectiveWeight();
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
     * The transformation cycles between objective and subjective modes.
     * The center intensities oscillate based on the mode.
     */
    void step(double dt = 1.0) {
        // Advance the transformation
        transformation_.step(dt);
        
        // Update mode balance on both centers
        double obj_weight = transformation_.objectiveWeight();
        universal_center_.setModeBalance(obj_weight);
        particular_center_.setModeBalance(1.0 - obj_weight);  // Complementary
        
        // Update current dominant mode
        current_mode_ = obj_weight > 0.5 ? Mode::OBJECTIVE : Mode::SUBJECTIVE;
        
        time_ += dt;
    }
    
    /**
     * @brief Transition between modes
     * @param rate Rate of transition (0 to 1)
     * 
     * Smoothly transitions the system between objective and subjective modes.
     */
    void transition(double rate = 0.1) {
        double u_intensity = universal_center_.intensity();
        double p_intensity = particular_center_.intensity();
        
        // Cross-influence: each center moves toward the other
        universal_center_.setIntensity(u_intensity + rate * (p_intensity - u_intensity));
        particular_center_.setIntensity(p_intensity + rate * (u_intensity - p_intensity));
    }
    
    /// Get current simulation time
    double time() const { return time_; }
    
    /// Reset the system to initial state
    void reset() {
        universal_center_.setIntensity(0.5);
        particular_center_.setIntensity(0.5);
        transformation_.reset();
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
        return "System 2: Perceptive Wholeness - Two centers defining two modes "
               "(objective and subjective) related together as a term. Introduces "
               "the fundamental duality of universal/particular with active "
               "transformation between orientations.";
    }

private:
    Center universal_center_;
    Center particular_center_;
    ModeTransformation transformation_;
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
    
    /**
     * @brief Register a callback for each simulation step
     */
    void onStep(Callback callback) {
        step_callbacks_.push_back(callback);
    }
    
    /**
     * @brief Register a callback for mode changes
     */
    void onModeChange(ModeChangeCallback callback) {
        mode_callbacks_.push_back(callback);
    }
    
    /**
     * @brief Notify observers of a step
     */
    void notifyStep(const System2& system, double dt) {
        for (auto& cb : step_callbacks_) {
            cb(system, dt);
        }
    }
    
    /**
     * @brief Notify observers of a mode change
     */
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
    // Threeness is maximized when transformation is active (mid-phase)
    // and minimized at pure objective or pure subjective states
    double obj = system.transformation().objectiveWeight();
    return 4.0 * obj * (1.0 - obj);  // Peaks at 1.0 when obj = 0.5
}

/**
 * @brief Check if the system exhibits discontinuity
 * 
 * The fundamental discontinuity in space-time projection occurs
 * at mode transition points.
 */
inline bool isDiscontinuity(const System2& system, double threshold = 0.01) {
    double obj = system.transformation().objectiveWeight();
    // Discontinuity near the transition point (obj ≈ 0.5)
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

} // namespace utils

} // namespace system2
} // namespace cosmic
