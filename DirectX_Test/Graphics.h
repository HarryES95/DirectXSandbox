#pragma once
#include "hWin.h"
#include <d3d11.h>
#include "hExcept.h"
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Graphics
{
public:
	class Exception : public hExcept {
	public:
		Exception( int line, const char* file, HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public Exception {
		using Exception::Exception;
	public:
		const char* GetType() const noexcept override;
	};
public:
	Graphics(HWND hWnd);
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer( float red, float green, float blue ) noexcept {
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView( pTarget.Get(), color );
	}
	void DrawTriangle();
private:
	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwap;
	wrl::ComPtr<ID3D11DeviceContext> pContext;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget;
};

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::Exception(__LINE__,__FILE__,hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))