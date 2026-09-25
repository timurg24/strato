#pragma once

/**
 * Texture class
 * Contains GPU image data
 */

// 3rd party
#include <bgfx/bgfx.h>

namespace Wrangler {

    class Texture {
    private:
        bgfx::TextureHandle m_handle = BGFX_INVALID_HANDLE;
    public:
        Texture(bgfx::TextureHandle handle): m_handle(handle) {}
        bgfx::TextureHandle handle() const { return m_handle; }
        ~Texture() {
            if(bgfx::isValid(m_handle)) bgfx::destroy(m_handle);
        }
    };

}