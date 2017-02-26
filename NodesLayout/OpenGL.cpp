#include "OpenGL.h"
#include "debug_new.h"
#include <qmath.h>
#include <QVector3D>
  
const double ORIGINAL_COORDINATE = -100.0;

COpenGL::COpenGL(QWidget *parent /* = 0 */, const char *aName /* = 0*/, bool aFullScreen /* = false */)
    : QGLWidget(parent),
      m_FullScreen(aFullScreen),
      m_Light(true),
      m_SphereRad(1.0)
{ 
  setFocusPolicy(Qt::StrongFocus);
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  
  m_Rotate = 0.0;
  m_XPos = m_YPos =  0.0;
  m_ZPos = ORIGINAL_COORDINATE;
  m_Scale = 1.0;

  m_eyex = 0.1;
  m_eyey = 0.1;
  m_eyez = 0.1;

  m_upx = 0.0;
  m_upy = 0.1;
  m_upz = 0.0;

  m_CurrentAngleZ = 0;
  m_CurrentAngleY = 0;
  m_LastAngleZ    = M_PI/4;
  m_LastAngleY    = M_PI/4;
  m_DisplayLable  = false;

  m_NodesNum  = CNodeDataManager::getInstance()->getNodeCount();  //获取节点数; 
  m_SphereRad = m_SphereRad - (m_NodesNum / 1000.0);       //根据节点数设置球的半径;
  m_SphereRad = m_SphereRad > 0.3 ? m_SphereRad : 0.3;     //半径最小0.3;
  m_EdgesNum  = CNodeDataManager::getInstance()->getEdgeCount();
  p = NULL;

//  m_TimerId = startTimer(300);   //启用定时器，每0.3s执行一次timerEvent;

//   if (m_FullScreen)
//   {
//     showFullScreen();
//   }
}

COpenGL::~COpenGL()
{
  emit signal_closedWin(1);
  killTimer(m_TimerId);
}

void COpenGL::initializeGL()
{
  glShadeModel(GL_SMOOTH);

  glClearColor(0.0, 0.7, 0.0, 0.5);
 // gluOrtho2D(0.0, 200.0, 0.0, 150.0);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  GLfloat lightAmbient[]  = {1.0, 1.0, 1.0, 0.0};     //环境光;
  GLfloat lightDiffuse[]  = {1.0, 1.0, 0.0, 1.0};     //漫反射光;
  GLfloat lightPosition[] = {0.0, 0.0, 0.0, 4.0};     //光源位置;
  GLfloat lightModel[]    = {0.1, 0.1, 0.1, 1.0};     //全局环境光;
  GLfloat materialSP[]    = {1.0, 1.0, 1.0, 1.0};     //设置材质;
  GLfloat materialSH[]    = {50.0};

  glLoadIdentity();
//   glMaterialfv(GL_FRONT, GL_SPECULAR, materialSP);
//   glMaterialfv(GL_FRONT, GL_SHININESS, materialSH);

  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);   
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);       //设定漫反射效果;
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightDiffuse);      //设定高光效果;
  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);     //设定光源位置;
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModel);   //全局环境光;
  glEnable(GL_LIGHT1);

  if (m_Light)
  {
    glEnable(GL_LIGHTING);
  }
}

void COpenGL::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  m_NodesNum  = CNodeDataManager::getInstance()->getNodeCount();
  m_EdgesNum  = CNodeDataManager::getInstance()->getEdgeCount();

  p = &(CNodeDataManager::getInstance()->getNodesInfo());

  for (int idx = 0; idx < m_NodesNum; ++idx)
  {
    drawSphere((*p)[idx].m_Coordinate.m_X,
               (*p)[idx].m_Coordinate.m_Y,
               (*p)[idx].m_Coordinate.m_Z);
    if (m_DisplayLable)
    {
      dispalyLable((*p)[idx]);
    }
  }

  glLoadIdentity();
  glTranslatef(m_XPos, m_YPos, m_ZPos);
  gluLookAt(m_eyex*0.1, m_eyey*0.1, m_eyez*0.1, 
            0.0, 0.0, 0.0, m_upx*0.1, m_upy*0.1, m_upz *0.1);

  glRotated(m_Rotate, 0.0, 1.0, 0.0);
  //glScalef(m_Scale, m_Scale, m_Scale);
  std::vector<SEdgeInfo> &e = (CNodeDataManager::getInstance()->getEdgesInfo());
  for (int idx = 0; idx < m_EdgesNum; ++idx)
  {
    int nodeID1 = e[idx].m_Nodes.m_Node1 - 1;
    int nodeID2 = e[idx].m_Nodes.m_Node2 - 1;
    glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glLineWidth(5.0);
      glVertex3f((*p)[nodeID1].m_Coordinate.m_X,
                 (*p)[nodeID1].m_Coordinate.m_Y,
                 (*p)[nodeID1].m_Coordinate.m_Z);
      glVertex3f((*p)[nodeID2].m_Coordinate.m_X,
                 (*p)[nodeID2].m_Coordinate.m_Y,
                 (*p)[nodeID2].m_Coordinate.m_Z);
    glEnd();
  }

//  QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);
}

void COpenGL::RotateViewPoint()
{
  double avAnale = M_PI / 180*0.2; //把每次移动的角度单位化;

  //把每次移动点跟开始按下鼠标记录的点作差，然后乘以avAngle,最后把上一次释放鼠标后时记录的;
  //角度相加起来;
  m_CurrentAngleZ  = (m_EndPoint.x()-m_StartPoint.x()) * avAnale;
  m_CurrentAngleZ += m_LastAngleZ;
  m_CurrentAngleY  = (m_EndPoint.y()-m_StartPoint.y()) * avAnale;
  m_CurrentAngleY += m_LastAngleY;

  QVector3D vector1(sin(m_CurrentAngleY) * sin(m_CurrentAngleZ),
                    cos(m_CurrentAngleY),
                    sin(m_CurrentAngleY) * cos(m_CurrentAngleZ));
  vector1 = vector1.normalized();  //将坐标单位化;
  m_eyex = vector1.x();
  m_eyey = vector1.y();
  m_eyez = vector1.z();

  /*主要计算第三组坐标*/
  QVector3D vectorA(0, sin(m_CurrentAngleY), 0);
  QVector3D vectorB = QVector3D(0, 0, 0) - 
                      QVector3D(sin(m_CurrentAngleY) * sin(m_CurrentAngleZ), 
                                0, 
                                sin(m_CurrentAngleY) * cos(m_CurrentAngleZ));
  QVector3D vectorAB = QVector3D::crossProduct(vectorA, vectorB);

  QVector3D vectorC = QVector3D(0, 0, 0) - vector1;
  QVector3D vector2 = QVector3D::crossProduct(vectorC, vectorAB);
  vector2 = vector2.normalized();
  m_upx = vector2.x();
  m_upy = vector2.y();
  m_upz = vector2.z();
}


void COpenGL::resizeGL(int width, int height)
{
  if (0 == height)
  {
    height = 1;
  }
  WINDOW_WIDTH = width;
  WINDOW_HEIGHT = height;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void COpenGL::keyPressEvent(QKeyEvent *e)
{
  switch(e->key())
  {
  case Qt::Key_F2:
    m_FullScreen = !m_FullScreen;
    if (m_FullScreen)
    {
      showFullScreen();
    }
    else
    {
      setWindowFlags(Qt::SubWindow);
      showNormal();
      //setGeometry(300, 200, 640, 480);
    }
    updateGL();
    break;
  case  Qt::Key_Q:
    m_Scale -= 0.1;
    updateGL();
    break;
  case  Qt::Key_E:
    m_Scale += 0.1;
    updateGL();
    break;
  case Qt::Key_L:
    m_Light = !m_Light;
    if (m_Light)
    {
      glEnable(GL_LIGHTING);
    }
    else
      glDisable(GL_LIGHTING);
    updateGL();
    break;
  case Qt::Key_A:
    m_Rotate += 2;
    updateGL();
    break;
  case Qt::Key_D:
    m_Rotate -= 2;
    updateGL();
    break;
  case Qt::Key_Left:
    m_XPos -= 2;
    updateGL();
    break;
  case Qt::Key_Right:
    m_XPos += 2;
    updateGL();
    break;
  case Qt::Key_Up:
    m_YPos += 2;
    updateGL();
    break;
  case Qt::Key_Down:
    m_YPos -= 2;
    updateGL();
    break;
  case Qt::Key_W:
    m_ZPos += 2;
    updateGL();
    break;
  case Qt::Key_S:
    m_ZPos -= 2;
    updateGL();
    break;
  case Qt::Key_Escape:
    close();
  }
  
}

void COpenGL::mouseMoveEvent(QMouseEvent *e)
{
  QPoint ed(e->pos());

  m_EndPoint = ed;
  RotateViewPoint();
  updateGL();
}

void COpenGL::mousePressEvent(QMouseEvent *e)
{
  if (!hasFocus())
  {
     setFocus();
  }

  QPoint st(e->pos());
  m_StartPoint = st;
}

void COpenGL::mouseReleaseEvent(QMouseEvent *e)
{
  /*记录上一次的角度*/
  m_LastAngleZ = m_CurrentAngleZ;
  m_LastAngleY = m_CurrentAngleY;
}

void COpenGL::wheelEvent(QWheelEvent *e)
{

}

void COpenGL::drawSphere(GLfloat aX, GLfloat aY, GLfloat aZ)
{
  glLoadIdentity();
  glTranslatef(m_XPos, m_YPos, m_ZPos);
  gluLookAt(m_eyex*0.1, m_eyey*0.1, m_eyez*0.1, 
            0.0, 0.0, 0.0, m_upx*0.1, m_upy*0.1, m_upz *0.1);
  glRotated(m_Rotate, 0.0, 1.0, 0.0);
//  glScalef(m_Scale, m_Scale, m_Scale);

  glTranslatef(aX, aY, aZ);
  glColor3f(0.5, 0.1, 0.1);
  glutSolidSphere(m_SphereRad, 32, 32);
}

void COpenGL::timerEvent(QTimerEvent *t)
{
  updateGL();
}

void COpenGL::slot_algorithmFinished(int aId)
{
  killTimer(m_TimerId);  
  updateGL();   //最后更新一次图像;
}

void COpenGL::slot_fishEyes(bool aFishEyes)
{
  if (aFishEyes)
  {
    m_SphereRad *= 12;
  }
  else
    m_SphereRad /= 12;
  

}

void COpenGL::slot_algorithmBegin(SParamPack aP)
{
  m_TimerId = startTimer(300);
}

void COpenGL::slot_lableDisplay(bool aDisplay)
{
  m_DisplayLable = aDisplay;
}

void COpenGL::dispalyLable(SNodeInfo &aNI)
{
  glLoadIdentity();
  glTranslatef(m_XPos, m_YPos, m_ZPos);
  gluLookAt(m_eyex*0.1, m_eyey*0.1, m_eyez*0.1, 
            0.0, 0.0, 0.0, m_upx*0.1, m_upy*0.1, m_upz *0.1);
  glRotated(m_Rotate, 0.0, 1.0, 0.0);
  
  qglColor(QColor(255, 100, 0));
  QFont lableFont("Times",  m_SphereRad);
  QString lable;

  lable.setNum(aNI.m_NodeID);
  renderText(aNI.m_Coordinate.m_X + m_SphereRad, aNI.m_Coordinate.m_Y + m_SphereRad,
              aNI.m_Coordinate.m_Z - m_SphereRad, lable, lableFont);
 
}