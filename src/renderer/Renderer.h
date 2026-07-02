#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#include <string>

class Renderer final
{
public:
    void Initialize(HWND windowHandle, UINT width, UINT height);
    void CreateTriangleShaders(const std::wstring& shaderPath);
    void Resize(UINT width, UINT height);
    void BeginFrame(float red, float green, float blue, float alpha) const;
    void DrawTriangle() const;
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
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;
    bool isVSyncEnabled_ = true;
};
