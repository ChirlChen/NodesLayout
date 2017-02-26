#ifndef _STRUCTDEF_H_
#define _STRUCTDEF_H_

/*类型定义*/
typedef unsigned int      uint;
typedef unsigned short    ushort;
typedef unsigned char     uchar;

/*全局变量/常量定义区*/
static double WINDOW_WIDTH  = 700.0 ;           //窗口宽度;
static double WINDOW_HEIGHT = 400.0 ;           //窗口高度;

const double INFINITY_NUM      = 100000.0;        //无穷大量;
const double INFINITESIMAL_NUM = 0.0000001;       //无穷小量;

/*枚举量定义区*/
enum EsBitContral    //位控制枚举,将一个short型数的各bit赋予含义;
{
  EBC_IS3Dims      = 0x0001,   //是否为三维（第1bit）：1-true,0-false;
  EBC_IsAlgStop    = 0x0002,   //是否暂停算法（第2bit）,没用;
  EBC_seeNodesMove = 0x0004,   //是否逐查看节点布局过程(第3bit);
  EBC_seeProcess   = 0x0008,   //是否逐过程查看布局过程（第4bit），一次迭代为一个过程;
  EBC_IterTimes    = 0x1FF0,   //迭代次数,从short的第5bit到13bit（0~511）;
                               //高3bit暂时预留;
};



/*结构体定义区*/
typedef struct _SNodeCoordinate  //节点的三维坐标;
{
  double m_X;
  double m_Y;
  double m_Z;

  _SNodeCoordinate()
  {
    m_X = m_Y = m_Z = 0.0;
  }

  void clear()
  {
    m_X = m_Y = m_Z = 0.0;
  }
  void operator+=(_SNodeCoordinate &aN)   //重载+=运算符;
  {
    m_X += aN.m_X;
    m_Y += aN.m_Y;
    m_Z += aN.m_Z;
  }
  void operator-=(_SNodeCoordinate &aN)
  {
    m_X -= aN.m_X;
    m_Y -= aN.m_Y;
    m_Z -= aN.m_Z;
  }
}SNodeCoordinate;

typedef struct _SNodeInfo         //存储节点自身的所有信息;
{
  SNodeCoordinate m_Coordinate;   //节点坐标;
  uint            m_NodeID;       //节点标签,节点的唯一标志ID;

  _SNodeInfo()
  {
    m_NodeID = 0;
  }
}SNodeInfo;

typedef struct _SNodeRelationPair //节点关系对,用于存储两个节点的关联关系（a号节点与b号节点有连接，则m_Node1 = a; m_Node2 = b）;
{
  uint m_Node1;                   //表示节点的ID;
  uint m_Node2;                   //表示节点的ID;

  _SNodeRelationPair()
  {
    m_Node1 = m_Node2 = 0;
  }
}SNodeRelationPair;

typedef struct _SEdgeInfo         //边信息;
{
  SNodeRelationPair m_Nodes;      //边的两个节点;
  uint              m_EdgeId;     //边的ID;
  double            m_EdgeLegth;  //边的长度; 
  _SEdgeInfo()
  {
    m_EdgeId = 0;
    m_EdgeLegth = 0.0;
  }
}SEdgeInfo;

typedef struct _SParamPack    //布局算法所需的参数包;
{
  short                 m_5InOneParam;    //5个参数整合;
  float                 m_AttractParam;   //引力系数;
  float                 m_RepulsParam;    //斥力系数;
  float                 m_TempK;          //降温系数;
  float                 m_DispMove;       //单位位移大小;
  _SParamPack()
  {
    clear();
  }
  void clear()
  {
    m_5InOneParam  = 0;
    m_RepulsParam  = 0.0;
    m_AttractParam = 0.0;
    m_TempK        = 0.0;
    m_DispMove     = 0.0;
  }
}SParamPack;

typedef struct _SNodesDegree                   //节点度信息;    
{
  uint            m_NodesID;            //节点ID;
  uint            m_NodesDegree;        //节点度数;

  _SNodesDegree()
  {
    m_NodesDegree = m_NodesID = 0;    
  }

  void operator=(const _SNodesDegree &p)
  {
    this->m_NodesDegree = p.m_NodesDegree;
    this->m_NodesID     = p.m_NodesID;
  }
}SNodesDegree;

// typedef struct _SRelationThread
// {
// 
// }SRealationThread;

#endif