/**
 * @file system1.hpp
 * @brief Detailed C++ model of System 1: Universal Wholeness
 * 
 * System 1 represents universal wholeness - the relationship of all things to
 * indeterminate sky. It transcends the whole of creation, history, space and time.
 * 
 * Based on Robert Campbell's "Fisherman's Guide to the Cosmic Order" and
 * "Physics & Cosmic Order II: Introducing the System"
 * 
 * Key concepts:
 * - Universal Inside (active, Light) relating to Universal Outside (passive, Darkness)
 * - Active Interface between inside and outside (the surfaces of things)
 * - Efflux (outward flow) and Reflux (inward return)
 * - All phenomena share both a universal inside and universal outside
 * - Everything communicates with sky - everything finds common unity in sky
 * 
 * Diagram Elements (Figure 11 from Fisherman's Guide):
 * - L (Light): Active center, yang principle
 * - D (Darkness): Passive periphery, yin principle
 * - Active Interface: Boundary relating center to periphery
 * - Passive Perspective: Sphere view (outside looking in)
 * - Active Perspective: Cone/funnel view (inside looking out)
 * 
 * Mathematical alignment: System 1 has 1 term (A000081(2) = 1)
 */

#pragma once

#include <cmath>
#include <limits>
#include <string>
#include <functional>
#include <vector>
#include <sstream>

namespace cosmic {
namespace system1 {

/**
 * @brief Light intensity levels in System 1
 * 
 * From Fisherman's Guide: "System 1 identifies the subjective active intensity
 * as energy represented by light."
 */
struct LightLevel {
    static constexpr double L0 = 1.0;   ///< Universal light (maximum intensity)
    static constexpr double D = 0.0;    ///< Darkness (zero intensity)
};

/**
 * @brief Represents the universal center (inside) of System 1
 * 
 * The center is the common inside shared by all phenomena.
 * It is active, unbounded, and represents the source of efflux.
 * 
 * From Fisherman's Guide: "Being is the relationship of a specific to a
 * nonspecific aspect. The specific aspect is evidenced as an active center
 * to experience."
 */
class UniversalCenter {
public:
    /**
     * @brief Construct a universal center with given intensity
     * @param intensity The active intensity at the center (default: L0 = 1.0)
     */
    explicit UniversalCenter(double intensity = LightLevel::L0) 
        : intensity_(intensity) {}
    
    /// Get the active intensity at the center (Light level)
    double intensity() const { return intensity_; }
    
    /// Get intensity as Light symbol
    std::string lightSymbol() const { return "L₀"; }
    
    /// Set the active intensity
    void setIntensity(double value) { intensity_ = value; }
    
    /// The center is always bounded (finite point)
    bool isBounded() const { return true; }
    
    /// Get the canonical representation
    std::string canonical() const { return "L"; }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "The universal center is the common inside shared by all phenomena. "
               "It represents Light (L₀) - the active, yang principle. "
               "All evidences of energy and substance find unity in a common "
               "relationship to unbounded sky.";
    }

private:
    double intensity_;
};

/**
 * @brief Represents the universal periphery (outside) of System 1
 * 
 * The periphery is the common outside to which all things relate.
 * It is passive, unbounded (infinite), and represents darkness.
 * 
 * From Fisherman's Guide: "The nonspecific aspect, as a passive periphery...
 * The receptive aspect to phenomena is implicitly nonspecific; yet it is something."
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
    
    /// Get intensity (always darkness at periphery)
    double intensity() const { return LightLevel::D; }
    
    /// Get intensity as Darkness symbol
    std::string darknessSymbol() const { return "D"; }
    
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
    std::string canonical() const { return "D"; }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "The universal periphery is the common outside to which all things relate. "
               "It represents Darkness (D) - the passive, yin principle. "
               "The inverse of zero is infinity - the periphery extends indeterminately.";
    }

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
 * 
 * From Fisherman's Guide: "Being knows of the relationship between its center
 * and its periphery in terms of the active process occurring across the
 * structural interface between them. This amounts to a technical way of saying
 * that everything communicates with sky."
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
    
    /// Get efflux rate (outward flow to sky)
    double effluxRate() const { return efflux_rate_; }
    
    /// Get reflux rate (inward return from sky)
    double refluxRate() const { return reflux_rate_; }
    
    /// Set efflux rate
    void setEffluxRate(double rate) { efflux_rate_ = rate; }
    
    /// Set reflux rate
    void setRefluxRate(double rate) { reflux_rate_ = rate; }
    
    /// Get net flow (positive = outward, negative = inward)
    double netFlow() const { return efflux_rate_ - reflux_rate_; }
    
    /// Check if interface is in equilibrium (balanced efflux/reflux)
    bool isEquilibrium() const { 
        return std::abs(netFlow()) < 1e-10; 
    }
    
    /**
     * @brief Get the communicative balance
     * 
     * From Fisherman's Guide: The loon "absorbs subtle energies from the sky,
     * then projects them back enhanced" - this is the communicative balance.
     */
    double communicativeBalance() const {
        if (efflux_rate_ + reflux_rate_ < 1e-10) return 0.0;
        return (efflux_rate_ - reflux_rate_) / (efflux_rate_ + reflux_rate_);
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
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "The active interface relates the center to the periphery. "
               "It does not isolate them - it connects them through efflux (outward) "
               "and reflux (inward) energy processes. Everything communicates with sky "
               "through this interface.";
    }

private:
    double efflux_rate_;
    double reflux_rate_;
    double accumulated_efflux_;
    double accumulated_reflux_;
};

/**
 * @brief Perspective from which System 1 can be viewed
 * 
 * From Fisherman's Guide Figure 11:
 * - PASSIVE: From darkness, looking into the center toward light (sphere view)
 * - ACTIVE: From light, looking out toward the periphery (cone/funnel view)
 */
enum class Perspective {
    PASSIVE,  ///< From outside (darkness) looking in toward light - sphere view
    ACTIVE    ///< From inside (light) looking out toward darkness - cone/funnel view
};

/**
 * @brief Get string representation of perspective
 */
inline std::string toString(Perspective p) {
    switch (p) {
        case Perspective::PASSIVE: return "Passive (from Darkness)";
        case Perspective::ACTIVE: return "Active (from Light)";
        default: return "Unknown";
    }
}

/**
 * @brief Complete model of System 1: Universal Wholeness
 * 
 * System 1 is the ground state representing the relationship of all things
 * to indeterminate sky. It specifies universal boundary conditions with an
 * active universal inside relating to a passive universal outside across
 * an active interface between them.
 * 
 * From Fisherman's Guide:
 * "System 1 subsumes all of the higher systems. All evidences of energy and
 * substance find unity in a common relationship to unbounded sky. Everything
 * from light, to atomic particles, to molecules, to suns, planets, galaxies,
 * and to all forms of energy and life in all the galaxies in all the universe,
 * is unified in sky."
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
     * @param center_intensity Initial intensity at the center (Light level)
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
     * @brief Get the Light-to-Darkness gradient
     * 
     * This represents the fundamental polarity of System 1.
     */
    double lightDarknessGradient() const {
        return center_.intensity() - periphery_.intensity();
    }
    
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
        center_.setIntensity(LightLevel::L0);
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
               "to indeterminate sky. An active universal inside (Light) relating to a "
               "passive universal outside (Darkness) across an active interface. "
               "All phenomena share both universal inside and universal outside.";
    }
    
    /**
     * @brief Generate SVG diagram of System 1
     * 
     * Creates a visual representation based on Figure 11 from Fisherman's Guide,
     * showing both passive (sphere) and active (cone) perspectives.
     */
    std::string toSVG(int width = 600, int height = 300) const {
        std::ostringstream svg;
        svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width 
            << "\" height=\"" << height << "\">\n";
        svg << "  <defs>\n";
        svg << "    <radialGradient id=\"lightGrad\" cx=\"50%\" cy=\"50%\" r=\"50%\">\n";
        svg << "      <stop offset=\"0%\" style=\"stop-color:#FFD700;stop-opacity:1\" />\n";
        svg << "      <stop offset=\"100%\" style=\"stop-color:#1a1a2e;stop-opacity:1\" />\n";
        svg << "    </radialGradient>\n";
        svg << "  </defs>\n";
        
        // Background
        svg << "  <rect width=\"100%\" height=\"100%\" fill=\"#1a1a2e\"/>\n";
        
        // Title
        svg << "  <text x=\"" << width/2 << "\" y=\"25\" text-anchor=\"middle\" "
            << "fill=\"white\" font-size=\"14\" font-weight=\"bold\">System 1: Universal Wholeness</text>\n";
        
        // Passive perspective (sphere) - left side
        int cx1 = width / 4;
        int cy = height / 2;
        int r = 80;
        svg << "  <circle cx=\"" << cx1 << "\" cy=\"" << cy << "\" r=\"" << r 
            << "\" fill=\"url(#lightGrad)\" stroke=\"#FFD700\" stroke-width=\"2\"/>\n";
        svg << "  <text x=\"" << cx1 << "\" y=\"" << cy << "\" text-anchor=\"middle\" "
            << "fill=\"white\" font-size=\"16\" font-weight=\"bold\">L</text>\n";
        svg << "  <text x=\"" << cx1 << "\" y=\"" << (cy + r + 20) << "\" text-anchor=\"middle\" "
            << "fill=\"#888\" font-size=\"12\">Passive Perspective</text>\n";
        svg << "  <text x=\"" << cx1 << "\" y=\"" << (cy + r + 35) << "\" text-anchor=\"middle\" "
            << "fill=\"#666\" font-size=\"10\">(from Darkness)</text>\n";
        
        // Active perspective (cone) - right side
        int cx2 = 3 * width / 4;
        svg << "  <path d=\"M " << (cx2-60) << " " << (cy+60) << " L " << cx2 << " " << (cy-60) 
            << " L " << (cx2+60) << " " << (cy+60) << " Z\" fill=\"url(#lightGrad)\" "
            << "stroke=\"#FFD700\" stroke-width=\"2\"/>\n";
        svg << "  <circle cx=\"" << cx2 << "\" cy=\"" << (cy-40) << "\" r=\"15\" "
            << "fill=\"#FFD700\" stroke=\"white\" stroke-width=\"1\"/>\n";
        svg << "  <text x=\"" << cx2 << "\" y=\"" << (cy-35) << "\" text-anchor=\"middle\" "
            << "fill=\"#1a1a2e\" font-size=\"12\" font-weight=\"bold\">L</text>\n";
        svg << "  <text x=\"" << cx2 << "\" y=\"" << (cy + 80) << "\" text-anchor=\"middle\" "
            << "fill=\"#888\" font-size=\"12\">Active Perspective</text>\n";
        svg << "  <text x=\"" << cx2 << "\" y=\"" << (cy + 95) << "\" text-anchor=\"middle\" "
            << "fill=\"#666\" font-size=\"10\">(from Light)</text>\n";
        
        // Labels
        svg << "  <text x=\"" << (cx1 + r + 15) << "\" y=\"" << cy << "\" "
            << "fill=\"#888\" font-size=\"11\">D</text>\n";
        svg << "  <text x=\"" << (cx2 + 70) << "\" y=\"" << (cy + 50) << "\" "
            << "fill=\"#888\" font-size=\"11\">D</text>\n";
        
        // Interface label
        svg << "  <text x=\"" << cx2 << "\" y=\"" << (cy + 20) << "\" text-anchor=\"middle\" "
            << "fill=\"#FFD700\" font-size=\"10\">Active Interface</text>\n";
        
        svg << "</svg>\n";
        return svg.str();
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

/**
 * @brief The Loon Analogy from Fisherman's Guide
 * 
 * From the text: "Each loon is an active interface with sky. It absorbs subtle
 * energies from the sky, then projects them back enhanced."
 * 
 * This class demonstrates System 1 principles through the loon example.
 */
class LoonAnalogy {
public:
    LoonAnalogy() : system_(1.0, 1.0, 1.0) {}
    
    /**
     * @brief Simulate the loon's communication with sky
     * 
     * The loon absorbs energies (reflux) and projects them back enhanced (efflux).
     */
    void communicate(double dt = 1.0) {
        // Loon absorbs from sky
        double absorbed = system_.interface().refluxRate() * dt;
        
        // Loon enhances and projects back
        double enhanced = absorbed * enhancement_factor_;
        system_.interface().setEffluxRate(enhanced);
        
        system_.step(dt);
    }
    
    /// Get the communicative balance
    double communicativeBalance() const {
        return system_.interface().communicativeBalance();
    }
    
    /// Set enhancement factor (how much the loon enhances absorbed energy)
    void setEnhancementFactor(double factor) { enhancement_factor_ = factor; }
    
    /// Get the underlying System 1
    const System1& system() const { return system_; }
    
    /**
     * @brief Get description from Fisherman's Guide
     */
    static std::string description() {
        return "The loon demonstrates System 1 principles: it is an active interface "
               "with sky, absorbing subtle energies and projecting them back enhanced. "
               "It maintains direct awareness of the communicative balance with sky.";
    }

private:
    System1 system_;
    double enhancement_factor_ = 1.1;
};

} // namespace system1
} // namespace cosmic
