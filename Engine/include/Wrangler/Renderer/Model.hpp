#pragma once

/**
 * Model class
 * Contains multiple meshes
 */

// std
#include <vector>

// renderer
#include "CPUMesh.hpp"
#include "GPUMesh.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Wrangler {

    // TODO: Move loader fucntions into AssetManager
    class Model {
        

        void processNode(
            aiNode* node,
            const aiScene* scene
        );

        CPUMesh processCPUMesh(
            aiMesh* mesh,
            const aiScene* scene
        );

        GPUMesh uploadMesh(
            const CPUMesh& mesh
        );
    public:
        std::vector<GPUMesh> meshes;
        void loadModel(const std::vector<byte>& data);
    };

}