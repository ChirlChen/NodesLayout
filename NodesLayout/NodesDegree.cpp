#include "NodesDegree.h"
#include <QMessageBox>
#include "DegreeAlgorithm.h"

CNodesDegreeDialog::CNodesDegreeDialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);
  
  m_NodesDegree.clear();
  m_NodesList   = NULL;

  connect(okButton, SIGNAL(clicked(bool)),
          this, SLOT(slot_okClicked(bool)));
  connect(cancelButton, SIGNAL(clicked(bool)),
          this, SLOT(slot_cancelClicked(bool)));
}

CNodesDegreeDialog::~CNodesDegreeDialog()
{
  if (NULL != m_NodesList)
  {
    m_NodesList->clear();
  }
}

const std::vector<SNodesDegree> &CNodesDegreeDialog::getNodeDegree()
{
  return m_NodesDegree;
}

void CNodesDegreeDialog::clearNodesDegree()
{
  if (!m_NodesDegree.empty())
  {
    m_NodesDegree.clear();
  }
}

void CNodesDegreeDialog::setNodesDegree()
{
  m_NodesDegree.clear();

  if (textEdit->toPlainText().isEmpty())
  { return;}
  
  m_NodesDegree.resize(calcNodesNum());

  for (int idx = 0; idx < m_NodesDegree.size(); ++idx)
  {
    m_NodesDegree[idx].m_NodesID = idx + 1;
    m_NodesDegree[idx].m_NodesDegree = m_NodesList->at(idx).toInt();
  }

  if (!m_NodesList->isEmpty())
  {
    m_NodesList->clear();
    m_NodesList = NULL;
  }
}

const uint CNodesDegreeDialog::calcNodesNum()
{
  m_NodesList = new QStringList(textEdit->toPlainText().split(',', QString::SkipEmptyParts));
  
  return m_NodesList->length();
}

void CNodesDegreeDialog::slot_cancelClicked(bool aClicked)
{
  textEdit->clear();
  this->hide();
}

void CNodesDegreeDialog::slot_okClicked(bool aClicked)
{
  if (textEdit->toPlainText().isEmpty())  //节点度信息为空;
  {
    QMessageBox::warning(this, "Waring", "Please Input Nodes Degree Sequence!");
    return;
  }

  this->setNodesDegree();             //保存节点度信息;
  if (!CDegreeAlgorithm::getInstance()->isGraphSequence(m_NodesDegree))
  {
    QMessageBox::warning(this, "Waring", "The Sequence is not Graph Sequence!\nPlease Input Another Sequence!");
    return;
  }
  textEdit->clear();  //清理编辑框内容;
  this->hide();

  emit signal_degreeEditOver(true);
}
