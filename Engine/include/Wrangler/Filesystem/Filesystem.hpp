#pragma once

/**
 * Filesystem class
 * Handles retrieving file data from the game assets (FROM DISK)
 */

// std
#include <filesystem>

// fs
#include "Wrangler/Filesystem/File.hpp"

namespace Wrangler {

    class Filesystem {
    public:
        Filesystem() = default;
        explicit Filesystem(const char* argv0, const std::filesystem::path& contentPath);
        void init(const char* argv0, const std::filesystem::path& contentPath);
        const File readFile(const std::string& path) const;
        ~Filesystem();
    private:
        bool initialized = false;
    };

}