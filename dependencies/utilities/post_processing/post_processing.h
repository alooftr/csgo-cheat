#pragma once

struct ImDrawList;

#ifdef _WIN32
struct IDirect3DDevice9;
#endif

namespace post_processing
{
#ifdef _WIN32
    void set_device(IDirect3DDevice9* device) noexcept;
    void clear_blur_textures() noexcept;
    void on_device_reset() noexcept;
#endif
    void new_frame() noexcept;
    void perform_blur(ImDrawList* drawList, float alpha, int w, int h) noexcept;
}
