#include "Wrangler/AssetManager/AssetManager.hpp"

// tul
#include <tul/StringOps.hpp>
#include <tul/ErrorOps.hpp>

// 3rd party
#include <stb_image.h>


Wrangler::AssetManager::AssetManager(const Filesystem &fs) : fs(fs) {
    tul::Print({"[AssetManager] AssetManager initialized\n"});
}

/// @brief Loads a model
/// @param path Model path (used for AssetID)
Wrangler::AssetID Wrangler::AssetManager::loadModel(const std::string &path)
{
    AssetID id = tul::HashString(path);
    if(textures.contains(id)) return id;
    const std::vector<byte> data = fs.readFile(path).asBinary();

    auto model = std::make_shared<Model>();

    models.insert_or_assign(
        id,
        model
    );

    return id;
}

std::shared_ptr<const Wrangler::Model> Wrangler::AssetManager::getModel(AssetID id) const
{
    auto it = models.find(id);

    if (it == models.end())
        return nullptr;

    return it->second;
}

/// @brief Loads a texture into memory
/// @param path 
/// @return 
Wrangler::AssetID Wrangler::AssetManager::loadTexture(const std::string &path)
{
    AssetID id = tul::HashString(path);
    if(textures.contains(id)) return id;
    const std::vector<byte> data = fs.readFile(path).asBinary();

    int width, height, channels;

    stbi_uc* pixels = stbi_load_from_memory(
        data.data(),
        data.size(),
        &width,
        &height,
        &channels,
        4
    );

    if(!pixels) tul::Alert({"Failed to load texture from archive: ", path});

    const uint32_t size = 
        static_cast<uint32_t>(
            width * height * 4
        );

    stbi_image_free(pixels);
    
    return id;
}

/// @brief Returns a texture object based on the AssetID
/// @param id AssetID
/// @return Texture object
std::shared_ptr<const Wrangler::Texture> Wrangler::AssetManager::getTexture(AssetID id) const
{
    auto it = textures.find(id);

    if (it == textures.end())
        return nullptr;

    return it->second;
}