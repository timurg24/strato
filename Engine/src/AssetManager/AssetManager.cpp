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

    model->loadModel(data);

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
    
    const bgfx::Memory* memory = 
        bgfx::copy(
            pixels,
            size
        );

    stbi_image_free(pixels);

    bgfx::TextureHandle texture = 
        bgfx::createTexture2D(
            static_cast<uint16_t>(width),
            static_cast<uint16_t>(height),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            0,
            memory
        );

    if(!bgfx::isValid(texture)) tul::Alert({"Failed to create GPU texture: ", path});

    textures.insert_or_assign(
        id,
        std::make_unique<Texture>(texture)
    );

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

/// @brief Loads a material. what did you expect?
/// @param path Path
/// @return AssetID
Wrangler::AssetID Wrangler::AssetManager::loadMaterial(const std::string &path, Shader& shader)
{
    AssetID id = tul::HashString(path);
    if(materials.contains(id)) return id;
    const std::string data = fs.readFile(path).asString();

    Payload pay;
    if (!pay.read(data))
    {
        tul::Alert({
            "Failed to read material payload: ",
            path
        });

        return 0;
    }

    Material mat;
    mat.loadPayload(*this, pay, shader);

    materials.insert_or_assign(
        id,
        std::make_unique<Material>(mat)
    );

    return id;
}

/// @brief Returns a material object based on the AssetID
/// @param id AssetID
/// @return Material object
std::shared_ptr<const Wrangler::Material> Wrangler::AssetManager::getMaterial(AssetID id) const
{
    auto it = materials.find(id);

    if (it == materials.end())
        return nullptr;

    return it->second;
}
