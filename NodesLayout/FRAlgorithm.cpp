#include "FRAlgorithm.h"
#include <vector>
#include <QThread>
#include <ctime>
#include "NodeDataManager.h"
#include "debug_new.h"

CFRAlgorithm::CFRAlgorithm(QObject *parent)
  : QObject(parent),
    m_IterTimes(0),    //默认迭代次数0次;
    m_IdealDist(0.0),
    m_NodesDist(0.0),
    m_Repulsion(0.0),
    m_Attraction(0.0),
    m_UnitDistance(0.001),
    m_Temperature(WINDOW_WIDTH / 2),
    m_TempK(0.7),
    m_AttractParam(100.0),
    m_RepulsParam(0.08)
{
   
}

CFRAlgorithm::~CFRAlgorithm(void)
{
}

void CFRAlgorithm::FRAlgorithm(const short aData)
{
  m_IterTimes = (aData & EBC_IterTimes) >> 4;
  if (0 == m_IterTimes)
  { return;}

  std::vector<SEdgeInfo> &e = (CNodeDataManager::getInstance()->getEdgesInfo());
  std::vector<SNodeInfo> &p = (CNodeDataManager::getInstance()->getNodesInfo());
  
  if ((aData & EBC_IS3Dims) == 0)  //二维切换;
  {
    for (int idx = 0; idx < p.size(); ++idx)
    {
      p[idx].m_Coordinate.m_Z = 0.0;
    }
  }
  
  calcIdealDist(p.size());  //计算理想距离;
  for (int iterTimes = 0; iterTimes < m_IterTimes; ++iterTimes)   //迭代次数;
  {
    //下面的过程有待优化;
    for (int nodesIdx = 0; nodesIdx < p.size(); ++nodesIdx)       //计算斥力;
    {
      for (int nodesIdx1 = 0; nodesIdx1 < p.size(); ++nodesIdx1)
      {
        if (nodesIdx1 != nodesIdx)
        {
          calcDistance(p[nodesIdx].m_Coordinate, p[nodesIdx1].m_Coordinate);  //计算两个点间的距离;
          calcRepulsion(m_NodesDist, m_IdealDist);        //算斥力;
          calcJoinForce(p[nodesIdx].m_Coordinate,      //计算nodesIdx点受到的合力; 
                        p[nodesIdx1].m_Coordinate, 
                          m_Repulsion);
        }
      }
      for (int edgeIdx = 0; edgeIdx < e.size(); ++edgeIdx)    //算引力;
      {
         if (e[edgeIdx].m_Nodes.m_Node1 == nodesIdx + 1)
         {
            e[edgeIdx].m_EdgeLegth = calcDistance(p[e[edgeIdx].m_Nodes.m_Node1 - 1].m_Coordinate, 
                                                     p[e[edgeIdx].m_Nodes.m_Node2 - 1].m_Coordinate); //计算距离;
            calcAttraction(m_NodesDist, m_IdealDist);              //计算引力;
            calcJoinForce(p[nodesIdx].m_Coordinate,             //计算nodesIdx点受到的合力; 
                          p[e[edgeIdx].m_Nodes.m_Node2 - 1].m_Coordinate, 
                            m_Attraction);
         }
         else if (e[edgeIdx].m_Nodes.m_Node2 == nodesIdx + 1)
         {
           e[edgeIdx].m_EdgeLegth = calcDistance(p[nodesIdx].m_Coordinate,      //计算距离;
                                        p[e[edgeIdx].m_Nodes.m_Node1 - 1].m_Coordinate); 
           
           calcAttraction(m_NodesDist, m_IdealDist);              //计算引力;
           calcJoinForce(p[nodesIdx].m_Coordinate,             //计算nodesIdx点受到的合力; 
                         p[e[edgeIdx].m_Nodes.m_Node1 - 1].m_Coordinate, 
                           m_Attraction);
         }
      }
      calcMoveCoordinate(p[nodesIdx].m_Coordinate, m_JionForce);  //计算所受合力下的移动距离;
      if ((aData & EBC_seeNodesMove))  //如果要逐个节点查看布局;
      {QThread::msleep(300);}
    }
    if (aData & EBC_seeProcess)        //如果用户需要逐个过程查看布局;
    { QThread::msleep(500);}
           
    m_JionForce.clear();              //清除上次的合力数据;
    cool();

    //将图形移到屏幕中央;
    for (int indx = 1; indx < p.size(); ++indx)
    {
      p[indx].m_Coordinate -= p[0].m_Coordinate;
    }
    p[0].m_Coordinate.clear();
  }

  emit signal_algoFinished(1);   //信号发射数据应该是线程ID;
  return;
}

inline double CFRAlgorithm::calcRepulsion(const double &aDist, const double &aIdealDist)
{
  if (0.0 == aDist)
  {
    m_Repulsion = INFINITY_NUM;
  }
  else
    m_Repulsion = powf(aIdealDist, 2.0) / aDist * m_RepulsParam;

  //m_Repulsion = Min(m_Repulsion, 1000.0);
  return m_Repulsion;
}

inline double CFRAlgorithm::calcAttraction(const double &aDist, const double &aIdealDist)
{
  if (0.0 == aIdealDist)
  {
    m_Attraction = INFINITY_NUM;
  }
  else 
    m_Attraction = powf(aDist, 2.0) / aIdealDist * m_AttractParam;

  m_Attraction = -m_Attraction;
  return m_Attraction;
}

inline double CFRAlgorithm::calcDistance(const SNodeCoordinate &aC1, const SNodeCoordinate &aC2)
{
  m_NodesDist = sqrtf(powf(aC1.m_X - aC2.m_X, 2.0) + powf(aC1.m_Y - aC2.m_Y, 2.0) + powf(aC1.m_Z - aC2.m_Z, 2.0));
  m_NodesDist = Max(INFINITESIMAL_NUM, m_NodesDist);
  
  return m_NodesDist;
}

double CFRAlgorithm::calcIdealDist(uint aNodeNum)
{
  if (0 == aNodeNum)
  {
    return 0.0;
  }
  m_IdealDist = sqrtf(WINDOW_HEIGHT * WINDOW_WIDTH / aNodeNum); 
  return m_IdealDist;
}

void CFRAlgorithm::calcMoveCoordinate(SNodeCoordinate &aMoveC, SNodeCoordinate &aJionForce)
{
  //使用降火算法，控制点移动距离;
  double dist = sqrtf(powf(aJionForce.m_X, 2.0) + powf(aJionForce.m_Y, 2.0) + powf(aJionForce.m_Z, 2.0));
  if (dist != Min(dist, m_Temperature))  //移动距离超出最大距离;
  {
    aJionForce.m_X = aJionForce.m_X / dist * m_Temperature;
    aJionForce.m_Y = aJionForce.m_Y / dist * m_Temperature;
    aJionForce.m_Z = aJionForce.m_Z / dist * m_Temperature;
  }
  
  aMoveC += aJionForce;  //在降火法后的合力基础上平移; 

  //平移后控制在屏幕内;
  aMoveC.m_X = Max(aMoveC.m_X, -WINDOW_WIDTH  / 2);
  aMoveC.m_X = Min(aMoveC.m_X,  WINDOW_WIDTH  / 2);
  aMoveC.m_Y = Max(aMoveC.m_Y, -WINDOW_HEIGHT / 2);
  aMoveC.m_Y = Min(aMoveC.m_Y,  WINDOW_HEIGHT / 2);
  aMoveC.m_Z = Max(aMoveC.m_Z, -WINDOW_HEIGHT / 2);
  aMoveC.m_Z = Min(aMoveC.m_Z,  WINDOW_HEIGHT / 2);
}

void CFRAlgorithm::calcJoinForce(const SNodeCoordinate &aBeForceC, const SNodeCoordinate &aC, const double aForce)
{
  if (0.0 == m_NodesDist) //两点重合;
  {
    srand(time(0));
    m_JionForce.m_X += (rand() % 10) / 10.0 * aForce * m_UnitDistance;  //在X轴上平移;
    m_JionForce.m_Y += (rand() % 10) / 10.0 * aForce * m_UnitDistance;  //在X轴上平移;
    m_JionForce.m_Z += (rand() % 10) / 10.0 * aForce * m_UnitDistance;  //在X轴上平移;
     
    return;
  }

  double cos_VX = (aBeForceC.m_X - aC.m_X) / m_NodesDist;   //两个点构成的向量与X轴的cos值;
  double cos_VY = (aBeForceC.m_Y - aC.m_Y) / m_NodesDist;   //两个点构成的向量与Y轴的cos值;
  double cos_VZ = (aBeForceC.m_Z - aC.m_Z) / m_NodesDist;   //两个点构成的向量与Z轴的cos值;

  m_JionForce.m_X += aForce*m_UnitDistance*cos_VX;          //X方向移动的距离;
  m_JionForce.m_Y += aForce*m_UnitDistance*cos_VY;          //Y方向移动的距离;
  m_JionForce.m_Z += aForce*m_UnitDistance*cos_VZ;          //Z方向移动的距离;
}

void CFRAlgorithm::cool()
{
  m_Temperature *= m_TempK;
  m_Temperature = Max(2, m_Temperature);
}

void CFRAlgorithm::slot_runAlgorithm(SParamPack aData)
{
  resetParameter(aData);
  FRAlgorithm(aData.m_5InOneParam);
}

void CFRAlgorithm::resetParameter(SParamPack &aData)
{
    m_UnitDistance = aData.m_DispMove;
    m_TempK        = aData.m_TempK;
    m_RepulsParam  = aData.m_RepulsParam;
    m_AttractParam = aData.m_AttractParam;
    m_JionForce.clear();
}