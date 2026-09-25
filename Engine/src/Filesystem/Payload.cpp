#include "Wrangler/Filesystem/Payload.hpp"

// tul
#include <tul/ErrorOps.hpp>

/// @brief Reads a payload file
/// @param payloadData JSON file contents
/// @return False on failure
bool Wrangler::Payload::read(const std::string &payloadData)
{
    json data;

    try {
        data = json::parse(payloadData);
    }
    catch (const json::parse_error& e) {
        tul::Alert({"Failed to parse Payload file\n", e.what()});
        return false;
    }

    if (!data.is_object())
        return false;

    if (!data.contains("type") ||
        data["type"] != "Wrangler Payload")
        return false;

    if (!data.contains("content") ||
        !data["content"].is_string())
        return false;

    contentType = data["content"].get<std::string>();

    if (!data.contains("version") ||
        !data["version"].is_number_integer())
        return false;

    contentVersion = data["version"].get<int>();

    if (contentVersion < 1)
        return false;

    if (!data.contains("payload"))
        return false;

    content = data["payload"];

    return true;
}
