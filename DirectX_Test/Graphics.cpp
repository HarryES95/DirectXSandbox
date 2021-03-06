#include "Graphics.h"
#include <string>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics( HWND hWnd ) {

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;
	
	GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &sd,
		&pSwap, &pDevice, nullptr, &pContext
	) );

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_FAILED(pSwap->GetBuffer( 0, __uuidof( ID3D11Resource ), &pBackBuffer ));
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget ));
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if( FAILED( hr = pSwap->Present( 1u, 0u ) ) ) {
		if( hr == DXGI_ERROR_DEVICE_REMOVED ) {
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else {
			GFX_THROW_FAILED( hr );
		}
	}
}

void Graphics::DrawTriangle() {

	HRESULT hr;

	struct Vertex {
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{ 0.0f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = sizeof( vertices );
	bd.StructureByteStride = sizeof( Vertex );

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	GFX_THROW_FAILED( pDevice->CreateBuffer( &bd, &sd, &pVertexBuffer ));
	const UINT stride = sizeof( Vertex );
	const UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pContext->Draw( 3, 0 );
}

Graphics::Exception::Exception( int line, const char* file, HRESULT hr ) noexcept
	: hExcept( line, file ), hr( hr ) {}

const char* Graphics::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::GetType() const noexcept
{
	return "Graphics Exception";
}

std::string Graphics::Exception::TranslateErrorCode( HRESULT hr ) noexcept {
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>( &pMsgBuf ), 0, nullptr );
	if( nMsgLen == 0 ) {
		return "Unidentified Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree( pMsgBuf );
	return errorString;
}

HRESULT Graphics::Exception::GetErrorCode() const noexcept {
	return hr;
}

std::string Graphics::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode( hr );
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}