/**
 * @file system1.hpp
 * @brief Detailed C++ model of System 1: Universal Wholeness
 * 
 * System 1 represents universal wholeness - the relationship of all things to
 * indeterminate sky. It transcends the whole of creation, history, space and time.
 * 
 * Based on Robert Campbell's "Physics & Cosmic Order II: Introducing the System"
 * 
 * Key concepts:
 * - Universal Inside (active) relating to Universal Outside (passive)
 * - Active Interface between inside and outside
 * - Efflux (outward flow) and Reflux (inward return)
 * - All phenomena share both a universal inside and universal outside
 * 
 * Mathematical alignment: System 1 has 1 term (A000081(2) = 1)
 */

#pragma once

#include <cmath>
#include <limits>
#include <string>
#include <functional>
#include <vector>

namespace cosmic {
namespace system1 {

/**
 * @brief Represents the universal center (inside) of System 1
 * 
 * The center is the common inside shared by all phenomena.
 * It is active, unbounded, and represents the source of efflux.
 */
class UniversalCenter {
public:
    /**
     * @brief Construct a universal center with given intensity
     * @param intensity The active intensity at the center (default: 1.0)
     */
    explicit UniversalCenter(double intensity = 1.0) 
        : intensity_(intensity) {}
    
    /// Get the active intensity at the center
    double intensity() const { return intensity_; }
    
    /// Set the active intensity
    void setIntensity(double value) { intensity_ = value; }
    
    /// The center is always bounded (finite point)
    bool isBounded() const { return true; }
    
    /// Get the canonical representation
    std::string canonical() const { return "C"; }

private:
    double intensity_;
};

/**
 * @brief Represents the universal periphery (outside) of System 1
 * 
 * The periphery is the common outside to which all things relate.
 * It is passive, unbounded (infinite), and represents the destination of efflux
 * and source of reflux.
 */
class UniversalPeriphery {
public:
    /**
     * @brief Construct a universal periphery
     * @param bounded If true, represents a finite boundary; if false, unbounded (default)
     */
    explicit UniversalPeriphery(bool bounded = false)
        : bounded_(bounded) {}
    
    /// Check if the periphery is bounded
    bool isBounded() const { return bounded_; }
    
    /// Get the extent (infinity if unbounded)
    double extent() const {
        return bounded_ ? finite_extent_ : std::numeric_limits<double>::infinity();
    }
    
    /// Set finite extent (makes periphery bounded)
    void setExtent(double value) {
        bounded_ = true;
        finite_extent_ = value;
    }
    
    /// Make periphery unbounded
    void makeUnbounded() {
        bounded_ = false;
    }
    
    /// Get the canonical representation
    std::string canonical() const { return bounded_ ? "P" : "∞"; }

private:
    bool bounded_;
    double finite_extent_ = 1.0;
};

/**
 * @brief Represents the active interface between inside and outside
 * 
 * The active interface is the boundary across which all phenomenal experience
 * occurs. It cannot be static - there must be interaction between inside and
 * outside. This is what we perceive as the "surfaces of things."
 */
class ActiveInterface {
public:
    /**
     * @brief Construct an active interface
     * @param efflux_rate Rate of outward flow from center to periphery
     * @param reflux_rate Rate of inward return from periphery to center
     */
    ActiveInterface(double efflux_rate = 1.0, double reflux_rate = 1.0)
        : efflux_rate_(efflux_rate)
        , reflux_rate_(reflux_rate)
        , accumulated_efflux_(0.0)
        , accumulated_reflux_(0.0) {}
    
    /// Get efflux rate (outward flow)
    double effluxRate() const { return efflux_rate_; }
    
    /// Get reflux rate (inward return)
    double refluxRate() const { return reflux_rate_; }
    
    /// Set efflux rate
    void setEffluxRate(double rate) { efflux_rate_ = rate; }
    
    /// Set reflux rate
    void setRefluxRate(double rate) { reflux_rate_ = rate; }
    
    /// Get net flow (positive = outward, negative = inward)
    double netFlow() const { return efflux_rate_ - reflux_rate_; }
    
    /// Check if interface is in equilibrium
    bool isEquilibrium() const { 
        return std::abs(netFlow()) < 1e-10; 
    }
    
    /**
     * @brief Advance the interface by one time step
     * @param dt Time step duration
     * @return The net energy transferred
     */
    double step(double dt = 1.0) {
        double efflux = efflux_rate_ * dt;
        double reflux = reflux_rate_ * dt;
        accumulated_efflux_ += efflux;
        accumulated_reflux_ += reflux;
        return efflux - reflux;
    }
    
    /// Get total accumulated efflux
    double accumulatedEfflux() const { return accumulated_efflux_; }
    
    /// Get total accumulated reflux
    double accumulatedReflux() const { return accumulated_reflux_; }
    
    /// Reset accumulated values
    void reset() {
        accumulated_efflux_ = 0.0;
        accumulated_reflux_ = 0.0;
    }
    
    /// Get the canonical representation
    std::string canonical() const { return "I"; }

private:
    double efflux_rate_;
    double reflux_rate_;
    double accumulated_efflux_;
    double accumulated_reflux_;
};

/**
 * @brief Perspective from which System 1 can be viewed
 */
enum class Perspective {
    PASSIVE,  ///< From outside active interfaces looking in
    ACTIVE    ///< From inside active interfaces looking out
};

/**
 * @brief Complete model of System 1: Universal Wholeness
 * 
 * System 1 is the ground state representing the relationship of all things
 * to indeterminate sky. It specifies universal boundary conditions with an
 * active universal inside relating to a passive universal outside across
 * an active interface between them.
 * 
 * Properties:
 * - Transcends all of creation, history, space and time
 * - Cannot manifest as a physical thing itself
 * - All phenomena share both universal inside and universal outside
 * - We can only know the active interface between them
 */
class System1 {
public:
    /**
     * @brief Construct System 1 with default parameters
     */
    System1()
        : center_()
        , periphery_()
        , interface_()
        , perspective_(Perspective::ACTIVE)
        , time_(0.0) {}
    
    /**
     * @brief Construct System 1 with specified parameters
     * @param center_intensity Initial intensity at the center
     * @param efflux_rate Rate of outward flow
     * @param reflux_rate Rate of inward return
     */
    System1(double center_intensity, double efflux_rate, double reflux_rate)
        : center_(center_intensity)
        , periphery_()
        , interface_(efflux_rate, reflux_rate)
        , perspective_(Perspective::ACTIVE)
        , time_(0.0) {}
    
    // Component accessors
    UniversalCenter& center() { return center_; }
    const UniversalCenter& center() const { return center_; }
    
    UniversalPeriphery& periphery() { return periphery_; }
    const UniversalPeriphery& periphery() const { return periphery_; }
    
    ActiveInterface& interface() { return interface_; }
    const ActiveInterface& interface() const { return interface_; }
    
    // Perspective
    Perspective perspective() const { return perspective_; }
    void setPerspective(Perspective p) { perspective_ = p; }
    
    /**
     * @brief Get the interface ratio (center to periphery)
     * 
     * For unbounded periphery, this approaches 0.
     * This represents the relationship between inside and outside.
     */
    double interfaceRatio() const {
        if (!periphery_.isBounded()) return 0.0;
        return center_.intensity() / periphery_.extent();
    }
    
    /**
     * @brief Get the total energy in the system
     * 
     * Energy is conserved across the active interface.
     * Total energy = center intensity (normalized to 1.0 for universal wholeness)
     */
    double energy() const { return center_.intensity(); }
    
    /**
     * @brief Advance the system by one time step
     * @param dt Time step duration
     * 
     * The active interface processes efflux and reflux.
     * In equilibrium, the center intensity remains constant.
     */
    void step(double dt = 1.0) {
        double net_transfer = interface_.step(dt);
        
        // In System 1, the center intensity is adjusted by net flow
        // but total energy is conserved (efflux returns as reflux)
        if (!interface_.isEquilibrium()) {
            // Non-equilibrium: center loses/gains based on net flow
            center_.setIntensity(center_.intensity() - net_transfer * 0.01);
        }
        
        time_ += dt;
    }
    
    /// Get current simulation time
    double time() const { return time_; }
    
    /// Reset the system to initial state
    void reset() {
        center_.setIntensity(1.0);
        interface_.reset();
        time_ = 0.0;
    }
    
    /**
     * @brief Get the canonical tree representation
     * 
     * System 1 has 1 term, represented as a single rooted tree with 2 nodes:
     * root (universal context) + 1 child (the single term)
     */
    std::string canonical() const { return "(())"; }
    
    /**
     * @brief Get the number of terms in System 1
     * 
     * System 1 has exactly 1 term (A000081(2) = 1)
     */
    static constexpr int termCount() { return 1; }
    
    /**
     * @brief Get the number of clusters in System 1
     * 
     * System 1 has exactly 1 cluster (A000055(2) = 1)
     */
    static constexpr int clusterCount() { return 1; }
    
    /**
     * @brief Get the node count (for OEIS alignment)
     * 
     * System 1 corresponds to n=2 nodes in the rooted tree representation
     */
    static constexpr int nodeCount() { return 2; }
    
    /// Get a description of System 1
    static std::string description() {
        return "System 1: Universal Wholeness - The relationship of all things "
               "to indeterminate sky. An active universal inside relating to a "
               "passive universal outside across an active interface.";
    }

private:
    UniversalCenter center_;
    UniversalPeriphery periphery_;
    ActiveInterface interface_;
    Perspective perspective_;
    double time_;
};

/**
 * @brief Observer for System 1 dynamics
 * 
 * Allows monitoring of efflux/reflux cycles and interface activity.
 */
class System1Observer {
public:
    using Callback = std::function<void(const System1&, double)>;
    
    /**
     * @brief Register a callback for each simulation step
     * @param callback Function called with system state and time
     */
    void onStep(Callback callback) {
        callbacks_.push_back(callback);
    }
    
    /**
     * @brief Notify all observers of a step
     * @param system The System 1 instance
     * @param dt Time step
     */
    void notify(const System1& system, double dt) {
        for (auto& cb : callbacks_) {
            cb(system, dt);
        }
    }

private:
    std::vector<Callback> callbacks_;
};

} // namespace system1
} // namespace cosmic
