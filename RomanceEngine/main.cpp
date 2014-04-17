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

using namespace std;
using namespace RomanceEngine::Math;
using namespace RomanceEngine::Memory;
using namespace RomanceEngine::Render;
using namespace RomanceEngine::Image;

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
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  virtual void renderEnd()
  {
    glDisableClientState(GL_VERTEX_ARRAY);
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

namespace RomanceEngine {
namespace GUI {

class GUIObject
{
public:
  GUIObject() {}
  virtual ~GUIObject() {};

  virtual Math::Rect getRegion() const { return region_; }
  virtual void setRegion(const Math::Rect& region) { region_ = region; }

  virtual void click(const Math::Float2& p)
  {
    clickEvent_(p);
  };

  virtual void render(const RenderContextPtr& context) {};
  virtual void update() {};

public:
  boost::signals2::signal<void(const Math::Float2&)> clickEvent_;

protected:
  Math::Rect region_;
};
typedef Memory::SharedPtr<GUIObject> GUIObjectPtr;

class Button : public GUIObject
{
public:
  virtual ~Button() {}

  virtual void render(const RenderContextPtr& context)
  {
    PrimitiveRenderer renderer(context);
    renderer.drawRect(region_, Math::Float4(1, 1, 0, 0.5));
  }

};

class GUIManager
{
public:
  GUIManager() {}
  ~GUIManager() {}

  void update()
  {
    for (int i=0; i < (int)objects_.size(); ++i)
    {
      objects_[i]->update();
    }
  }

  void render(const RenderContextPtr& context)
  {
    for (int i=0; i < (int)objects_.size(); ++i)
    {
      objects_[i]->render(context);
    }
  }

  void add(const GUIObjectPtr& obj)
  {
    objects_.push_back(obj);
  }

  void click(const Math::Float2& p)
  {
    cout << p.x_ << ", " << p.y_ << endl;
    for (int i=0; i < (int)objects_.size(); ++i)
    {
      if (objects_[i]->getRegion().contains(p))
      {
        objects_[i]->click(p);
        break;
      }
    }
  }

private:
  std::vector<GUIObjectPtr> objects_;
};

} // GUI
} // RomanceEngine
using namespace RomanceEngine::GUI;

void clickA(const Float2& p)
{
  cout << "clickedA : (" << p.x_ << ", " << p.y_ << ")" << endl;
}

void clickB(const Float2& p)
{
  cout << "clickedB : (" << p.x_ << ", " << p.y_ << ")" << endl;
}

namespace {
  enum {
    kRM_SHADER_COLOR_SIMPLE=0,
    kRM_SHADER_TEX_SIMPLE,

    kRM_SHADER_N
  };
}

#if 0
// [WIP]
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
  
  rctx_ = RenderContextPtr(new GLRenderContext());
  rctx_->init();

  vs_ = rctx_->getShaderManager()->createVertexShader("shader/color_simple_v.cg", "color_simple_v_main");
  fs_ = rctx_->getShaderManager()->createFragmentShader("shader/color_simple_f.cg", "color_simple_f_main");
  vs_->registParameter("modelViewProj");

  vs_tex_ = rctx_->getShaderManager()->createVertexShader("shader/tex_simple_v.cg", "tex_simple_v_main");
  fs_tex_ = rctx_->getShaderManager()->createFragmentShader("shader/tex_simple_f.cg", "tex_simple_f_main");
  vs_tex_->registParameter("modelViewProj");
  fs_tex_->registParameter("decal");


  reshape(width, height);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glewExperimental=GL_TRUE;
  if ( glewInit() != GLEW_OK )
  {
    printf("Error : GLEWの初期化に失敗しました\n");
    return 0;
  }

  image_.Load("fish1.dds");
  fs_tex_->setParameterTexture("decal", image_.ID);


  {
    Button* b1 = new Button();
    Button* b2 = new Button();
    b1->setRegion(Rect(400, 400, 50, 50));
    b2->setRegion(Rect(460, 400, 80, 80));
    b1->clickEvent_.connect(clickA);
    b2->clickEvent_.connect(clickB);

    guiManager_.add(GUIObjectPtr(b1));
    guiManager_.add(GUIObjectPtr(b2));
  }

  {
    const int bufferW = 800;
    const int bufferH = 600;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
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
    glGenRenderbuffersEXT( 1, &renderbuffer_name );
    glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderbuffer_name );
    glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                              bufferW, bufferH );

    // init frame buffer;
    glGenFramebuffersEXT( 1, &framebuffer_name );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );
    
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D, fbuffer_texture_name, 0 );
    glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
      GL_RENDERBUFFER_EXT, renderbuffer_name );
    
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
  vs_tex_->setMatrixParameter("modelViewProj", myProjectionMatrix);
  vs_tex_->update();

  // fs_tex update
  fs_tex_->setParameterTexture("decal", image_.ID);
  fs_tex_->update();

  // vs_update
  vs_->setMatrixParameter("modelViewProj", myProjectionMatrix);
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
  
  
  //myProjectionMatrix = Matrix4x4::buildPerspective(fieldOfView, aspectRatio, 1.0, 20.0  /* Znear and Zfar */);  
  myProjectionMatrix = Matrix4x4::buildOrth(0, width, 0, height, 0.0, 20.0);
  
  glViewport(0, 0, width, height);
}

static const double myPi = 3.14159265358979323846;

#if 0
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
#endif

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

#if 0
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
#endif

void RenderGL( HDC dc )
{
  renderToTexture();
  
  glClearColor(0.1, 0.1, 0.1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  PrimitiveRenderer pr(rctx_);

  rctx_->renderBegin();
  
  // vs_tex update.
  vs_tex_->setMatrixParameter("modelViewProj", myProjectionMatrix);
  vs_tex_->update();

  // fs_tex update
  fs_tex_->setParameterTexture("decal", fbuffer_texture_name);
  fs_tex_->update();

  // vs update
  vs_->setMatrixParameter("modelViewProj", myProjectionMatrix);
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

  SwapBuffers( dc );
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
			RenderGL( hdc );
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
#endif
	FreeConsole();
	return 0;
}


