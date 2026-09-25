#include "Wrangler/Filesystem/File.hpp"

Wrangler::File::File(const std::vector<Wrangler::byte> &data): data(data) {}

/// @brief Returns file content in text
/// @return File data as a std::string
std::string Wrangler::File::asString() const
{
    return std::string(data.begin(), data.end());
}

/// @brief Returns file content in binary
/// @return File data as a std::vector<Wrangler::byte>
const std::vector<Wrangler::byte> &Wrangler::File::asBinary() const
{
    return data;
}
