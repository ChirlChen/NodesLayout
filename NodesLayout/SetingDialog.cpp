#include "SetingDialog.h"
#include "debug_new.h"

CSetingDialog::CSetingDialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  qRegisterMetaType<SParamPack>("SParamPack");

  slot_paramSeting(true);     //设置默认参数;
  groupBox_2->hide();
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  connect(okButton, SIGNAL(clicked(bool)),
          this, SLOT(slot_paramSeting(bool)));
}

CSetingDialog::~CSetingDialog()
{

}

void CSetingDialog::slot_paramSeting(bool aClicked)
{
  m_ParamPack.clear();
  if (dims3->isChecked())
  {
    m_ParamPack.m_5InOneParam |= EBC_IS3Dims;
  }
  if (oneNode->isChecked())
  {
    m_ParamPack.m_5InOneParam |= EBC_seeNodesMove;
  }
  if (oneProcess->isChecked())
  {
    m_ParamPack.m_5InOneParam |= EBC_seeProcess;
  }
  m_ParamPack.m_5InOneParam |= (iterTimes->value()) << 4;
  m_ParamPack.m_AttractParam = attractParam->text().toFloat();
  m_ParamPack.m_DispMove     = dispMove->text().toFloat();
  m_ParamPack.m_RepulsParam  = repulsParam->text().toFloat();
  m_ParamPack.m_TempK        = temperatureK->text().toFloat();

  emit signal_okClicked(true);
}
