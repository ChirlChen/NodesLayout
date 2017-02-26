#include "NodeDataManager.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "debug_new.h"

const uint BUF_SIZE = 200;

CNodeDataManager::CNodeDataManager(void)
  : m_RandRoomSize(1),
    m_NodeCount(0),
    m_EdgeCount(0)
{
  m_vNodesInfo.clear();
  m_vEdgesInfo.clear();

  srand(time(0));
  for (int idx = 0; idx < 10; idx++)
  {
    SNodeInfo tempNodeInfo;
    tempNodeInfo.m_NodeID = idx;
    randomCoordinate(tempNodeInfo.m_Coordinate);
    
    m_vNodesInfo.push_back(tempNodeInfo);
  }
}

CNodeDataManager::~CNodeDataManager(void)
{
  CDegreeAlgorithm::getInstance()->destroyInstance();
}

bool CNodeDataManager::setEdgesNodesInfo(const std::vector<SNodesDegree> &p)
{
  CDegreeAlgorithm::getInstance()->calcGraphInfo(p, m_vEdgesInfo, m_EdgeCount);   //计算边信息;
  m_NodeCount = p.size();
  CDegreeAlgorithm::getInstance()->clearDegree();  

  m_RandRoomSize = m_NodeCount * 4;   //设置随机分布节点的空间大小;
  m_RandRoomSize = m_RandRoomSize > 800 ? 800 : m_RandRoomSize; //最大空间size = 800;
  return true;
}

bool CNodeDataManager::loadEdgesNodesInfo(const char *aPath)
{
  if (NULL == aPath)
  { return false;}

  clearNodesInfo();
  clearEdgesInfo();

  std::ifstream myFile;
  myFile.open(aPath, std::ifstream::in);
  if (myFile.fail())
  { return false;}

  char *Buf = new char[BUF_SIZE];
  memset(Buf, 0, BUF_SIZE);
  myFile.getline(Buf, BUF_SIZE);  //读取文件第一行，获取边数和节点数;
  
  char *pB = Buf;
  while ('\0' != *pB)
  {
    int charCount = 0;
    if ('N' == *pB)//节点数;
    {
      m_NodeCount = findTheFirstNumFromStr(pB, charCount); 
      pB += charCount + 2;
      charCount = 0;
    }
    else if ('E' == *pB)//边数;
    {
      m_EdgeCount = findTheFirstNumFromStr(pB, charCount);
      pB += charCount + 2;
    }
    else if (*pB >= 'A' && *pB <= 'z')  //数据格式出错;
    {
      return false;
    }
    ++pB;
  }
  delete []Buf;
  m_vEdgesInfo.resize(m_EdgeCount);
  
  int dataIndex = 0;
  while (!myFile.eof() && dataIndex < m_EdgeCount)  //依次读取所有边数据;
  {
    myFile >> m_vEdgesInfo[dataIndex].m_Nodes.m_Node1 >> m_vEdgesInfo[dataIndex].m_Nodes.m_Node2;
    m_vEdgesInfo[dataIndex].m_EdgeId = dataIndex;
    dataIndex++; 
  }
  myFile.close();  //读取结束，关闭文件;

  m_RandRoomSize = m_NodeCount * 4;   //设置随机分布节点的空间大小;
  m_RandRoomSize = m_RandRoomSize > 800 ? 800 : m_RandRoomSize; //最大空间size = 800;
  return true;
}

bool CNodeDataManager::saveNodesInfo(const SNodeInfo &aNodeInfo)
{
  QMutexLocker locker(&m_Mutex);
  return true;
}

std::vector<SNodeInfo> &CNodeDataManager::getNodesInfo()
{
  return m_vNodesInfo;
}

std::vector<SEdgeInfo> &CNodeDataManager::getEdgesInfo()
{
  return m_vEdgesInfo;
}

void CNodeDataManager::initNodesPostion()
{
  QMutexLocker locker(&m_Mutex);
  m_vNodesInfo.resize(m_NodeCount);

  m_vNodesInfo[0].m_NodeID = 1; //将第一个点初始化在原点;
//   m_vNodesInfo[1].m_NodeID = 2;    //用于调试的变量;
//   m_vNodesInfo[1].m_Coordinate.m_X = 1.0;
//   m_vNodesInfo[2].m_NodeID = 3;
//   m_vNodesInfo[2].m_Coordinate.m_Y = 1.0;
//   m_vNodesInfo[3].m_NodeID = 4;
//   m_vNodesInfo[3].m_Coordinate.m_Z = 1.0;
   
  for (int idx = 1; idx < m_NodeCount; ++idx)
  {
    m_vNodesInfo[idx].m_NodeID = idx + 1;
    randomCoordinate(m_vNodesInfo[idx].m_Coordinate);
  }
}

void CNodeDataManager::randomCoordinate(SNodeCoordinate &aNodeCoord) const
{
  aNodeCoord.m_X = static_cast<double>(rand() % m_RandRoomSize) / 10.0 - (m_RandRoomSize / 20.0);
  aNodeCoord.m_Y = static_cast<double>(rand() % m_RandRoomSize) / 10.0 - (m_RandRoomSize / 20.0);
  aNodeCoord.m_Z = static_cast<double>(rand() % m_RandRoomSize) / 10.0 - (m_RandRoomSize / 20.0);
}

int CNodeDataManager::findTheFirstNumFromStr(const char *aStr, int &aIndex) const
{
  if (NULL == aStr)
  { return -1;}
  
  char tempChar[10] = {'\0'};
  aIndex = 0; //跳过的字符数;

  while ('\0' != *aStr)
  {
    while (*aStr >= '0' && *aStr <= '9')
    {
      tempChar[aIndex] = *aStr;
      aIndex++;
      aStr++;
    }
    if ('\0' != tempChar[0])
    {
      tempChar[aIndex] = '\0';
      break;
    }
    aStr++;
  }
  
  return atoi(tempChar);
}

void CNodeDataManager::clearNodesInfo()
{
  m_NodeCount = 0;
  m_vNodesInfo.clear();
}

void CNodeDataManager::clearEdgesInfo()
{
  m_EdgeCount = 0;
  m_vEdgesInfo.clear();
}