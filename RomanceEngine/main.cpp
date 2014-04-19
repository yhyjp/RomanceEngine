#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <GL/glew.h>

#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>

#include <RomanceEngine/Math/vector_3d.h>
#include <RomanceEngine/Math/matrix_4x4.h>
#include <RomanceEngine/Math/constant.h>
#include <RomanceEngine/Math/float1234.h>
#include <RomanceEngine/Memory/shared_ptr.h>
#include <RomanceEngine/Render/shader_manager.h>
#include <RomanceEngine/Render/primitive_renderer.h>
#include <RomanceEngine/Image/dds.h>
#include <RomanceEngine/Render/render_context.h>
#include <RomanceEngine/Math/rect.h>
#include <RomanceEngine/GUI/gui_object.h>
#include <RomanceEngine/GUI/button.h>
#include <RomanceEngine/GUI/value_bar.h>
#include <RomanceEngine/GUI/gui_manager.h>

using namespace std;
using namespace RomanceEngine::Math;
using namespace RomanceEngine::Memory;
using namespace RomanceEngine::Render;
using namespace RomanceEngine::Image;
using namespace RomanceEngine::GUI;

// warning C4996: 'freopen': This function or variable may be unsafe...
#pragma warning (disable : 4996)

#define TIMER_ID     (100)      // 作成するタイマの識別ID.
#define TIMER_ELAPSE (1000/60)     // WM_TIMERの発生間隔.

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

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

void testMath()
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

  cout << "-----------" << endl;

  {
    Float1 f1(1);
    Float2 f2(2,3);
    Float3 f3(4,5,6);
    Float4 f4(7,8,9,10);
    V v(f3);
    cout << v.asString() << endl;
  }
}

void sandbox()
{
  testMath();
}

#define USE_GL 1
#define USE_DX 0

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

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
#pragma comment( lib, "glew32.lib" )

HGLRC glrc;

GLuint	fbuffer_texture_name;
GLuint	renderbuffer_name;
GLuint	framebuffer_name;


// 別ファイルへの以降はもう少し待つ.
class GLRenderContext : public RenderContext
{
public:
  GLRenderContext()
  {
  }

  virtual ~GLRenderContext()
  {
  }

  virtual void init()
  {
    context_ = cgCreateContext();
    shaderManager_.reset(new ShaderManager());
    shaderManager_->init(context_);
  }

  virtual ShaderManagerPtr& getShaderManager() { return shaderManager_; }
  virtual const ShaderManagerPtr& getShaderManager() const { return shaderManager_; }

  virtual void renderBegin()
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  virtual void renderEnd()
  {
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  virtual void setVertexPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer)
  {
    glVertexPointer(size, type, stride, pointer);
  }

  virtual void setNormalPointer(const uint32_t type, const uint32_t stride, const void* pointer)
  {
    glNormalPointer(type, stride, pointer);
  }

  virtual void setColorPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer)
  {
    glColorPointer(size, type, stride, pointer);
  }

  virtual void setTexCoordPointer(const int32_t size, const uint32_t type, const uint32_t stride, const void* pointer)
  {
    glTexCoordPointer(size, type, stride, pointer);
  }

  virtual void drawElements(const uint32_t mode, const uint32_t count, const uint32_t type, const void* indices)
  {
    glDrawElements(mode, count, type, indices);
  }

private:
  CGcontext context_;
  ShaderManagerPtr shaderManager_;
};

RenderContextPtr rctx_;
VertexShaderPtr vs_tex_;
FragmentShaderPtr fs_tex_;
VertexShaderPtr vs_;
FragmentShaderPtr fs_;
VertexShaderPtr vs_light_;
FragmentShaderPtr fs_light_;

void clickA(const Float2& p)
{
  cout << "clickedA : (" << p.x_ << ", " << p.y_ << ")" << endl;
}

void clickB(const Float2& p)
{
  cout << "clickedB : (" << p.x_ << ", " << p.y_ << ")" << endl;
}

#if 0
// [WIP]
namespace {
  enum {
    kRM_SHADER_COLOR_SIMPLE=0,
    kRM_SHADER_TEX_SIMPLE,

    kRM_SHADER_N
  };
}

class EmbededShaderSet
{
public:
  void setup()
  {

  }

  VertexShaderPtr& getVertexShader(const uint32_t enumShader)
  {
    assert(0<=enumShader && enumShader<kRM_SHADER_N);
    return vertex_[enumShader];
  }

  FragmentShaderPtr& getFragmentShader(const uint32_t enumShader)
  {
    assert(0<=enumShader && enumShader<kRM_SHADER_N);
    return fragment_[enumShader];
  }

private:
  VertexShaderPtr vertex_[kRM_SHADER_N];
  FragmentShaderPtr fragment_[kRM_SHADER_N];
};
#endif 

DDSImage image_;
GUIManager guiManager_;

static float myLightAngle = -0.4;   /* Angle light rotates around scene. */
static Matrix4x4 myPerspectiveMatrix;
static Matrix4x4 myOrthoMatrix;
static float myGlobalAmbient[3] = { 0.1, 0.1, 0.1 };  /* Dim */
static float myLightColor[3] = { 0.95, 0.95, 0.95 }; 

static void reshape(int width, int height);

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
  glClearDepth(1);
  glEnable(GL_DEPTH_TEST);         /* Hidden surface removal. */
  
  rctx_ = RenderContextPtr(new GLRenderContext());
  rctx_->init();

  vs_ = rctx_->getShaderManager()->createVertexShader("shader/color_simple_v.cg", "color_simple_v_main");
  fs_ = rctx_->getShaderManager()->createFragmentShader("shader/color_simple_f.cg", "color_simple_f_main");
  vs_->registParameter("modelViewProj");

  vs_tex_ = rctx_->getShaderManager()->createVertexShader("shader/tex_simple_v.cg", "tex_simple_v_main");
  fs_tex_ = rctx_->getShaderManager()->createFragmentShader("shader/tex_simple_f.cg", "tex_simple_f_main");
  vs_tex_->registParameter("modelViewProj");
  fs_tex_->registParameter("decal");

  vs_light_ = rctx_->getShaderManager()->createVertexShader("shader/light_simple_v.cg", "light_simple_v_main");
  fs_light_ = rctx_->getShaderManager()->createFragmentShader("shader/light_simple_f.cg", "light_simple_f_main");
  vs_light_->registParameter("modelViewProj");
  fs_light_->registParameter("lightColor");
  fs_light_->registParameter("lightPosition");
  fs_light_->setParameterFloat3("lightColor", 1, 1, 1);

  reshape(width, height);

  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);

  glewExperimental=GL_TRUE;
  if ( glewInit() != GLEW_OK )
  {
    printf("Error : GLEWの初期化に失敗しました\n");
    return 0;
  }

  image_.Load("fish1.dds");
  fs_tex_->setParameterTexture("decal", image_.ID);


  // GUI Test.
  {
    Button* b1 = new Button();
    Button* b2 = new Button();
    ValueBar* vb1 = new ValueBar(0.3);
    b1->setRegion(Rect(400, 400, 50, 50));
    b2->setRegion(Rect(460, 400, 80, 80));
    vb1->setRegion(Rect(20, 500, 200, 20));
    b1->clickEvent_.connect(clickA);
    b2->clickEvent_.connect(clickB);

    guiManager_.add(GUIObjectPtr(b1));
    guiManager_.add(GUIObjectPtr(b2));
    guiManager_.add(GUIObjectPtr(vb1));
  }

  {
    const int bufferW = 800;
    const int bufferH = 600;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glGenTextures( 1, &fbuffer_texture_name );
    glBindTexture( GL_TEXTURE_2D, fbuffer_texture_name );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, bufferW, bufferH,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

    //init render buffer.
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glGenTextures( 1, &renderbuffer_name );
    glBindTexture( GL_TEXTURE_2D, renderbuffer_name );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, bufferW, bufferH,
                    0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0 );
    //glGenRenderbuffersEXT( 1, &renderbuffer_name );
    glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderbuffer_name );
    glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                              bufferW, bufferH );

    // init frame buffer;
    glGenFramebuffersEXT( 1, &framebuffer_name );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );
    
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D, fbuffer_texture_name, 0 );
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_TEXTURE_2D, renderbuffer_name, 0 );
    
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    
    fs_tex_->setParameterTexture("decal", fbuffer_texture_name);
  }

  wglMakeCurrent( dc, 0 );

  ReleaseDC( hwnd, dc );
  SendMessage( hwnd, WM_PAINT, 0, 0 );
  return true;
}

void renderToTexture()
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );
  
  glClearColor( 0.2, 0.3, 0.6, 1.0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
  rctx_->renderBegin();
  
  // vs_tex update.
  vs_tex_->setMatrixParameter("modelViewProj", myOrthoMatrix);
  vs_tex_->update();

  // fs_tex update
  fs_tex_->setParameterTexture("decal", image_.ID);
  fs_tex_->update();

  // vs_update
  vs_->setMatrixParameter("modelViewProj", myOrthoMatrix);
  vs_->update();

  PrimitiveRenderer pr(rctx_);

  {
    vs_tex_->bind();
    fs_tex_->bind();

    pr.drawRect(Float2(10, 10), Float2(100, 100), Float4(1, 1, 1, 1), true);

    vs_tex_->bind();
    fs_tex_->unbind();
  }

  // color shader.
  {
    vs_->bind();
    fs_->bind();

    glLineWidth(3);
    pr.drawRect(Rect(300, 100, 100, 100), Float4(0, 1, 0, 0.4));
    pr.drawRect(Float2(350, 150), Float2(100, 100), Float4(0, 1, 0, 0.4));
    pr.drawLine(Float2(100, 300), Float2(300, 350), Float4(1, 0, 0, 1));

    vs_->unbind();
    fs_->unbind();
  }

  rctx_->renderEnd();

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}

static void reshape(int width, int height)
{
  double aspectRatio = (float) width / (float) height;
  double fieldOfView = degreeToRadian(40.0); /* Radian */

  /* Build projection matrix once. */
  myPerspectiveMatrix = Matrix4x4::buildPerspective(fieldOfView, aspectRatio, 1.0, 20.0  /* Znear and Zfar */);  
  myOrthoMatrix = Matrix4x4::buildOrth(0, width, 0, height, 0.0, 50.0);
  
  glViewport(0, 0, width, height);
} 

void ReleaseGL()
{
    //
    //レンダリングコンテキスト破棄.
    //
    wglDeleteContext( glrc );
}

void RenderGL3(HDC dc)
{
  /* World-space positions for light and eye. */
  
  PrimitiveRenderer pr(rctx_);
  rctx_->renderBegin();
  
  vs_light_->bind();
  fs_light_->bind();
  
  //-----------------------
  // setup.
  //-----------------------
  Vector3D lightPosition(5*sin(myLightAngle), 
                         3.5,
                         5*cos(myLightAngle));
  fs_light_->setParameterFloat3("lightPosition", lightPosition);

  Vector3D eyePosition(5, 5, 13);
  Matrix4x4 viewMatrix = Matrix4x4::buildLookAt(eyePosition, Vector3D(0, 0, 0), Vector3D(0, 1, 0));


  //------------------------
  // cube.
  //------------------------
  // model
  Matrix4x4 translateMatrix = Matrix4x4::buildTranslate(-2, 0, 0);
  Matrix4x4 modelMatrix = translateMatrix;
  // mode-view
  Matrix4x4 modelViewMatrix = viewMatrix.multiply(modelMatrix);
  // model-view-proj
  Matrix4x4 modelViewProjMatrix = myPerspectiveMatrix.multiply(modelViewMatrix);
  vs_light_->setMatrixParameter("modelViewProj", modelViewProjMatrix);
 
  // shader update
  vs_light_->update();
  fs_light_->update();

  //glutSolidCube(3.0);
  pr.drawCube(2.0, Float4(1,1,1,1));
  
  //------------------------
  // shpere.
  //------------------------
  // model
  translateMatrix = Matrix4x4::buildTranslate(+2, 0, 0);
  modelMatrix = translateMatrix;
  // mode-view
  modelViewMatrix = viewMatrix.multiply(modelMatrix);
  // model-view-proj
  modelViewProjMatrix = myPerspectiveMatrix.multiply(modelViewMatrix);
  vs_light_->setMatrixParameter("modelViewProj", modelViewProjMatrix);
  vs_light_->update();

  glutSolidSphere(1.0, 40, 40);


  //unbind
  vs_light_->unbind();
  fs_light_->unbind();

  rctx_->renderEnd();
}


void RenderGL( HDC dc )
{
  renderToTexture();
  
  PrimitiveRenderer pr(rctx_);

  rctx_->renderBegin();
  
  // vs_tex update.
  vs_tex_->setMatrixParameter("modelViewProj", myOrthoMatrix);
  vs_tex_->update();

  // fs_tex update
  fs_tex_->setParameterTexture("decal", fbuffer_texture_name);
  //fs_tex_->setParameterTexture("decal", renderbuffer_name);
  fs_tex_->update();

  // vs update
  vs_->setMatrixParameter("modelViewProj", myOrthoMatrix);
  vs_->update();
  
  glDepthMask(GL_FALSE);
  
  // tex shader.
  {
    vs_tex_->bind();
    fs_tex_->bind();
    
    pr.drawRect(Float2(10, 10), Float2(80*3, 60*3), Float4(1, 1, 1, 1));

    vs_tex_->unbind();
    fs_tex_->unbind();
  }
  
  // color shader.
  {
    vs_->bind();
    fs_->bind();
    glLineWidth(3);
    pr.drawRect(Rect(300, 100, 100, 100), Float4(0, 1, 0, 0.4));
    pr.drawRect(Float2(350, 150), Float2(100, 100), Float4(0, 1, 0, 0.4));
    pr.drawLine(Float2(100, 300), Float2(300, 350), Float4(1, 0, 0, 1));

    guiManager_.render(rctx_);

    vs_->unbind();
    fs_->unbind();
  }
  
  glDepthMask(GL_TRUE);
  
  rctx_->renderEnd();
}

#endif


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  RECT rc;
  GetClientRect( g_hWnd, &rc );
  const UINT width = rc.right - rc.left;
  const UINT height = rc.bottom - rc.top;

	switch(message){
	case WM_CLOSE:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
  case WM_LBUTTONUP:
    guiManager_.click(Float2(LOWORD(lParam), height-HIWORD(lParam)));
    break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
#if USE_GL
			wglMakeCurrent( hdc, glrc );
      glClearColor(0.1, 0.1, 0.1, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      RenderGL3( hdc );
			RenderGL( hdc );
      SwapBuffers( hdc );
			wglMakeCurrent( hdc, 0 );
#endif
			EndPaint(hWnd, &ps);
		}
  case WM_TIMER:
    if( wParam != TIMER_ID )
    {
      break;
    }
    myLightAngle += 0.03;  /* Add a small angle (in radians). */
    if (myLightAngle > 2*kRM_PI) {
      myLightAngle -= 2*kRM_PI;
    }
		InvalidateRect( hWnd, NULL, FALSE );

		return 0;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

BOOL setClientSize(const HWND hWnd, const int width, const int height)
{
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw);
	::GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
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
  g_hWnd = hWnd = CreateWindow(
    szWindowClass,
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
  
  setClientSize(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

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
#if USE_GL
    image_.release();
    ReleaseGL();
#endif
	FreeConsole();
	return 0;
}


