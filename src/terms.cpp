/**
 * @file terms.cpp
 * @brief Implementation of system terms functionality
 * 
 * Most functionality is provided as inline functions in terms.hpp.
 * This file contains non-inline implementations for complex operations.
 */

#include "cosmic/terms.hpp"
#include <sstream>
#include <algorithm>

namespace cosmic {
namespace terms {

// ============================================================================
// Helper Functions
// ============================================================================

/**
 * @brief Generate a nested term description from position addresses
 */
std::string generateNestedTermDescription(const std::vector<int>& positions) {
    if (positions.empty()) return "";
    
    auto sys4terms = getSystem4Terms();
    std::stringstream ss;
    
    for (size_t i = 0; i < positions.size(); ++i) {
        int pos = positions[i];
        
        // Find the term for this position
        for (const auto& term : sys4terms) {
            if (term.position == pos) {
                if (i > 0) ss << " within ";
                ss << term.name;
                break;
            }
        }
    }
    
    return ss.str();
}

/**
 * @brief Generate a term address string
 */
std::string generateAddress(const std::vector<int>& positions) {
    std::stringstream ss;
    for (size_t i = 0; i < positions.size(); ++i) {
        if (i > 0) ss << ".";
        ss << positions[i];
    }
    return ss.str();
}

/**
 * @brief Generate a term code string (e.g., "T1E.T4E.T2E")
 */
std::string generateTermCode(const std::vector<int>& positions) {
    auto sys4terms = getSystem4Terms();
    std::stringstream ss;
    
    for (size_t i = 0; i < positions.size(); ++i) {
        int pos = positions[i];
        
        // Find the term for this position
        for (const auto& term : sys4terms) {
            if (term.position == pos) {
                if (i > 0) ss << ".";
                ss << term.shortName;
                break;
            }
        }
    }
    
    return ss.str();
}

// ============================================================================
// Term Navigation Class
// ============================================================================

class TermNavigator {
public:
    explicit TermNavigator(int systemLevel) : system_level_(systemLevel) {}
    
    /**
     * @brief Get term at a specific address
     * @param address Dot-separated position string (e.g., "1.4.2")
     * @return Term description
     */
    std::optional<std::string> getTermAt(const std::string& address) const {
        auto positions = parseAddress(address);
        if (positions.empty()) return std::nullopt;
        
        // Validate positions
        for (int pos : positions) {
            if (pos < 1 || pos > 9) return std::nullopt;
        }
        
        // Check if address depth is valid for system level
        int maxDepth = getMaxDepth();
        if (static_cast<int>(positions.size()) > maxDepth) return std::nullopt;
        
        return generateNestedTermDescription(positions);
    }
    
    /**
     * @brief Get all valid addresses at a given depth
     */
    std::vector<std::string> getAllAddresses(int depth) const {
        std::vector<std::string> addresses;
        generateAddressesRecursive(addresses, "", depth, 0);
        return addresses;
    }
    
private:
    std::vector<int> parseAddress(const std::string& address) const {
        std::vector<int> positions;
        std::stringstream ss(address);
        std::string token;
        
        while (std::getline(ss, token, '.')) {
            try {
                positions.push_back(std::stoi(token));
            } catch (...) {
                return {};
            }
        }
        
        return positions;
    }
    
    int getMaxDepth() const {
        switch (system_level_) {
            case 0: return 0;
            case 1: return 0;
            case 2: return 0;
            case 3: return 1;
            case 4: return 1;
            case 5: return 1;
            case 6: return 1;
            case 7: return 2;
            case 8: return 2;
            case 9: return 3;
            case 10: return 4;
            default: return 0;
        }
    }
    
    void generateAddressesRecursive(std::vector<std::string>& addresses,
                                     const std::string& prefix,
                                     int targetDepth,
                                     int currentDepth) const {
        if (currentDepth == targetDepth) {
            if (!prefix.empty()) {
                addresses.push_back(prefix);
            }
            return;
        }
        
        for (int i = 1; i <= 9; ++i) {
            std::string newPrefix = prefix.empty() ? 
                std::to_string(i) : 
                prefix + "." + std::to_string(i);
            generateAddressesRecursive(addresses, newPrefix, targetDepth, currentDepth + 1);
        }
    }
    
    int system_level_;
};

// ============================================================================
// Process Sequence Generator
// ============================================================================

/**
 * @brief Process step information
 */
struct ProcessStep {
    int position;
    std::string name;
    bool isShockPoint;
    std::string phase;
};

/**
 * @brief Generate the creative process sequence through the enneagram
 */
class ProcessSequenceGenerator {
public:
    /**
     * @brief Get the full process sequence with shock points
     */
    static std::vector<ProcessStep> getFullSequence() {
        return {
            {1, "Perception of Need", false, "Initiating"},
            {4, "Organized Input", false, "Developing"},
            {2, "Idea Creation", false, "Developing"},
            {3, "Idea Transference", true, "First Shock Point"},
            {8, "Response to Need", false, "Maturing"},
            {5, "Physical Action", false, "Maturing"},
            {7, "Quantized Memory", false, "Maturing"},
            {6, "Corporeal Body", true, "Second Shock Point"},
            {9, "Discretionary Hierarchy", false, "Completion"}
        };
    }
    
    /**
     * @brief Get the next position in the process
     */
    static int nextPosition(int current) {
        static const std::map<int, int> sequence = {
            {1, 4}, {4, 2}, {2, 3}, {3, 8}, {8, 5}, {5, 7}, {7, 6}, {6, 9}, {9, 1}
        };
        auto it = sequence.find(current);
        return (it != sequence.end()) ? it->second : 1;
    }
};

// ============================================================================
// Verification Functions
// ============================================================================

/**
 * @brief Verify that term counts match OEIS A000081
 */
bool verifyTermCounts() {
    for (int level = 0; level <= 10; ++level) {
        size_t expected = termCountForLevel(level);
        size_t actual = A000081[level + 1];
        if (expected != actual) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Verify that cluster counts match OEIS A000055
 */
bool verifyClusterCounts() {
    for (int level = 0; level <= 10; ++level) {
        size_t expected = clusterCountForLevel(level);
        size_t actual = A000055[level + 1];
        if (expected != actual) {
            return false;
        }
    }
    return true;
}

} // namespace terms
} // namespace cosmic
