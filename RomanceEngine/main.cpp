#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <RomanceEngine/Math/vector_3d.h>
#include <RomanceEngine/Math/matrix_4x4.h>
#include <RomanceEngine/Math/constant.h>
#include <RomanceEngine/Memory/shared_ptr.h>
#include <RomanceEngine/Render/vertex_shader.h>

using namespace std;
using namespace RomanceEngine::Math;
using namespace RomanceEngine::Memory;
using namespace RomanceEngine::Render;

// warning C4996: 'freopen': This function or variable may be unsafe...
#pragma warning (disable : 4996)

#define TIMER_ID     (100)      // 作成するタイマの識別ID.
#define TIMER_ELAPSE (1000/60)     // WM_TIMERの発生間隔.

class Test
{
public:
  Test()
  {
    cout << "Test Con" << endl;
  }
  void test(const char* s) { cout << s << endl; }
  ~Test()
  {
    cout << "Test Des" << endl;
  }
};

void sandbox()
{
	typedef RomanceEngine::Math::Vector3D V;
	typedef RomanceEngine::Math::Matrix4x4 Mat4;
  const float kPI = RomanceEngine::Math::kRM_PI;

	{
		V a(1, 2, 3);
		V b(0, 0, 1);
		cout << (a+b).asString() << endl;
		cout << (a-b).asString() << endl;
		cout << (a*4).asString() << endl;
		cout << (3*a).asString() << endl;
		cout << a.normal().abs2() << endl;
		cout << a.cross(b).asString() << endl;
		cout << a.calcSin(b) << endl;
		cout << a.calcCos(b) << endl;
		cout << a.projection(b).asString() << endl;
		cout << RomanceEngine::Math::kRM_PI2 << endl;
		cout << RomanceEngine::Math::kRM_PI_HALF << endl;
	}

	cout << "-------" << endl;

	{
		Mat4 a(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    V x(1,2,3);
		Mat4 b(1,-5, 3, 0,
			     0,-2, 6, 0,
			     7, 2,-4, 0,
			     0, 0, 0, 1);
		Mat4 c(-8, 6, 1, 0,
			     7, 0, -3, 0,
			     2, 4, 5, 0,
			     0, 0, 0, 1);
    V y(1, 0, 0);
    cout << b.determinant() << endl;
    cout << b.inverse().asString() << endl;
    cout << b.multiply(b.inverse()).asString() << endl;
    cout << Mat4::buildScale(2,3,4).multiply(x).asString() << endl;

		cout << a.asString() << endl;
		cout << a.transpose().asString() << endl;
		cout << (a*2).asString() << endl;
		cout << (a/2).asString() << endl;
		cout << (b.multiply(c)).asString() << endl;
    cout << b.multiply(x).asString() << endl;
		cout << a.isDiagonal() << " " << b.isDiagonal() << endl;

    Mat4 d(3,-2, 0, 0,
           1, 4,-3, 0,
           -1,0, 2, 0,
           0, 0, 0, 1);
    cout << d.determinant() << endl;

    cout << Mat4::buildTranslate(2,3,4).multiply(y).asString() << endl;
	}
	
}

#define USE_GL 1
#define USE_DX 0

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

#if USE_DX
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       g_pd3dDevice = NULL;
 ID3D11DeviceContext*                g_pImmediateContext = NULL;
IDXGISwapChain*                     g_pSwapChain = NULL;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor( 0.7f, 0.7f, 0.7f, 1.0f );

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}



//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
	GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"none.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"none.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
    };

    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Create index buffer
    // Create vertex buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
    g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create the constant buffers
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBNeverChanges );
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );
    if( FAILED( hr ) )
        return hr;

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile( g_pd3dDevice, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    g_View = XMMatrixLookAtLH( Eye, At, Up );

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose( g_View );
    g_pImmediateContext->UpdateSubresource( g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0 );

    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f );
    
    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
    g_pImmediateContext->UpdateSubresource( g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0 );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pSamplerLinear ) g_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_pCBNeverChanges ) g_pCBNeverChanges->Release();
    if( g_pCBChangeOnResize ) g_pCBChangeOnResize->Release();
    if( g_pCBChangesEveryFrame ) g_pCBChangesEveryFrame->Release();
    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Update our time
    static float t = 0.0f;
    if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

    // Rotate cube around the origin
    g_World = XMMatrixRotationY( t );

    // Modify the color
    g_vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    //
    // Update variables that change once per frame
    //
    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose( g_World );
    cb.vMeshColor = g_vMeshColor;
    g_pImmediateContext->UpdateSubresource( g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );

    //
    // Render the cube
    //
    g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
    g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
    g_pImmediateContext->VSSetConstantBuffers( 1, 1, &g_pCBChangeOnResize );
    g_pImmediateContext->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
    g_pImmediateContext->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
    g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );
    g_pImmediateContext->DrawIndexed( 36, 0, 0 );

    //
    // Present our back buffer to our front buffer
    //
    g_pSwapChain->Present( 0, 0 );
}

#endif

#if USE_GL

#include <cstdio>
#include <cmath>
#include <cassert>
#include <map>
#include <gl/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <GL/glut.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "cg.lib" )
#pragma comment( lib, "cgGL.lib" )

HGLRC glrc;
static CGcontext   myCgContext;
static CGprofile   myCgVertexProfile,
                   myCgFragmentProfile;
static CGprogram   myCgVertexProgram,
                   myCgFragmentProgram;
static CGparameter myCgVertexParam_modelViewProj,
                   myCgFragmentParam_globalAmbient,
                   myCgFragmentParam_lightColor,
                   myCgFragmentParam_lightPosition,
                   myCgFragmentParam_eyePosition,
                   myCgFragmentParam_Ke,
                   myCgFragmentParam_Ka,
                   myCgFragmentParam_Kd,
                   myCgFragmentParam_Ks,
                   myCgFragmentParam_shininess;

bool checkCgError(const CGcontext context, const std::string& situation)
{
  CGerror error;
  const char *str = cgGetLastErrorString(&error);
  
  if (error != CG_NO_ERROR) {
    printf("Error: %s: %s\n", situation.c_str(), str);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(context));
    }
    return false;
  }
  return true;
}

class FragmentShader
{
public:
  FragmentShader()
    : hasLoaded_(false)
  {
  }

  bool load(const CGcontext context, const std::string& fileName, const std::string& programName)
  {
    assert(hasLoaded_ == false);
    context_ = context;
    fileName_ = fileName;
    programName_ = programName;

    profile_ = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(profile_);
    if (checkCgError(context, "selecting fragment profile") == false) return false;
    
    program_ =
      cgCreateProgramFromFile(
      context_,                /* Cg runtime context */
      CG_SOURCE,               /* Program in human-readable form */
      fileName_.c_str(),       /* Name of file containing program */
      profile_,                /* Profile: OpenGL ARB vertex program */
      programName_.c_str(),    /* Entry function name */
      NULL);                   /* No extra compiler options */
    if (checkCgError(context_, "creating fragment program from file : "+ fileName_ + ">" + programName_) == false) return false;

    cgGLLoadProgram(program_);
    if (checkCgError(context_, "loading fragment program") == false) return false;
  
    hasLoaded_ = true;

    return true;
  }

  void registParameter(const std::string& name)
  {
    parameters_[name] = cgGetNamedParameter(program_, name.c_str());
    checkCgError(context_, "could not get " + name + " parameter");
  }

  void bind()
  {
    cgGLBindProgram(program_);
    checkCgError(context_, "binding fragment program");

    cgGLEnableProfile(profile_);
    checkCgError(context_, "enabling fragment profile");
  }

  void unbind()
  {
    cgGLDisableProfile(profile_);
    checkCgError(context_, "disabling fragment profile");
  }

  void setParameterMatrix(const std::string& name, const Matrix4x4& value)
  {
    std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
    if (it == parameters_.end())
    {
      printf("Error: parameter not registed > %s\n", name.c_str());
      assert(false);
    }
    cgSetMatrixParameterfr(parameters_[name], value.p_);
    checkCgError(context_, "setting " + name + " parameter");
  }

  void setParameterFloat3(const std::string& name, const Vector3D& value)
  {
    setParameterFloat3(name, value.p_);
  }

  void setParameterFloat3(const std::string& name, const float* value/*[3]*/)
  {
    std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
    if (it == parameters_.end())
    {
      printf("Error: parameter not registed > %s\n", name.c_str());
      assert(false);
    }
    cgSetParameter3fv(parameters_[name], value);
    checkCgError(context_, "setting " + name + " parameter");
  }

  void setParameterFloat3(const std::string& name, const float value0, const float value1, const float value2)
  {
    std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
    if (it == parameters_.end())
    {
      printf("Error: parameter not registed > %s\n", name.c_str());
      assert(false);
    }
    cgSetParameter3f(parameters_[name], value0, value1, value2);
    checkCgError(context_, "setting " + name + " parameter");
  }

  void setParameterFloat1(const std::string& name, const float value)
  {
    std::map<std::string, CGparameter>::iterator it = parameters_.find(name);
    if (it == parameters_.end())
    {
      printf("Error: parameter not registed > %s\n", name.c_str());
      assert(false);
    }
    cgSetParameter1f(parameters_[name], value);
    checkCgError(context_, "setting " + name + " parameter");
  }

  void update()
  {
    cgUpdateProgramParameters(program_);
  }

private:
  bool checkError()
  {
    CGerror error;
    cgGetLastErrorString(&error);
    return (error == CG_NO_ERROR);
  }  

private:
  bool hasLoaded_;
  std::string programName_;
  std::string fileName_;
  CGcontext context_;
  CGprofile profile_;
  CGprogram program_;
  std::map<std::string, CGparameter> parameters_;
};
typedef SharedPtr<FragmentShader> FragmentShaderPtr;

class ShaderManager
{
public:
  ShaderManager()
    : hasInit_(false)
  {}

  void init()
  {
    context_ = cgCreateContext();
    checkCgError(context_, "creating context");

    cgGLSetDebugMode(CG_FALSE);
    cgSetParameterSettingMode(context_, CG_DEFERRED_PARAMETER_SETTING);

    hasInit_ = true;
  }

  VertexShaderPtr createVertexShader(const std::string& fileName, const std::string& programName)
  {
    assert(hasInit_);
    VertexShaderPtr res(new VertexShader);
    if (res->load(context_, fileName, programName) == false)
    {
      checkCgError(context_, "load: " + fileName + " > " + programName);
      return VertexShaderPtr();
    }
    return res;
  }

  FragmentShaderPtr createFragmentShader(const std::string& fileName, const std::string& programName)
  {
    assert(hasInit_);
    FragmentShaderPtr res(new FragmentShader);
    if (res->load(context_, fileName, programName) == false)
    {
      checkCgError(context_, "load: " + fileName + " > " + programName);
      return FragmentShaderPtr();
    }
    return res;
  }

private:
  CGcontext context_;
  bool hasInit_;
};

ShaderManager shaderManager_;
VertexShaderPtr vertexShader_;
FragmentShaderPtr fragmentShader_;

static const char *myProgramName = "10_fragment_lighting",
                  *myVertexProgramFileName = "vs.cg",
/* Page 124 */    *myVertexProgramName = "C5E2v_fragmentLighting",
                  *myFragmentProgramFileName = "fs.cg",
/* Page 125 */    *myFragmentProgramName = "C5E3f_basicLight";

static float myLightAngle = -0.4;   /* Angle light rotates around scene. */
static Matrix4x4 myProjectionMatrix;
static float myGlobalAmbient[3] = { 0.1, 0.1, 0.1 };  /* Dim */
static float myLightColor[3] = { 0.95, 0.95, 0.95 }; 

static void reshape(int width, int height);

static void checkForCgError(const char *situation)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s: %s\n",
      myProgramName, situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(myCgContext));
    }
    exit(1);
  }
}

bool initGL(HWND hwnd)
{
    HDC dc = GetDC( hwnd );
    try
    {
        //
        //ピクセルフォーマットの設定
        //
        PIXELFORMATDESCRIPTOR pfd =
        { 
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
            1,                     // version number 
            PFD_DRAW_TO_WINDOW |   // support window 
            PFD_SUPPORT_OPENGL |   // support OpenGL 
            PFD_DOUBLEBUFFER,      // double buffered 
            PFD_TYPE_RGBA,         // RGBA type 
            24,                    // 24-bit color depth 
            0, 0, 0, 0, 0, 0,      // color bits ignored 
            0,                     // no alpha buffer 
            0,                     // shift bit ignored 
            0,                     // no accumulation buffer 
            0, 0, 0, 0,            // accum bits ignored 
            32,                    // 32-bit z-buffer 
            0,                     // no stencil buffer 
            0,                     // no auxiliary buffer 
            PFD_MAIN_PLANE,        // main layer 
            0,                     // reserved 
            0, 0, 0                // layer masks ignored 
        };
        int format = ChoosePixelFormat( dc, &pfd );
        if( format == 0 )
            throw "";

        if( !SetPixelFormat( dc, format, &pfd ) )
            throw "";

        //
        //レンダリングコンテキスト作成
        //
        glrc = wglCreateContext( dc );
        if( !glrc )
            throw "";
    }
    catch( ... )
    {
        ReleaseDC( hwnd, dc );
        return false;
    }
	
    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    wglMakeCurrent( dc, glrc );

    glClearColor(0.1, 0.1, 0.1, 0);  /* Gray background. */
    glEnable(GL_DEPTH_TEST);         /* Hidden surface removal. */

    shaderManager_.init();

    vertexShader_ = shaderManager_.createVertexShader(myVertexProgramFileName, myVertexProgramName);
    vertexShader_->registParameter("modelViewProj");

    fragmentShader_ = shaderManager_.createFragmentShader(myFragmentProgramFileName, myFragmentProgramName);
    fragmentShader_->registParameter("globalAmbient");
    fragmentShader_->registParameter("lightColor");
    fragmentShader_->registParameter("lightPosition");
    fragmentShader_->registParameter("eyePosition");
    fragmentShader_->registParameter("Ke");
    fragmentShader_->registParameter("Ka");
    fragmentShader_->registParameter("Kd");
    fragmentShader_->registParameter("Ks");
    fragmentShader_->registParameter("shininess");

    fragmentShader_->setParameterFloat3("globalAmbient", myGlobalAmbient);
    fragmentShader_->setParameterFloat3("lightColor", myLightColor);
    

  reshape(width, height);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  wglMakeCurrent( dc, 0 );
  
  ReleaseDC( hwnd, dc );
  SendMessage( hwnd, WM_PAINT, 0, 0 );
  return true;
}

/* Forward declared routine used by reshape callback. */
static void buildPerspectiveMatrix(double fieldOfView,
                                   double aspectRatio,
                                   double zMin, double zMax,
                                   float m[16]);

static void reshape(int width, int height)
{
  double aspectRatio = (float) width / (float) height;
  double fieldOfView = degreeToRadian(40.0); /* Radian */

  /* Build projection matrix once. */
  
  myProjectionMatrix = Matrix4x4::buildPerspective(
    fieldOfView, aspectRatio,
    1.0, 20.0  /* Znear and Zfar */);
  
  //myProjectionMatrix = Matrix4x4::buildOrth(0, width, 0, height, 1.0, 20.0);
  
  glViewport(0, 0, width, height);
}

static const double myPi = 3.14159265358979323846;


static void setBrassMaterial(void)
{
  const float brassEmissive[3] = {0.0,  0.0,  0.0},
              brassAmbient[3]  = {0.33, 0.22, 0.03},
              brassDiffuse[3]  = {0.78, 0.57, 0.11},
              brassSpecular[3] = {0.99, 0.91, 0.81},
              brassShininess = 27.8;

  fragmentShader_->setParameterFloat3("Ke", brassEmissive);
  fragmentShader_->setParameterFloat3("Ka", brassAmbient);
  fragmentShader_->setParameterFloat3("Kd", brassDiffuse);
  fragmentShader_->setParameterFloat3("Ks", brassSpecular);
  fragmentShader_->setParameterFloat1("shininess", brassShininess);
}
static void setRedPlasticMaterial(void)
{
  const float redPlasticEmissive[3] = {0.0,  0.0,  0.0},
              redPlasticAmbient[3]  = {0.0, 0.0, 0.0},
              redPlasticDiffuse[3]  = {0.5, 0.0, 0.0},
              redPlasticSpecular[3] = {0.7, 0.6, 0.6},
              redPlasticShininess = 32.0;
  
  fragmentShader_->setParameterFloat3("Ke", redPlasticEmissive);
  fragmentShader_->setParameterFloat3("Ka", redPlasticAmbient);
  fragmentShader_->setParameterFloat3("Kd", redPlasticDiffuse);
  fragmentShader_->setParameterFloat3("Ks", redPlasticSpecular);
  fragmentShader_->setParameterFloat1("shininess", redPlasticShininess);
}

static void setEmissiveLightColorOnly(void)
{
  const float zero[3] = {0.0,  0.0,  0.0};

  fragmentShader_->setParameterFloat3("Ke", myLightColor);
  fragmentShader_->setParameterFloat3("Ka", zero);
  fragmentShader_->setParameterFloat3("Kd", zero);
  fragmentShader_->setParameterFloat3("Ks", zero);
  fragmentShader_->setParameterFloat1("shininess", 0);
}

void ReleaseGL()
{
    //
    //レンダリングコンテキスト破棄
    //
    wglDeleteContext( glrc );
}

void renderCube(const float size)
{
	float s = size / 2;
	float v[8][4] = {
		{ -s, +s, -s, 1 },
		{ -s, +s, +s, 1 },
		{ +s, +s, +s, 1 },
		{ +s, +s, -s, 1 },
		{ -s, -s, -s, 1 },
		{ -s, -s, +s, 1 },
		{ +s, -s, +s, 1 },
		{ +s, -s, -s, 1 },
	};
	
  // 0 2.
  // 1 3.
  //
  // 4 7.
  // 5 6.

  float n[6][3] = {
    0, 1, 0,
    0,-1, 0,
    0, 0, 1,
    0, 0,-1,
    1, 0, 0,
   -1, 0, 0,
  };

	glBegin(GL_TRIANGLES);
  glNormal3fv(n[0]);
	glVertex4fv(v[0]); glVertex4fv(v[1]); glVertex4fv(v[3]);
	glVertex4fv(v[1]); glVertex4fv(v[2]); glVertex4fv(v[3]);
	
  glNormal3fv(n[1]);
	glVertex4fv(v[7]); glVertex4fv(v[5]); glVertex4fv(v[4]);
	glVertex4fv(v[7]); glVertex4fv(v[6]); glVertex4fv(v[5]);
	
  glNormal3fv(n[2]);
	glVertex4fv(v[1]); glVertex4fv(v[5]); glVertex4fv(v[6]);
	glVertex4fv(v[5]); glVertex4fv(v[6]); glVertex4fv(v[7]);
  
  glNormal3fv(n[3]);
	glVertex4fv(v[0]); glVertex4fv(v[3]); glVertex4fv(v[4]);
	glVertex4fv(v[3]); glVertex4fv(v[4]); glVertex4fv(v[7]);
	
  glNormal3fv(n[4]);
	glVertex4fv(v[0]); glVertex4fv(v[4]); glVertex4fv(v[1]);
	glVertex4fv(v[4]); glVertex4fv(v[1]); glVertex4fv(v[5]);
  
  glNormal3fv(n[5]);
	glVertex4fv(v[3]); glVertex4fv(v[7]); glVertex4fv(v[2]);
	glVertex4fv(v[7]); glVertex4fv(v[2]); glVertex4fv(v[6]);


	glEnd();
}

static void drawRect()
{
  glDepthMask(GL_FALSE);

  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(100, 100, 2);
  glVertex3f(100, 200, 2);
  glVertex3f(200, 100, 2);
  glVertex3f(200, 200, 2);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(150, 150, 2);
  glVertex3f(150, 250, 2);
  glVertex3f(250, 150, 2);
  glVertex3f(250, 250, 2);
  glEnd();
}


void RenderGL2( HDC dc )
{

  /* World-space positions for light and eye. */

  Vector3D eyePosition(5, 5, 13);
  Vector3D lightPosition(5*sin(myLightAngle), 
                         1.5,
                         5*cos(myLightAngle));

  Matrix4x4 viewMatrix = Matrix4x4::buildLookAt(eyePosition, Vector3D(0, 0, 0), Vector3D(0, 1, 0));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vertexShader_->bind();
  fragmentShader_->bind();

  /*** Render brass solid sphere ***/

  setBrassMaterial();

  /* modelView = rotateMatrix * translateMatrix */
  Matrix4x4 rotateMatrix = Matrix4x4::buildRotateByVector(Vector3D(1, 1, 1), degreeToRadian(70));
  Matrix4x4 translateMatrix = Matrix4x4::buildTranslate(2, 0, 0);
  Matrix4x4 modelMatrix = translateMatrix.multiply(rotateMatrix);

  /* invModelMatrix = inverse(modelMatrix) */
  Matrix4x4 invModelMatrix = modelMatrix.inverse();

  /* Transform world-space eye and light positions to sphere's object-space. */
  Vector3D objSpaceEyePosition = invModelMatrix.multiply(eyePosition);
  Vector3D objSpaceLightPosition = invModelMatrix.multiply(lightPosition);
  fragmentShader_->setParameterFloat3("eyePosition", objSpaceEyePosition);
  fragmentShader_->setParameterFloat3("lightPosition", objSpaceLightPosition);

  /* modelViewMatrix = viewMatrix * modelMatrix */
  Matrix4x4 modelViewMatrix = viewMatrix.multiply(modelMatrix);

  /* modelViewProj = projectionMatrix * modelViewMatrix */
  Matrix4x4 modelViewProjMatrix = myProjectionMatrix.multiply(modelViewMatrix);

  /* Set matrix parameter with row-major matrix. */
  vertexShader_->setMatrixParameter("modelViewProj", modelViewProjMatrix);
 
  vertexShader_->update();
  fragmentShader_->update();

  glutSolidSphere(2.0, 40, 40);

  /*** Render red plastic solid cone ***/

  setRedPlasticMaterial();

  /* modelView = viewMatrix * translateMatrix */
  translateMatrix = Matrix4x4::buildTranslate(-2, -1.5, 0);
  rotateMatrix = Matrix4x4::buildRotateByVector(Vector3D(1, 0, 0), degreeToRadian(90));
  modelMatrix = translateMatrix.multiply(rotateMatrix);

  /* invModelMatrix = inverse(modelMatrix) */
  invModelMatrix = modelMatrix.inverse();

  /* Transform world-space eye and light positions to sphere's object-space. */
  objSpaceEyePosition = invModelMatrix.multiply(eyePosition);
  objSpaceLightPosition = invModelMatrix.multiply(lightPosition);

  /* modelViewMatrix = viewMatrix * modelMatrix */
  modelViewMatrix = viewMatrix.multiply(modelMatrix);

  /* modelViewProj = projectionMatrix * modelViewMatrix */
  modelViewProjMatrix = myProjectionMatrix.multiply(modelViewMatrix);

  /* Set matrix parameter with row-major matrix. */
  vertexShader_->setMatrixParameter("modelViewProj", modelViewProjMatrix);

  vertexShader_->update();
  fragmentShader_->update();

  glutSolidCone(1.5, 3.5, 30, 30);
  //renderCube(2);

  /*** Render light as emissive white ball ***/

  /* modelView = translateMatrix */
  modelMatrix = Matrix4x4::buildTranslate(lightPosition[0], lightPosition[1], lightPosition[2]);

  /* modelViewMatrix = viewMatrix * modelMatrix */
  modelViewMatrix = viewMatrix.multiply(modelMatrix);

  /* modelViewProj = projectionMatrix * modelViewMatrix */
  modelViewProjMatrix = myProjectionMatrix.multiply(modelViewMatrix);

  setEmissiveLightColorOnly();
  /* Avoid degenerate lightPosition. */
  fragmentShader_->setParameterFloat3("lightPosition", 0, 0, 0);

  /* Set matrix parameter with row-major matrix. */
  vertexShader_->setMatrixParameter("modelViewProj", modelViewProjMatrix);
  
  vertexShader_->update();
  fragmentShader_->update();

  glutSolidSphere(0.2, 12, 12);
  
  vertexShader_->unbind();
  fragmentShader_->unbind();

  SwapBuffers( dc );
}

void RenderGL( HDC dc )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vertexShader_->bind();
  fragmentShader_->bind();

  vertexShader_->setMatrixParameter("modelViewProj", myProjectionMatrix);
  
  vertexShader_->update();
  fragmentShader_->update();

  drawRect();
  
  vertexShader_->unbind();
  fragmentShader_->unbind();

  SwapBuffers( dc );
}
#endif


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case WM_CLOSE:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
#if USE_GL
			// TODO: 描画コードをここに追加してください...
			wglMakeCurrent( hdc, glrc );
			RenderGL2( hdc );
			wglMakeCurrent( hdc, 0 );
#endif
			EndPaint(hWnd, &ps);
		}
  case WM_TIMER:
    if( wParam != TIMER_ID )
    {
      break;
    }
    myLightAngle += 0.008;  /* Add a small angle (in radians). */
    if (myLightAngle > 2*myPi) {
      myLightAngle -= 2*myPi;
    }
		InvalidateRect( hWnd, NULL, FALSE );

		return 0;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout); //標準出力をコンソールにする
	freopen("CONIN$", "r", stdin);   //標準入力をコンソールにする

	sandbox();

	//ウィンドウクラスを登録して
	TCHAR szWindowClass[] = TEXT("RomanceEngine");
	WNDCLASS wcex;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	RegisterClass(&wcex);

	//ウィンドウをクリエイト
	HWND hWnd;
	g_hWnd = hWnd = CreateWindow(szWindowClass,
        TEXT("RomanceEngine"),
        WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        0,
        0,
        800,
        600,
        NULL,
        NULL,
        hInstance,
        NULL);

#if USE_GL
	initGL(hWnd);
#endif

	ShowWindow(hWnd, nCmdShow);


#if USE_DX
    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }
#endif

	SetTimer( hWnd, TIMER_ID, TIMER_ELAPSE, NULL );

	//メインループ
	MSG hMsg;
	while(true){
		if (PeekMessageW(&hMsg, NULL, 0, 0, PM_REMOVE))
		{
			if(hMsg.message == WM_QUIT){
				goto End;
			}
			TranslateMessage(&hMsg);
			DispatchMessage(&hMsg);
		}
		else
		{
#if USE_DX
            Render();
#endif
		}
	}

End:
#if USE_DX
    CleanupDevice();
#endif
	FreeConsole();
	return 0;
}


