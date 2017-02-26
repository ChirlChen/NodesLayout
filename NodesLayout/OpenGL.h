#ifndef OPENGL_H
#define OPENGL_H

#include <QtOpenGL/qgl.h>
#include <gl/glut.h>
#include <qevent.h>
#include "NodeDataManager.h"

class COpenGL : public QGLWidget
{
    Q_OBJECT

public:
    COpenGL(QWidget *parent = 0, const char *aName = 0, bool aFullScreen = false);
    ~COpenGL();
   
    void drawSphere(GLfloat aX, GLfloat aY, GLfloat aZ);  //画球体;

protected:
    void initializeGL();                    //初始化OpenGL窗口;
    void paintGL();                         //绘制OpenGL窗口，有更新就会调用;
    void resizeGL( int width, int height ); //当窗口长宽变化，就会调用;
    void keyPressEvent(QKeyEvent *e);       //按键处理;
    void mousePressEvent(QMouseEvent *e);   //鼠标点击事件;
    void mouseMoveEvent(QMouseEvent *e);    //鼠标移动事件;
    void mouseReleaseEvent(QMouseEvent *e); //鼠标释放事件;
    void wheelEvent(QWheelEvent *e);        //滚轮事件;

    void timerEvent(QTimerEvent *t);        //计时器;

    void RotateViewPoint();                 //用鼠标旋转视点;
    void dispalyLable(SNodeInfo &aNI);       //是否显示节点标签;
protected slots:
    void slot_fishEyes(bool aFishEyes);       //鱼眼;
    void slot_algorithmFinished(int);         //算法迭代结束;
    void slot_algorithmBegin(SParamPack aP);  //算法开始，开始刷新图像;
    void slot_lableDisplay(bool aDisplay);    //设置显示节点标签;
signals:
     void signal_closedWin(int);              //关闭窗口，参数为窗口ID,为功能扩展做准备;
private:
    bool        m_FullScreen;   //是否全屏;
    bool        m_Light;        //是否打开光源;
    double      m_Rotate;       //旋转角度;

    GLfloat     m_ZPos;         //深度;
    GLfloat     m_XPos;         //X坐标;
    GLfloat     m_YPos;         //Y坐标;

    GLfloat     m_Scale;        //缩放控制;
//鼠标控制旋转;
private:
    QPoint      m_StartPoint;     //记录鼠标按下的点;
    QPoint      m_EndPoint;       //鼠标移动过程中的点;

    //gluLookAt函数中第一组坐标跟第三组坐标;
    GLdouble    m_eyex;
    GLdouble    m_eyey;
    GLdouble    m_eyez;

    GLdouble    m_upx;
    GLdouble    m_upy;
    GLdouble    m_upz ;

    double       m_CurrentAngleZ;   //当前与Z轴的夹角;
    double       m_CurrentAngleY;   //当前与Y轴的夹角;

    double       m_LastAngleZ;      //上一次与Z轴的夹角;
    double       m_LastAngleY;      //上一次与Y轴的夹角;

private:
    double       m_SphereRad;       //球的半径;
    uint         m_NodesNum;        //节点数;
    uint         m_EdgesNum;        //边数;
    int          m_TimerId;         //计时器ID;
    std::vector<SNodeInfo> *p;      //取节点数据; 
    bool         m_DisplayLable;    //是否显示节点标签;
};

#endif // OPENGLTEST_H
