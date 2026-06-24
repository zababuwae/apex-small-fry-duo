#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

class Renderer final
{
public:
    void Initialize(HWND windowHandle, UINT width, UINT height);
    void Resize(UINT width, UINT height);
    void BeginFrame(float red, float green, float blue, float alpha) const;
    void EndFrame() const;
    void Clear(float red, float green, float blue, float alpha) const;
    void Present() const;
    void SetVSyncEnabled(bool isEnabled);
    bool IsVSyncEnabled() const;
    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;

private:
    void CreateRenderTarget();
    void SetViewport(UINT width, UINT height) const;

    Microsoft::WRL::ComPtr<ID3D11Device> device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;
    bool isVSyncEnabled_ = true;
};
