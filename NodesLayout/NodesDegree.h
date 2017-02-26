#ifndef NODESDEGREE_H
#define NODESDEGREE_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <vector>
#include "ui_DegreeInput.h"
#include "StructDefine.h"

class CNodesDegreeDialog : public QDialog, public Ui_Form
{
  Q_OBJECT

public:
    CNodesDegreeDialog(QWidget *parent);
    ~CNodesDegreeDialog();

public:
    const std::vector<SNodesDegree> &getNodeDegree(); //获取节点度信息; 
    void clearNodesDegree();          //清除节点度信息;
private:
    void setNodesDegree();            //设置节点度信息;
    const uint calcNodesNum();        //计算节点数;
   
protected slots:
    void slot_okClicked(bool);        //确定按钮;
    void slot_cancelClicked(bool);    //取消;

signals:
    void signal_degreeEditOver(bool); //节点度编辑完毕;

private:
    std::vector<SNodesDegree>   m_NodesDegree;//节点号以及度信息;
    QStringList                *m_NodesList;  //节点字符串链;
};

#endif // NODESDEGREE_H
