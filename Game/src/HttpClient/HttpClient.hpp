#pragma once

/**
 * HttpClient class
 */

#include <vector>
#include <cstdint>
#include <string>

namespace Strato {

    class HttpClient {
    private:
        static size_t writeCallback(char* data, size_t size, size_t count, void* userData);
    public:
        static std::vector<uint8_t> get(const std::string& url);
    };
}