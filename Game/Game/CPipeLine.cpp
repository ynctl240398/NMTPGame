#include "CPipeLine.h"
#include "CUtil.h"
#include "CGame.h"

CPipeLine* CPipeLine::__instance = NULL;

CPipeLine::CPipeLine() {}

CPipeLine::~CPipeLine() {}

CPipeLine* CPipeLine::GetInstance() {
	if (__instance == NULL) {
		__instance = new CPipeLine;
	}
	return __instance;
}

ID3D10EffectTechnique* CPipeLine::GetEffectTechnique() const {
	return _effectTechnique;
}

ID3D10EffectShaderResourceVariable* CPipeLine::GetEffectSRV() const {
	return _effectSRV;
}

IDXGISwapChain* CPipeLine::GetSwapChain() const {
	return _swapChain;
}

ID3D10InputLayout* CPipeLine::GetInputLayout() const {
	return _inputLayout;
}

ID3D10RenderTargetView* CPipeLine::GetRenderTargetView() const {
	return _renderTargetView;
}

ID3D10RasterizerState* CPipeLine::GetRasterizerState() const {
	return _rasterizerState;
}

ID3D10BlendState* CPipeLine::GetBlendState() const {
	return _blendState;
}

bool CPipeLine::LoadEffect(std::string filePath) {
	HRESULT hResult = D3DX10CreateEffectFromFileA(
		filePath.c_str(),
		nullptr,
		nullptr,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		CUtil::DirectDevice,
		nullptr,
		nullptr,
		&_effect,
		nullptr,
		nullptr
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create effect\n");
		return false;
	}

	_effectTechnique = _effect->GetTechniqueByName("main");
	if (_effectTechnique == nullptr) {
		OutputDebugStringA("[CPipeLine] Failed to get technique by name\n");
		return false;
	}

	_effectSRV = _effect->GetVariableByName("tex2D")->AsShaderResource();

	return true;
}

bool CPipeLine::CreateDeviceAndSwapChain(HWND hWND, unsigned int backBufferWidth, unsigned int backBufferHeight, unsigned int frameRate) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = frameRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWND;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	HRESULT hResult = D3D10CreateDeviceAndSwapChain(
		nullptr,
		D3D10_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&CUtil::DirectDevice
	);

	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create device and swap chain\n");
		return false;
	}

	return true;
}

bool CPipeLine::CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* layout, unsigned int elements) {
	D3D10_PASS_DESC passDesc;
	_effectTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hResult = CUtil::DirectDevice->CreateInputLayout(
		layout,
		elements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&_inputLayout
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create input layout\n");
		return false;
	}
	CUtil::DirectDevice->IASetInputLayout(_inputLayout);

	return true;
}

bool CPipeLine::CreateRenderTagetView() {
	//Gets the back buffer from the swap chain
	ID3D10Texture2D* backBuffer;
	HRESULT hResult = _swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to get back buffer from the swap chain\n");
		return false;
	}

	//Creates the render target view
	hResult = CUtil::DirectDevice->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
	backBuffer->Release();
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create render target view\n");
		return false;
	}
	//Sets the render target;
	CUtil::DirectDevice->OMSetRenderTargets(1, &_renderTargetView, nullptr);

	return true;
}

bool CPipeLine::CreateViewport(unsigned int backBufferWidth, unsigned int backBufferHeight) {
	//Creates and sets the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	CUtil::DirectDevice->RSSetViewports(1, &viewPort);

	//Creates the sprite handler;
	HRESULT hResult = D3DX10CreateSprite(CUtil::DirectDevice, 0, &CUtil::SpriteHandler);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create sprite handler\n");
		return false;
	}

	//Creates the projection matrix using the values in the viewport
	D3DXMATRIX projectionMatrix;
	D3DXMatrixOrthoOffCenterLH(
		&projectionMatrix,
		static_cast<float>(viewPort.TopLeftX),
		static_cast<float>(viewPort.Width),
		static_cast<float>(viewPort.TopLeftY),
		static_cast<float>(viewPort.Height),
		0.1f,
		10.0f
	);
	hResult = CUtil::SpriteHandler->SetProjectionTransform(&projectionMatrix);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create the projection matrix\n");
		return false;
	}

	return true;
}

bool CPipeLine::CreateRasterizerState() {
	D3D10_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D10_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D10_FILL_SOLID;
	rasterizerDesc.CullMode = D3D10_CULL_NONE;
	rasterizerDesc.DepthClipEnable = true;

	HRESULT hResult = CUtil::DirectDevice->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create rasterizer state\n");
		return false;
	}
	CUtil::DirectDevice->RSSetState(_rasterizerState);

	return true;
}

bool CPipeLine::CreateBlendState() {
	//Initializes the blend state for alpha drawing
	D3D10_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HRESULT hResult = CUtil::DirectDevice->CreateBlendState(&blendDesc, &_blendState);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create blend state\n");
		return false;
	}

	return true;
}

bool CPipeLine::PipeCreateFont(int fontHeight) {
	HRESULT hResult = D3DX10CreateFontA(
		CUtil::DirectDevice,
		fontHeight,
		0,
		FW_DONTCARE,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Time News Roman",
		&CUtil::Font
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[CPipeLine] Failed to create font\n");
		return false;
	}

	return true;
}

void CPipeLine::Release() {
	if (CUtil::Font != NULL) {
		CUtil::Font->Release();
	}

	if (_blendState != nullptr) {
		_blendState->Release();
	}

	if (_rasterizerState != NULL) {
		_rasterizerState->Release();
	}

	if (CUtil::SpriteHandler != NULL) {
		CUtil::SpriteHandler->Release();
	}

	if (_renderTargetView != nullptr) {
		_renderTargetView->Release();
	}

	if (_inputLayout != nullptr) {
		_inputLayout->Release();
	}

	if (CUtil::DirectDevice != NULL) {
		CUtil::DirectDevice->Release();
	}

	if (_swapChain != nullptr) {
		_swapChain->Release();
	}

	if (_effectTechnique != nullptr) {
		delete _effectSRV;
		delete _effectTechnique;
		_effect->Release();
	}

	if (__instance != nullptr) {
		delete __instance;
		__instance = nullptr;
	}
}