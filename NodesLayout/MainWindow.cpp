#include "MainWindow.h"
#include "MacroDef.h"
#include "TabWidget.h"
#include "NodeDataManager.h"
#include "OpenGL.h"
#include "FRAlgorithm.h"
#include "SetingDialog.h"
#include "NodesDegree.h"
#include "debug_new.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include <QThread>

#define THE_HEADER_BELOW_IS_FOR_TEST 0  //测试程序开关;
#if THE_HEADER_BELOW_IS_FOR_TEST

#endif



CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_SetingDialog(NULL),
      m_DegreeDialog(NULL),
      m_Algorithm(NULL),
      m_AlgoThread(NULL),
      m_FullScreen(false),
      m_fishEyes(false),
      m_displayLable(false)
{
    //初始化;
    m_ui.setupUi(this);

    //初始化参数设置对话框;
    m_SetingDialog = new CSetingDialog(this);
    m_SetingDialog->hide();
    
    m_DegreeDialog = new CNodesDegreeDialog(this);
    m_DegreeDialog->hide();

    //线程相关操作;
    m_AlgoThread = new QThread;
    m_Algorithm  = new CFRAlgorithm;
    m_Algorithm->moveToThread(m_AlgoThread);
    connect(this, SIGNAL(signal_runAlgorithm(SParamPack)),
            m_Algorithm, SLOT(slot_runAlgorithm(SParamPack)));
    connect(m_Algorithm, SIGNAL(signal_algoFinished(int)),
            this, SLOT(slot_algoFinished(int)));

    setCentralWidget(CTabWidget::getInstance());  //设置主窗口;
//    connect(CTabWidget::getInstance(), SIGNAL())

    //按键与功能进绑定;
    connect(m_ui.newFile, SIGNAL(triggered(bool)),
            this, SLOT(slot_newFile(bool)));
    connect(m_ui.openFile, SIGNAL(triggered(bool)),
            this, SLOT(slot_openFile(bool)));
    connect(m_ui.savePic, SIGNAL(triggered(bool)),
            this, SLOT(slot_savePic(bool)));
    connect(m_ui.saveDoc, SIGNAL(triggered(bool)),
            this, SLOT(slot_saveDoc(bool)));
    connect(m_ui.exit, SIGNAL(triggered(bool)),
            this, SLOT(slot_exit(bool)));
//     connect(m_ui.algoriParamSeting, SIGNAL(triggered(bool)),
//             this, SLOT(slot_algorithmSet(bool)));
    connect(m_ui.beginLayout, SIGNAL(triggered(bool)),
            this, SLOT(slot_beginLayout(bool)));
    connect(m_ui.stopLayout, SIGNAL(triggered(bool)),
            this, SLOT(slot_stopLayout(bool)));
    connect(m_SetingDialog, SIGNAL(signal_okClicked(bool)),
            this, SLOT(slot_runAlgorithm(bool)));
    connect(m_ui.fishEyes, SIGNAL(triggered(bool)),
            this, SLOT(slot_fishEyes(bool)));
    connect(m_ui.continueLayout, SIGNAL(triggered(bool)),
            this, SLOT(slot_continueLayout(bool)));
    connect(m_ui.relayout, SIGNAL(triggered(bool)),
            this, SLOT(slot_relayout(bool)));
    connect(m_ui.fullScreen, SIGNAL(triggered(bool)),
            this, SLOT(slot_fullScreen(bool)));
    connect(m_ui.nodesDegreeEdit, SIGNAL(triggered(bool)),
            this, SLOT(slot_degreeEdit(bool)));
    connect(m_DegreeDialog, SIGNAL(signal_degreeEditOver(bool)),
            this, SLOT(slot_degreeEditOver(bool)));
    connect(m_ui.lableDisplay,  SIGNAL(triggered(bool)),
            this, SLOT(slot_lableDisplay(bool)));
    if (m_FullScreen)
    {
      showFullScreen();
    }
#if THE_HEADER_BELOW_IS_FOR_TEST
    test();
#endif
}

CMainWindow::~CMainWindow()
{
  CTabWidget::destroyInstance();
  CNodeDataManager::destroyInstance();
  IF_NOT_NULL_DELETE_s(m_Algorithm);
  if (!m_AlgoThread->isFinished())
  {
    m_AlgoThread->quit();
  }
  IF_NOT_NULL_DELETE_s(m_AlgoThread);
  IF_NOT_NULL_DELETE_s(m_SetingDialog);
}

void CMainWindow::slot_openFile(bool aClicked)
{

}

void CMainWindow::slot_newFile(bool aClicked)
{
  m_FilePath.clear();
  m_FilePath = QFileDialog::getOpenFileName(this, tr("Open File"), 
                                            "", tr("files(*.nrd)"));
  if (m_FilePath.isEmpty())
  { return;}

  QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
  QByteArray tempPath = m_FilePath.toLocal8Bit();
  char *p = tempPath.data();

  CNodeDataManager::getInstance()->loadEdgesNodesInfo(p);
  CNodeDataManager::getInstance()->initNodesPostion();
  
  this->openglConfig();
  //  m_ui.algoriParamSeting->setEnabled(true);
}

void CMainWindow::slot_savePic(bool aClicked)
{


}

void CMainWindow::slot_saveDoc(bool aClicked)
{

}

void CMainWindow::slot_exit(bool aClicked)
{
  close();
}

// void CMainWindow::slot_algorithmSet(bool aClicked)
// {
//   m_SetingDialog->show();
// }

void CMainWindow::slot_degreeEdit(bool aClicked)
{
  if (NULL != m_DegreeDialog)
  {
    m_DegreeDialog->show();
  }
}

void CMainWindow::slot_algoFinished(int aAlgorId)
{
  m_ui.relayout->setEnabled(true);
  m_ui.stopLayout->setEnabled(false);
  m_AlgoThread->terminate();

  emit signal_algFinished(1);
}

void CMainWindow::slot_beginLayout(bool aClicked)
{
//   if (m_FilePath.isEmpty())
//   {
//     QMessageBox::warning(this, tr("Prompting"),
//                 tr("There is no file! \nPlease Open a File!!!"),
//                 QMessageBox::Ok);
//   }
  CNodeDataManager::getInstance()->initNodesPostion();

  m_SetingDialog->show();
  
  m_ui.beginLayout->setEnabled(false);
  m_ui.stopLayout->setEnabled(true); 
}

void CMainWindow::slot_continueLayout(bool aClicked)
{
  m_SetingDialog->show();
  m_ui.continueLayout->setEnabled(false);
  m_ui.stopLayout->setEnabled(true);
}

void CMainWindow::slot_stopLayout(bool aClicked)
{
  m_AlgoThread->terminate();    //模拟暂停线程,这种方式暂停线程有问题，有待解决;
  m_ui.stopLayout->setEnabled(false);
  m_ui.continueLayout->setEnabled(true);
}

void CMainWindow::test()
{
  //CNodeDataManager::getInstance();
}

void CMainWindow::slot_runAlgorithm(bool aClicked)
{
  if (!m_FilePath.isEmpty() || !m_DegreeDialog->getNodeDegree().empty())
  {
    if (m_AlgoThread->isRunning())
    {
      m_AlgoThread->terminate();
    }
    m_AlgoThread->start();            //开始算法线程;
    emit signal_runAlgorithm(m_SetingDialog->m_ParamPack);    //发送开始算法信号（以及算法参数）;
//    m_FilePath.clear();               //清空文件路劲;
  }
}

void CMainWindow::slot_fishEyes(bool aClicked)
{
  m_fishEyes = !m_fishEyes;
  emit signal_fishEyes(m_fishEyes);
}

void CMainWindow::slot_relayout(bool aClicked)
{
  CNodeDataManager::getInstance()->initNodesPostion();
  m_SetingDialog->show();
  m_ui.stopLayout->setEnabled(true);
  m_ui.relayout->setEnabled(false);
}

void CMainWindow::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
  {
//   case Qt::Key_F2:
//     m_FullScreen = !m_FullScreen;
//     if (m_FullScreen)
//     {
//       showFullScreen();
//     }
//     else 
//     {
//       showNormal();
//     }
//     break;
  }
}

void CMainWindow::slot_openGLWinClosed(int aWinId)
{
  m_ui.menu_2->setEnabled(false);
  m_ui.relayout->setEnabled(false);
  m_ui.fishEyes->setEnabled(false);
  m_ui.lableDisplay->setEnabled(false);
}

void CMainWindow::slot_fullScreen(bool aF)
{
  m_FullScreen = !m_FullScreen;
  if (m_FullScreen)
  {
    showFullScreen();
  }
  else
    showNormal();
}

void CMainWindow::slot_degreeEditOver(bool aOver)
{
  CNodeDataManager::getInstance()->setEdgesNodesInfo(m_DegreeDialog->getNodeDegree());
  CNodeDataManager::getInstance()->initNodesPostion();
  
  this->openglConfig();
}

void CMainWindow::slot_lableDisplay(bool aClicked)
{
  m_displayLable = !m_displayLable;

  emit(signal_lableDisplay(m_displayLable));
}

void CMainWindow::openglConfig()
{
  m_OpenGL = new COpenGL;
  CTabWidget::getInstance()->newTab(m_OpenGL, "OpenGL");
  connect(this, SIGNAL(signal_fishEyes(bool)),    //该信息传递的解决方案有问题;
          m_OpenGL, SLOT(slot_fishEyes(bool)));
  connect(this, SIGNAL(signal_algFinished(int)),
          m_OpenGL, SLOT(slot_algorithmFinished(int)));
  connect(this, SIGNAL(signal_runAlgorithm(SParamPack)),
          m_OpenGL, SLOT(slot_algorithmBegin(SParamPack)));
  connect(m_OpenGL, SIGNAL(signal_closedWin(int)),
          this, SLOT(slot_openGLWinClosed(int)));
  connect(this, SIGNAL(signal_lableDisplay(bool)),
          m_OpenGL, SLOT(slot_lableDisplay(bool)));
  m_ui.beginLayout->setEnabled(true);
  m_ui.relayout->setEnabled(false);
  m_ui.fishEyes->setEnabled(true);
  m_ui.menu_2->setEnabled(true);
  m_ui.lableDisplay->setEnabled(true);
}