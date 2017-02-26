#ifndef _DEGREEALGORITHM_H_
#define _DEGREEALGORITHM_H_

#include "SingleTon.h"
#include "StructDefine.h"
#include <vector>

class CDegreeAlgorithm : public CSingleTon<CDegreeAlgorithm>
{
public:
  CDegreeAlgorithm(void);
  ~CDegreeAlgorithm(void);

  bool isGraphSequence(std::vector<SNodesDegree> &p); //判断某度序列是否为图序列;
  void calcGraphInfo(const std::vector<SNodesDegree> &p,
                     std::vector<SEdgeInfo> &e,
                     uint &aEdgesNum);                   //从度信息（p）得到边信息（e）和节点数（aNodesCount）;
  void clearDegree();                                      //清除度信息;
  void degreeSort(std::vector<SNodesDegree> &p);           //对节点度信息从大到小排序;

private:
  void calcNextSequence(std::vector<SNodesDegree> &p);     //由传入的图序列计算下一等价序列;

private:
  std::vector<SNodesDegree>       m_CurNodesDegree;      //当前的节点度信息;
  uint                            m_NodesNum;            //节点数;
  uint                            m_EdgesNum;            //边数;
  
};


#endif
