#pragma once

/**
 * File class
 * Stores file data that can be accessed in any way like string or binary
 */

// std
#include <vector>
#include <string>

// core
#include "Wrangler/Core/Types.hpp"

namespace Wrangler {

    /// @brief Content agnostic file container
    class File {
    private:
        std::vector<byte> data; 
    public:
        File(const std::vector<byte>& data);

        std::string asString() const;
        const std::vector<byte>& asBinary() const;
    };

}