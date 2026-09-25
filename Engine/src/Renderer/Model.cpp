#include "Wrangler/Renderer/Model.hpp"

// tul
#include <tul/ErrorOps.hpp>

// 3rd party
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// @brief Processes a node
/// @param node 
/// @param scene 
void Wrangler::Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        CPUMesh cpuMesh = processCPUMesh(mesh, scene);
        meshes.push_back(uploadMesh(cpuMesh));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

/// @brief Processes a mesh into CPU mesh
/// @param mesh 
/// @param scene 
/// @return Data
Wrangler::CPUMesh Wrangler::Model::processCPUMesh(aiMesh *mesh, const aiScene *scene)
{
    CPUMesh data;
    // vertices
    data.vertices.reserve(mesh->mNumVertices);
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // tex coords
        if(mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x; 
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        } else vertex.texCoords = Vec2{0.0f, 0.0f};

        data.vertices.emplace_back(vertex);
    }

    // indices
    data.indices.reserve(mesh->mNumFaces);
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            data.indices.push_back(face.mIndices[j]);
    }

    data.materialSlot = mesh->mMaterialIndex;
    return data;
}

/// @brief Turns CPU mesh data into GPU mesh data
/// @param mesh 
/// @return GPU meshes
Wrangler::GPUMesh Wrangler::Model::uploadMesh(const CPUMesh &mesh)
{
    GPUMesh gpuMesh;
    gpuMesh.materialSlot = mesh.materialSlot;

    // Vertex buffer
    const bgfx::Memory* vertexMemory = bgfx::copy(
        mesh.vertices.data(),
        static_cast<uint32_t>(
            mesh.vertices.size() * sizeof(Vertex)
        )
    );

    gpuMesh.vertexBuffer = 
        bgfx::createVertexBuffer(
            vertexMemory,
            Vertex::layout
        );

    // indices
    const bgfx::Memory* indexMemory = bgfx::copy(
        mesh.indices.data(),
        static_cast<uint32_t>(
            mesh.indices.size() * sizeof(uint32_t)
        )
    );

    gpuMesh.indexBuffer = 
        bgfx::createIndexBuffer(
            indexMemory,
            BGFX_BUFFER_INDEX32
        );

    return gpuMesh;
}

/// @brief Parses a model and saves the GPU mesh data into memory
/// @param data GLTF model data
void Wrangler::Model::loadModel(const std::vector<byte> &data)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(
        data.data(),
        data.size(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType |
        aiProcess_PreTransformVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_FlipUVs,
        "fbx"
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        tul::Alert({"Failed to load model\nAssimp Error: ", importer.GetErrorString()});
        return;
    }
    
    meshes.reserve(scene->mNumMeshes);
    processNode(scene->mRootNode, scene);
    tul::Print({"[Model] Loaded a model\n"});
}