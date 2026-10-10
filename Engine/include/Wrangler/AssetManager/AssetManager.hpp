#pragma once

/**
 * AssetManager class
 * Stores asset data in memory (READ FROM DISK VIA FILESYSTEM CLASS)
 */

// std
#include <unordered_map>
#include <memory>

// core
#include "Wrangler/Core/Types.hpp"

// filesystem
#include "Wrangler/Filesystem/Filesystem.hpp"

// renderer
#include "Wrangler/Renderer/Model.hpp"
#include "Wrangler/Renderer/Material.hpp"
#include "Wrangler/Renderer/Texture.hpp"

#include <gltfio/AssetLoader.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/TextureProvider.h>

namespace Wrangler {
    
    class AssetManager {
    private:
        std::unordered_map<Wrangler::AssetID, std::shared_ptr<Model>> models;
        // std::unordered_map<Wrangler::AssetID, std::shared_ptr<Shader>> shaders;
        std::unordered_map<Wrangler::AssetID, std::shared_ptr<Texture>> textures;

        const Filesystem& fs;


    public:

        filament::gltfio::AssetLoader* assetLoader = nullptr; // general purpose
        filament::gltfio::MaterialProvider* materialProvider = nullptr;
        filament::gltfio::ResourceLoader* resourceLoader = nullptr; // filament specific files
        filament::gltfio::TextureProvider* stbDecoder = nullptr;
        filament::gltfio::TextureProvider* ktxDecoder = nullptr;

        AssetManager(const Filesystem& fs, filament::Engine* filamentEngine);

        AssetID loadModel(const std::string& path);
        std::shared_ptr<const Model> getModel(AssetID id) const;

        AssetID loadTexture(const std::string& path);
        std::shared_ptr<const Texture> getTexture(AssetID id) const;

        ~AssetManager();
    };

}