#pragma once

/**
 * Model class
 * Contains multiple meshes
 */

#include <gltfio/AssetLoader.h>
#ifdef assert_invariant
    #undef assert_invariant
#endif

namespace Wrangler {

    struct Model {
        filament::gltfio::FilamentAsset* asset = nullptr;
    };

}