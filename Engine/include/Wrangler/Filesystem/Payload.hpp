#pragma once

/**
 * Payload class
 * Reads Wrangler Payload config files
 */

// std
#include <string>

// json
#include <json.hpp>
using json = nlohmann::json;

namespace Wrangler {

    class Payload {
    public:
        std::string contentType;
        int         contentVersion;

        json        content;
        bool read(const std::string& payloadData);
    };

}