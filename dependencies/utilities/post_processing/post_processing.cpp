#include <d3d9.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#include "../../imgui/imgui.h"

// shaders are build during compilation and header files are created
#ifdef _DEBUG
#include "../../resources/Shaders/Build/Debug/blur_x.h"
#include "../../resources/Shaders/Build/Debug/blur_y.h"
#include "../../resources/Shaders/Build/Debug/chromatic_aberration.h"
#include "../../resources/Shaders/Build/Debug/monochrome.h"
#endif

#include "post_processing.h"

static int backbufferWidth = 0;
static int backbufferHeight = 0;

static IDirect3DDevice9* device; // DO NOT RELEASE!

[[nodiscard]] static IDirect3DTexture9* createTexture(int width, int height) noexcept
{
    IDirect3DTexture9* texture;
    device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr);
    return texture;
}

static void copyBackbufferToTexture(IDirect3DTexture9* texture, D3DTEXTUREFILTERTYPE filtering) noexcept
{
    if (ComPtr<IDirect3DSurface9> backBuffer; device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, backBuffer.GetAddressOf()) == D3D_OK) {
        if (ComPtr<IDirect3DSurface9> surface; texture->GetSurfaceLevel(0, surface.GetAddressOf()) == D3D_OK)
            device->StretchRect(backBuffer.Get(), nullptr, surface.Get(), nullptr, filtering);
    }
}

static void setRenderTarget(IDirect3DTexture9* rtTexture) noexcept
{
    if (ComPtr<IDirect3DSurface9> surface; rtTexture->GetSurfaceLevel(0, surface.GetAddressOf()) == D3D_OK)
        device->SetRenderTarget(0, surface.Get());
}

class ShaderProgram {
public:
    ~ShaderProgram() {
#ifndef _WIN32
        if (program)
            glDeleteProgram(program);
#endif
    }

    void use(float uniform, int location) const noexcept
    {
        device->SetPixelShader(pixelShader.Get());
        const float params[4] = { uniform };
        device->SetPixelShaderConstantF(location, params, 1);
    }

    void init(const BYTE* pixelShaderSrc) noexcept
    {
        if (initialized)
            return;
        initialized = true;

        device->CreatePixelShader(reinterpret_cast<const DWORD*>(pixelShaderSrc), pixelShader.GetAddressOf());
    }

private:
    ComPtr<IDirect3DPixelShader9> pixelShader;

    bool initialized = false;
};

class BlurEffect {
public:
    static void draw(ImDrawList* drawList, float alpha) noexcept
    {
        instance()._draw(drawList, alpha);
    }

    static void clearTextures() noexcept
    {
        if (instance().blurTexture1) {
            instance().blurTexture1->Release();
            instance().blurTexture1 = nullptr;
        }
        if (instance().blurTexture2) {
            instance().blurTexture2->Release();
            instance().blurTexture2 = nullptr;
        }
    }
private:
    IDirect3DSurface9* rtBackup = nullptr;
    IDirect3DTexture9* blurTexture1 = nullptr;
    IDirect3DTexture9* blurTexture2 = nullptr;

    ShaderProgram blurShaderX;
    ShaderProgram blurShaderY;
    static constexpr auto blurDownsample = 6;

    BlurEffect() = default;
    BlurEffect(const BlurEffect&) = delete;

    ~BlurEffect()
    {
        if (rtBackup)
            rtBackup->Release();
        if (blurTexture1)
            blurTexture1->Release();
        if (blurTexture2)
            blurTexture2->Release();
    }

    static BlurEffect& instance() noexcept
    {
        static BlurEffect blurEffect;
        return blurEffect;
    }

    static void begin(const ImDrawList*, const ImDrawCmd*) noexcept { instance()._begin(); }
    static void firstPass(const ImDrawList*, const ImDrawCmd*) noexcept { instance()._firstPass(); }
    static void secondPass(const ImDrawList*, const ImDrawCmd*) noexcept { instance()._secondPass(); }
    static void end(const ImDrawList*, const ImDrawCmd*) noexcept { instance()._end(); }

    void createTextures() noexcept
    {
        if (!blurTexture1)
            blurTexture1 = createTexture(backbufferWidth / blurDownsample, backbufferHeight / blurDownsample);
        if (!blurTexture2)
            blurTexture2 = createTexture(backbufferWidth / blurDownsample, backbufferHeight / blurDownsample);
    }

    void createShaders() noexcept
    {
        blurShaderX.init(blur_x);
        blurShaderY.init(blur_y);
    }

    void _begin() noexcept
    {
        device->GetRenderTarget(0, &rtBackup);

        copyBackbufferToTexture(blurTexture1, D3DTEXF_LINEAR);

        device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

        const D3DMATRIX projection{ {{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f / (backbufferWidth / blurDownsample), 1.0f / (backbufferHeight / blurDownsample), 0.0f, 1.0f
        }} };
        device->SetVertexShaderConstantF(0, &projection.m[0][0], 4);
    }

    void _firstPass() noexcept
    {
        blurShaderX.use(1.0f / (backbufferWidth / blurDownsample), 0);
        setRenderTarget(blurTexture2);
    }

    void _secondPass() noexcept
    {
        blurShaderY.use(1.0f / (backbufferHeight / blurDownsample), 0);
        setRenderTarget(blurTexture1);
    }

    void _end() noexcept
    {
        device->SetRenderTarget(0, rtBackup);
        rtBackup->Release();

        device->SetPixelShader(nullptr);
        device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
    }

    void _draw(ImDrawList* drawList, float alpha) noexcept
    {
        createTextures();
        createShaders();

        if (!blurTexture1 || !blurTexture2)
            return;

        drawList->AddCallback(&begin, nullptr);
        for (int i = 0; i < 8; ++i) {
            drawList->AddCallback(&firstPass, nullptr);
            drawList->AddImage(reinterpret_cast<ImTextureID>(blurTexture1), { -1.0f, -1.0f }, { 1.0f, 1.0f });
            drawList->AddCallback(&secondPass, nullptr);
            drawList->AddImage(reinterpret_cast<ImTextureID>(blurTexture2), { -1.0f, -1.0f }, { 1.0f, 1.0f });
        }
        drawList->AddCallback(&end, nullptr);
        drawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

        drawList->AddImage(reinterpret_cast<ImTextureID>(blurTexture1), { 0.0f, 0.0f }, { backbufferWidth * 1.0f, backbufferHeight * 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, IM_COL32(255, 255, 255, 255 * alpha));
    }
};

void post_processing::set_device(IDirect3DDevice9* device) noexcept
{
    ::device = device;
}

void post_processing::clear_blur_textures() noexcept
{
    BlurEffect::clearTextures();
}

void post_processing::on_device_reset() noexcept
{
    BlurEffect::clearTextures();
}

void post_processing::new_frame() noexcept
{   
    BlurEffect::clearTextures();
}

void post_processing::perform_blur(ImDrawList* drawList, float alpha, int w, int h) noexcept
{
    backbufferWidth = static_cast<int>(w);
    backbufferHeight = static_cast<int>(h);
    BlurEffect::draw(drawList, alpha);
}