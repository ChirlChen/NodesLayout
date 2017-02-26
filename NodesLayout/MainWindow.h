#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "StructDefine.h"
#include "OpenGL.h"
#include <QString>
#include <QVBoxLayout>

class CFRAlgorithm;
class CSetingDialog;
class CNodesDegreeDialog;

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

public:
    void keyPressEvent(QKeyEvent *e);       //重载按键事件;
public slots:
    void slot_newFile(bool aClicked);       //新建文件,即打开一个节点数据包;
    void slot_openFile(bool aClicked);      //打开文件;
    void slot_savePic(bool aClicked);       //保存布局结果图片;
    void slot_saveDoc(bool aClicked);       //保存布局结果文档;
    void slot_exit(bool aClicked);          //退出程序;
 //   void slot_algorithmSet(bool aClicked);  //设置布局算法相关量;
    void slot_degreeEdit(bool aClicked);    //度序列编辑;  
    void slot_beginLayout(bool aClicked);   //开始节点布局(只是进入布局设置，以及准备前期工作），并绘制布局三/二维图像;
    void slot_algoFinished(int aAlgorId);   //算法计算结束(aAlgorId为布局结束算法的ID,用于后面功能扩展，控制多算同时工作);
    void slot_stopLayout(bool);             //暂停布局;
    void slot_continueLayout(bool);         //继续布局（只是进入布局设置，以及准备前期工作）;
    void slot_runAlgorithm(bool);           //开始运行算法;
    void slot_fishEyes(bool);               //鱼眼效果，暂时未启用，正在思考参数传递;
    void slot_relayout(bool);               //重新开始布局;
    void slot_openGLWinClosed(int aWinId);  //opengl窗口关闭;
    void slot_fullScreen(bool);             //全屏/半屏切换;
    void slot_degreeEditOver(bool);         //节点度信息编辑完毕;
    void slot_lableDisplay(bool);           //显示节点标签;
signals:
    void signal_runAlgorithm(SParamPack aData);    //发射开始算法信号,aData还有待确定;
    void signal_algFinished(int aAlgorId);  //算法结束信号(aAlgorId为布局结束算法的ID);    
    void signal_fishEyes(bool);             //鱼眼信号;
    void signal_lableDisplay(bool);         //显示节点标签;
private:
    void test();                            //用于测试部分代码;
    void paramSeting();                     //从参数设置对话框获取算法参数;
    void openglConfig();                    //对OpenGL进行配置;
private:
    Ui::CMainWindowClass  m_ui;             //主界面;
    CSetingDialog        *m_SetingDialog;   //参数设置;
    CNodesDegreeDialog         *m_DegreeDialog;   //节点度编辑窗口;
    QString               m_FilePath;       //新建文件路径;
    CFRAlgorithm         *m_Algorithm;      //算法类; 
    QThread              *m_AlgoThread;     //算法工作线程;
    bool                  m_FullScreen;     //切换全屏;
    COpenGL              *m_OpenGL;         //暂时用来跟图像界面进行信号传递,等待之后扩展为线程池和数据池;
    bool                  m_fishEyes;       //是否鱼眼效果;
    bool                  m_displayLable;   //是否显示节点标签;
};

#endif // MAINWINDOW_H
