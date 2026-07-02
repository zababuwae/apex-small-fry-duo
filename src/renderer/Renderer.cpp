#include "Renderer.h"
#include "core/DebugLog.h"

#include <d3dcompiler.h>

#include <stdexcept>

namespace
{
void ThrowIfFailed(HRESULT result, const char* message)
{
    if (FAILED(result))
    {
        throw std::runtime_error(message);
    }
}

Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
    const std::wstring& path,
    const char* entryPoint,
    const char* target)
{
    UINT compileFlags = 0;
#if defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> byteCode;
    Microsoft::WRL::ComPtr<ID3DBlob> errors;
    const HRESULT result = D3DCompileFromFile(
        path.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        target,
        compileFlags,
        0,
        byteCode.GetAddressOf(),
        errors.GetAddressOf());

    if (FAILED(result))
    {
        if (errors != nullptr)
        {
            DebugLog::Error(static_cast<const char*>(errors->GetBufferPointer()));
        }
        throw std::runtime_error("Failed to compile shader.");
    }

    return byteCode;
}
}

void Renderer::Initialize(HWND windowHandle, UINT width, UINT height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDescription{};
    swapChainDescription.BufferDesc.Width = width;
    swapChainDescription.BufferDesc.Height = height;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.BufferCount = 2;
    swapChainDescription.OutputWindow = windowHandle;
    swapChainDescription.Windowed = TRUE;
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT creationFlags = 0;
#if defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL requestedFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL selectedFeatureLevel{};
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        requestedFeatureLevels,
        static_cast<UINT>(std::size(requestedFeatureLevels)),
        D3D11_SDK_VERSION,
        &swapChainDescription,
        swapChain_.GetAddressOf(),
        device_.GetAddressOf(),
        &selectedFeatureLevel,
        deviceContext_.GetAddressOf());

    if (result == E_INVALIDARG)
    {
        DebugLog::Warning("Feature level 11.1 unavailable. Retrying with 11.0.");
        result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &requestedFeatureLevels[1],
            1,
            D3D11_SDK_VERSION,
            &swapChainDescription,
            swapChain_.GetAddressOf(),
            device_.GetAddressOf(),
            &selectedFeatureLevel,
            deviceContext_.GetAddressOf());
    }

#if defined(_DEBUG)
    if (result == DXGI_ERROR_SDK_COMPONENT_MISSING)
    {
        DebugLog::Warning("DX11 debug layer unavailable. Retrying without it.");
        creationFlags &= ~D3D11_CREATE_DEVICE_DEBUG;
        result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            requestedFeatureLevels,
            static_cast<UINT>(std::size(requestedFeatureLevels)),
            D3D11_SDK_VERSION,
            &swapChainDescription,
            swapChain_.ReleaseAndGetAddressOf(),
            device_.ReleaseAndGetAddressOf(),
            &selectedFeatureLevel,
            deviceContext_.ReleaseAndGetAddressOf());
    }
#endif

    ThrowIfFailed(result, "Failed to initialize DirectX 11.");
    CreateRenderTarget();
    SetViewport(width, height);
    DebugLog::Format("DX11 ready. FeatureLevel=0x%04X", static_cast<unsigned int>(selectedFeatureLevel));
}

void Renderer::CreateTriangleShaders(const std::wstring& shaderPath)
{
    const Microsoft::WRL::ComPtr<ID3DBlob> vertexByteCode =
        CompileShader(shaderPath, "VSMain", "vs_5_0");
    const Microsoft::WRL::ComPtr<ID3DBlob> pixelByteCode =
        CompileShader(shaderPath, "PSMain", "ps_5_0");

    ThrowIfFailed(
        device_->CreateVertexShader(
            vertexByteCode->GetBufferPointer(),
            vertexByteCode->GetBufferSize(),
            nullptr,
            vertexShader_.GetAddressOf()),
        "Failed to create the vertex shader.");

    ThrowIfFailed(
        device_->CreatePixelShader(
            pixelByteCode->GetBufferPointer(),
            pixelByteCode->GetBufferSize(),
            nullptr,
            pixelShader_.GetAddressOf()),
        "Failed to create the pixel shader.");

    DebugLog::Info("Triangle shaders compiled.");
}

void Renderer::Resize(UINT width, UINT height)
{
    if (swapChain_ == nullptr || width == 0 || height == 0)
    {
        return;
    }

    deviceContext_->OMSetRenderTargets(0, nullptr, nullptr);
    renderTargetView_.Reset();

    ThrowIfFailed(
        swapChain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0),
        "Failed to resize the swap chain.");

    CreateRenderTarget();
    SetViewport(width, height);
    DebugLog::Format("Render target resized. Size=%ux%u", width, height);
}

void Renderer::BeginFrame(float red, float green, float blue, float alpha) const
{
    Clear(red, green, blue, alpha);
}

void Renderer::DrawTriangle() const
{
    // No vertex buffer / input layout: the vertices are generated in the
    // vertex shader from SV_VertexID, so we only bind shaders and issue Draw.
    deviceContext_->IASetInputLayout(nullptr);
    deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext_->VSSetShader(vertexShader_.Get(), nullptr, 0);
    deviceContext_->PSSetShader(pixelShader_.Get(), nullptr, 0);
    deviceContext_->Draw(3, 0);
}

void Renderer::EndFrame() const
{
    Present();
}

void Renderer::Clear(float red, float green, float blue, float alpha) const
{
    const float clearColor[] = {red, green, blue, alpha};
    deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), nullptr);
    deviceContext_->ClearRenderTargetView(renderTargetView_.Get(), clearColor);
}

void Renderer::Present() const
{
    const UINT syncInterval = isVSyncEnabled_ ? 1U : 0U;
    ThrowIfFailed(swapChain_->Present(syncInterval, 0), "Failed to present the frame.");
}

void Renderer::SetVSyncEnabled(bool isEnabled)
{
    isVSyncEnabled_ = isEnabled;
}

bool Renderer::IsVSyncEnabled() const
{
    return isVSyncEnabled_;
}

ID3D11Device* Renderer::GetDevice() const
{
    return device_.Get();
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
    return deviceContext_.Get();
}

void Renderer::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    ThrowIfFailed(
        swapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())),
        "Failed to acquire the swap chain back buffer.");

    ThrowIfFailed(
        device_->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView_.GetAddressOf()),
        "Failed to create the render target view.");
}

void Renderer::SetViewport(UINT width, UINT height) const
{
    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    deviceContext_->RSSetViewports(1, &viewport);
}
