#include "Wrangler/Renderer/Renderer.hpp"

// fs
#include "Wrangler/Filesystem/Payload.hpp"

// tul
#include <tul/ErrorOps.hpp>

Wrangler::Renderer::Renderer(const RendererParameters &params) : assets(params.assets),
                                                                 fs(params.fs),
                                                                 width(params.width),
                                                                 height(params.height)
{
    filamentRenderer = params.filamentEngine->createRenderer();
}


Wrangler::Renderer::~Renderer() {}
