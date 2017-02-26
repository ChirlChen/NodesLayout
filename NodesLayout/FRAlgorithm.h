#ifndef _FRALGORITHM_H_
#define _FRALGORITHM_H_

#include <QObject>
#include "MacroDef.h"
#include "StructDefine.h"

class CFRAlgorithm : public QObject
{
  Q_OBJECT
public:
  CFRAlgorithm(QObject *parent = 0);
  ~CFRAlgorithm(void);

  void FRAlgorithm(const short aIterTimes = 100);
protected:
  double calcRepulsion(const double &aDist, const double &aIdealDist);    //计算两点间的斥力;
  double calcAttraction(const double &aDist, const double &aIdealDist);   //计算两点间的引力;
  double calcDistance(const SNodeCoordinate &aC1,                       //计算两点间的距离;
                      const SNodeCoordinate &aC2);                      

  double calcIdealDist(uint aNodeNum);                                  //计算理想距离;

  /*
  函 数 名：calcMoveCoordinate();
  参数说明：aC1为定点，aMoveC为需要移动的点，aJionForce为合力大小(+表示斥力，-表示引力);
  功    能：传入的aMoveC的坐标将直接被改为移动后的坐标,移动按照所受合力移动;
  */
  void calcMoveCoordinate(SNodeCoordinate &aMoveC,       
                          SNodeCoordinate &aJionForce);             
  
  /*
  函 数 名：calcJoinForce();
  函数说明：根据引力或斥力计算两点需要在该向量方向上的力，经过多次调用，计算该点受到的合力;
  参数说明：aBeForceC为受力点，aC为其他点，aForce为力大小(+表示斥力，-表示引力);
  功    能：传入的aMoveC的坐标将直接被改为移动后的坐标,移动方向为两个点构成的向量方向;
  */
  void calcJoinForce(const SNodeCoordinate &aBeForceC, 
                     const SNodeCoordinate &aC, 
                     const double aForce);

  void cool();                           //降火算法,降低温度系数;
  void resetParameter(SParamPack &aData);                 //重置参数;

protected slots:
  void slot_runAlgorithm(SParamPack aData);     //开始布局算法的接口槽;

signals:
  void signal_algoFinished(int);              //算法运行结束;
private:
  DEFSETGET(IterTimes, uint, private);         //算法迭代次数;
  DEFSETGET(IdealDist, double, private);       //理想距离(平衡距离);
  DEFSETGET(NodesDist, double, private);       //两点间的物理距离;
  DEFSETGET(Repulsion, double, private);       //斥力;
  DEFSETGET(Attraction, double, private);      //引力;
  DEFSETGET(UnitDistance, double, private);    //算法计算平移的单位距离;
  DEFSETGET(Temperature, double,  private);    //算法的温度系数;
  DEFSETGET(TempK, double, private);           //降火系数;
  DEFSETGET(JionForce, SNodeCoordinate, private);//一个点受到的合力;
  DEFSETGET(AttractParam, float, private);     //引力系数;
  DEFSETGET(RepulsParam, float, private);      //斥力系数;
};

#endif

