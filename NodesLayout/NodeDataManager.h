#ifndef _NODEDATAMANAGER_H_
#define _NODEDATAMANAGER_H_

#include "MacroDef.h"
#include "SingleTon.h"
#include "StructDefine.h"
#include "DegreeAlgorithm.h"
#include <vector>
#include <QMutexLocker>

class CNodeDataManager : public CSingleTon<CNodeDataManager>
{
public:
  CNodeDataManager(void);
  ~CNodeDataManager(void);

  bool setEdgesNodesInfo(const std::vector<SNodesDegree> &p);//从度序列转换为节点边信息;
  bool loadEdgesNodesInfo(const char *aPath);               //从文件里面读取节点关系信息;
  bool saveNodesInfo(const SNodeInfo &aNodeInfo);           //往NodeInfoVector里面存数据;
  std::vector<SNodeInfo> &getNodesInfo();                   //取出NodeInfoVector;
  std::vector<SEdgeInfo> &getEdgesInfo();                   //取出NodeRelationPairVector;

  void initNodesPostion();                                  //初始化节点位置(随机产生节点位置);
  void clearNodesInfo();                                    //清除节点信息数据;
  void clearEdgesInfo();                                    //清除节点关系数据;

protected:
  void randomCoordinate(SNodeCoordinate &aNodeCoord) const;  //生成随机坐标;
  int findTheFirstNumFromStr(const char *aStr, int &aIndex) const;                  //取出aStr中的第一个数字字符并转为int;
private:
  std::vector<SEdgeInfo>          m_vEdgesInfo;           //用于存储所有节点关系对;
  std::vector<SNodeInfo>          m_vNodesInfo;           //用于存储节点自身信息;
  
  DEFSETGET(RandRoomSize, uint, private);                //随机初始化的空间大小;
  DEFSETGET(NodeCount, uint, private);                   //记录节点个数;   
  DEFSETGET(EdgeCount, uint, private);                   //记录边数量;     

  mutable QMutex                  m_Mutex;               //互斥量;
};


#endif
