#include "Terrain/Terrain.hpp"

#include "HttpClient/HttpClient.hpp"

/// @brief Requests elevation textures and sets the sampler to it
void Strato::Terrain::updateTerrainTexture()
{
    auto data = HttpClient::get("test");
}

Strato::Terrain::Terrain(filament::Engine &engine)
{
    // create terrain texture
    elevationTexture = filament::Texture::Builder()
        .width(STRATO_TERRAIN_QUALITY)
        .height(STRATO_TERRAIN_QUALITY)
        .levels(1)
        .sampler(filament::Texture::Sampler::SAMPLER_2D)
        .format(filament::Texture::InternalFormat::RGBA8)
        .build(engine);
}