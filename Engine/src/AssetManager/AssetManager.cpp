#include "Wrangler/AssetManager/AssetManager.hpp"

// tul
#include <tul/StringOps.hpp>
#include <tul/ErrorOps.hpp>

// 3rd party
#include <stb_image.h>


Wrangler::AssetManager::AssetManager(const Filesystem &fs, filament::Engine* filamentEngine) : fs(fs) {

    filament::gltfio::ResourceConfiguration config{};
    config.engine = filamentEngine;
    config.gltfPath = "";

    materialProvider = filament::gltfio::createJitShaderProvider(
        filamentEngine,
        true,
        {}
    );

    resourceLoader = new filament::gltfio::ResourceLoader(config);
    stbDecoder = filament::gltfio::createStbProvider(filamentEngine);
    ktxDecoder = filament::gltfio::createKtx2Provider(filamentEngine);

    resourceLoader->addTextureProvider("image/png", stbDecoder);
    resourceLoader->addTextureProvider("image/jpeg", stbDecoder);
    resourceLoader->addTextureProvider("image/ktx2", ktxDecoder);

    resourceLoader->setConfiguration(config);

    assetLoader = filament::gltfio::AssetLoader::create({
        filamentEngine,
        materialProvider
    });

    tul::Print({"[AssetManager] AssetManager initialized\n"});
}

/// @brief Loads a model
/// @param path Model path (used for AssetID)
Wrangler::AssetID Wrangler::AssetManager::loadModel(const std::string &path)
{
    AssetID id = tul::HashString(path);
    if(models.contains(id)) return id;
    const std::vector<byte> data = fs.readFile(path).asBinary();

    auto model = std::make_shared<Model>();

    models.insert_or_assign(
        id,
        model
    );

    model->asset = assetLoader->createAsset(
        data.data(),
        data.size()
    );

    if(!model->asset) tul::Alert({"Failed to parse model: ", path});
    if (!resourceLoader->loadResources(model->asset)) tul::FatalError({"Failed to load GLB resources: ", path});

    model->asset->releaseSourceData();

    return id;
}

std::shared_ptr<const Wrangler::Model> Wrangler::AssetManager::getModel(AssetID id) const
{
    auto it = models.find(id);

    if (it == models.end())
        return nullptr;

    return it->second;
}