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
#include "Wrangler/Renderer/Shader.hpp"
#include "Wrangler/Renderer/Texture.hpp"
#include "Wrangler/Renderer/CPUMesh.hpp"

namespace Wrangler {
    
    class AssetManager {
    private:
        std::unordered_map<Wrangler::AssetID, std::shared_ptr<Model>> models;
        std::unordered_map<Wrangler::AssetID, std::shared_ptr<Material>> materials;
        // std::unordered_map<Wrangler::AssetID, std::shared_ptr<Shader>> shaders;
        std::unordered_map<Wrangler::AssetID, std::shared_ptr<Texture>> textures;

        const Filesystem& fs;
    public:
        AssetManager(const Filesystem& fs);

        AssetID loadModel(const std::string& path);
        std::shared_ptr<const Model> getModel(AssetID id) const;

        AssetID loadTexture(const std::string& path);
        std::shared_ptr<const Texture> getTexture(AssetID id) const;

        AssetID loadMaterial(const std::string& path, Shader& shader);
        std::shared_ptr<const Material> getMaterial(AssetID id) const;
    };

}