
// Engine2View.cpp : CEngine2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Engine2.h"
#endif

#include "Engine2Doc.h"
#include "Engine2View.h"
#include<math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEngine2View

IMPLEMENT_DYNCREATE(CEngine2View, CFormView)

BEGIN_MESSAGE_MAP(CEngine2View, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_THROTTLE, &CEngine2View::OnNMCustomdrawSliderThrottle)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LOAD, &CEngine2View::OnNMCustomdrawSliderLoad)
	ON_BN_CLICKED(IDC_BUTTON_STARTENGINE, &CEngine2View::OnBnClickedButtonStartengine)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STARTHYDRAULIC, &CEngine2View::OnBnClickedButtonStarthydraulic)
	ON_BN_CLICKED(IDC_BUTTON_COOPERATIVE, &CEngine2View::OnBnClickedButtonCooperative)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CEngine2View::OnBnClickedButtonRecord)
	
	ON_BN_CLICKED(IDABORT, &CEngine2View::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CEngine2View::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CEngine2View::OnBnClickedButtonStop)
END_MESSAGE_MAP()

// CEngine2View 构造/析构

CEngine2View::CEngine2View()
	: CFormView(CEngine2View::IDD)
	, m_vSliderThrottle(0)
	, m_vEditThrottle(0)
	, m_vSliderLoad(0)
	, m_vEditLoad(0)
	, m_nTimeId0(1000)
	, m_nTimeId1(2000)
	, m_Speed(800)
	, m_Torque(0)
	, m_Power(0)
	
	, m_SpecificFuelConsumption(0)
	, m_TranRatioNumber(0)
	, m_TranRatio(0)
	, m_PumpTorque(0)
	, m_AllLoaded(false)
	,i(0)
	
	, m_StartEngine(false)
	, m_StartHydraulic(false)
	, m_TorqueRatio(0)
	, m_InputPumpTorque(0)
	, m_Efficiency(0)
	, m_TurbineSpeed(0)
	, m_StartCooperative(false)
	, m_TurbineTorque(0)
	, m_Power2(0)
	, m_vEditTranRatio(0)
	, m_PumpSpeed(0)
	, m_HydraulicFinish(false)
	, m_SpecificFuelConsumption2(0)
	, counter(0)
{
	// TODO: 在此处添加构造代码

}

CEngine2View::~CEngine2View()
{
}

void CEngine2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENGINECURVE, m_CStaticEngine);
	DDX_Control(pDX, IDC_SLIDER_THROTTLE, m_cSliderThrottle);
	DDX_Slider(pDX, IDC_SLIDER_THROTTLE, m_vSliderThrottle);
	DDV_MinMaxInt(pDX, m_vSliderThrottle, 1, 100);
	DDX_Control(pDX, IDC_EDIT_THROTTLE, m_cEditThrottle);
	DDX_Text(pDX, IDC_EDIT_THROTTLE, m_vEditThrottle);
	DDV_MinMaxInt(pDX, m_vEditThrottle, 1, 100);
	DDX_Control(pDX, IDC_SLIDER_LOAD, m_cSliderLoad);
	//DDX_Slider(pDX, IDC_SLIDER_LOAD, m_vSliderLoad);
	//DDV_MinMaxInt(pDX, m_vSliderLoad, 0,1000);
	DDX_Control(pDX, IDC_EDIT_LOAD, m_cEditLoad);
	DDX_Text(pDX, IDC_EDIT_LOAD, m_vEditLoad);
	DDV_MinMaxDouble(pDX, m_vEditLoad, 0,1000);
	DDX_Control(pDX, IDC_STATIC_HYDRAULIC, m_CStaticHydraulic);
	DDX_Control(pDX, IDC_INPUTCURVE, m_CStaticInput);
	DDX_Control(pDX, IDC_OUTPUTCURVE, m_CStaticOutput);
	DDX_Control(pDX, IDC_STATIC_GAUGE1, m_CStaticEngineTorqueGauge);
	DDX_Control(pDX, IDC_STATIC_GAUGE2, m_CStaticEngineSpeedGauge);
	DDX_Control(pDX, IDC_STATIC_GAUGE3, m_CStaticTurbineTorqueGauge);
	DDX_Control(pDX, IDC_STATIC_GAUGE4, m_CStaticTurbineSpeedGauge);
	DDX_Control(pDX, IDC_EDIT_TRANRATIO, m_cEditTranRatio);
	DDX_Text(pDX, IDC_EDIT_TRANRATIO, m_vEditTranRatio);
	DDV_MinMaxDouble(pDX, m_vEditTranRatio, 0, 1);
	DDX_Control(pDX, IDC_EDIT_MTMAX, m_cEditMTmax);
	DDX_Control(pDX, IDC_EDIT_DM, m_cEditdM);
	DDX_Control(pDX, IDC_EDIT_DN, m_cEditdn);
	DDX_Control(pDX, IDC_EDIT_PT1, m_cEditPT1);
	DDX_Control(pDX, IDC_EDIT_PT2, m_cEditPT2);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CEngine2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CEngine2View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_cSliderThrottle.SetRange(5,10,0);
	m_cSliderThrottle.SetPos(10);
	
	m_cSliderLoad.SetRange(1,600,0);
	m_cSliderLoad.SetLineSize( 1); 
	VCL_InitControls(m_hWnd);

	m_EngineScope.Open(m_CStaticEngine.m_hWnd);//发动机试验初始化
	m_EngineScope.Title.Text=_T("JX439Q1柴油机外特性曲线");
	m_EngineScope.Channels.Add(2);
	m_EngineScope.YAxis.AdditionalAxes.Add(2);
	m_EngineScope.Channels[0].YAxisIndex=0;
	m_EngineScope.Channels[1].YAxisIndex=1;
	m_EngineScope.Channels[2].YAxisIndex=2;
	m_EngineScope.Channels[0].Name=_T("Me(N·M)");
	m_EngineScope.Channels[1].Name=_T("Pe(kW)");
	m_EngineScope.Channels[2].Name=_T("ge(g/kW)");
	m_EngineScope.XAxis.AxisLabel.Text=_T("Speed n（r/min）");
	m_EngineScope.YAxis.AxisLabel.Text=_T("Me(N·M)");
	m_EngineScope.YAxis.AdditionalAxes[0].Axis.AxisLabel.Text=_T("Pe(kW)");
	m_EngineScope.YAxis.AdditionalAxes[1].Axis.AxisLabel.Text=_T("ge(g/kW)");

	

	m_HydraulicScope.Open(m_CStaticHydraulic.m_hWnd);//液力变矩器原始特性试验初始化
	m_HydraulicScope.Title.Text=_T("YJ375A液力变矩器原始特性曲线");
	m_HydraulicScope.Channels.Add(2);
	m_HydraulicScope.YAxis.AdditionalAxes.Add(2);
	m_HydraulicScope.Channels[0].YAxisIndex=0;
	m_HydraulicScope.Channels[1].YAxisIndex=1;
	m_HydraulicScope.Channels[2].YAxisIndex=2;
	m_HydraulicScope.Channels[0].Name=_T("K");
	m_HydraulicScope.Channels[1].Name=_T("η （%）");
	m_HydraulicScope.Channels[2].Name=_T("MBg(N·M)");
	m_HydraulicScope.XAxis.AxisLabel.Text=_T("i");
	m_HydraulicScope.YAxis.AxisLabel.Text=_T("K");
	m_HydraulicScope.YAxis.AdditionalAxes[0].Axis.AxisLabel.Text=_T("η （%）");
	m_HydraulicScope.YAxis.AdditionalAxes[1].Axis.AxisLabel.Text=_T("MBg(N·M)");
	
	
	m_HydraulicScope.Cursors.Add( 2 );
    m_HydraulicScope.Cursors[ 0].Position.X =0.3781 ;//高效区间标志
	m_HydraulicScope.Cursors[ 1].Position.X =0.8808 ;
	m_HydraulicScope.CursorLinks.Add(  );
	m_HydraulicScope.CursorLinks[ 0].BeginCursorIndex = 0;
	m_HydraulicScope.CursorLinks[ 0 ].EndCursorIndex = 1;
	m_HydraulicScope.CursorLinks[ 0 ].Name=_T("高效区间");

	m_InputScope.Open(m_CStaticInput.m_hWnd);//输入特性曲线初始化
	m_InputScope.Title.Text=_T("共同工作输入特性曲线");
	m_InputScope.XAxis.AxisLabel.Text=_T("n1(rpm)");
	m_InputScope.YAxis.AxisLabel.Text=_T("N·M");
	m_InputScope.Channels.Add(11);
	m_InputScope.Channels[0].Name=_T("Me(N·M)");
	m_InputScope.Channels[1].Name=_T(" i=0");
	m_InputScope.Channels[2].Name=_T(" i=0.1");
	m_InputScope.Channels[3].Name=_T(" i=0.2");
	m_InputScope.Channels[4].Name=_T(" i=0.3");
	m_InputScope.Channels[5].Name=_T(" i=0.4");
	m_InputScope.Channels[6].Name=_T(" i=0.5");
	m_InputScope.Channels[7].Name=_T(" i=0.6");
	m_InputScope.Channels[8].Name=_T(" i=0.7");
	m_InputScope.Channels[9].Name=_T(" i=0.8");
	m_InputScope.Channels[10].Name=_T(" i=0.9");
	m_InputScope.Channels[11].Name=_T(" i=0.95");
	m_InputScope.YAxis.Max.DataValue = 500;
	


	m_OutputScope.Open(m_CStaticOutput.m_hWnd);//输出特性曲线初始化
	m_OutputScope.Title.Text=_T("共同工作输出特性曲线");
	m_OutputScope.Channels.Add(4);
	m_OutputScope.YAxis.AdditionalAxes.Add(4);
	m_OutputScope.Channels[0].YAxisIndex=0;
	m_OutputScope.Channels[1].YAxisIndex=1;
	m_OutputScope.Channels[2].YAxisIndex=2;
	m_OutputScope.Channels[3].YAxisIndex=3;

	m_OutputScope.XAxis.AxisLabel.Text=_T("涡轮转速 n2(rpm)");
	m_OutputScope.YAxis.Visible=false;
	m_OutputScope.YAxis.AdditionalAxes[0].Axis.Visible=false;
	m_OutputScope.YAxis.AdditionalAxes[1].Axis.Visible=false;
	m_OutputScope.YAxis.AdditionalAxes[2].Axis.Visible=false;
	m_OutputScope.YAxis.AdditionalAxes[3].Axis.Visible=false;
	m_OutputScope.Channels[0].Name=_T("M1(N·M)");
	m_OutputScope.Channels[1].Name=_T("M2(N·M)");
	m_OutputScope.Channels[2].Name=_T("η （%）");
	m_OutputScope.Channels[3].Name=_T("ge2(g/kW)");
	m_OutputScope.Channels[4].Name=_T("P2(kW)");
	
	m_EngineSpeedGauge.Open(m_CStaticEngineSpeedGauge.m_hWnd);
	m_EngineSpeedGauge.Caption.Text=_T("发动机(泵轮)\n转速(rpm)");
	m_EngineTorqueGauge.Open(m_CStaticEngineTorqueGauge.m_hWnd);
	m_EngineTorqueGauge.Caption.Text=_T("净转矩\n(泵轮)(N·M)");
	m_TurbineSpeedGauge.Open(m_CStaticTurbineSpeedGauge.m_hWnd);
	m_TurbineSpeedGauge.Caption.Text=_T("涡轮\n转速(rpm)");
	m_TurbineTorqueGauge.Open(m_CStaticTurbineTorqueGauge.m_hWnd);
	m_TurbineTorqueGauge.Caption.Text=_T("涡轮\n转矩(N·M)");
	m_EngineSpeedGauge.Max=3000;
	m_EngineSpeedGauge.Min=800;

	m_EngineTorqueGauge.Max=800;
	m_EngineTorqueGauge.Min=0;

	m_TurbineTorqueGauge.Max=1000;
	m_TurbineTorqueGauge.Min=0;

	m_TurbineSpeedGauge.Min=0;
	m_TurbineSpeedGauge.Max=3000;


	VCL_Loaded();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
//列表初始化
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_list.SetBkColor(RGB(247,247,255));
	m_list.SetTextColor(RGB(0,0,255));
	m_list.SetTextBkColor(RGB(247,247,255));
	m_list.InsertColumn(0, _T("工况点 i"), LVCFMT_LEFT, 80);//表头
	m_list.InsertColumn(1, _T("泵轮转速 n1（rpm） "), LVCFMT_LEFT, 140);
	m_list.InsertColumn(2, _T("泵轮扭矩 M1(N·M)"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(3, _T("涡轮转速 n2（rpm）"), LVCFMT_LEFT, 140);
	m_list.InsertColumn(4, _T("涡轮扭矩 M2(N·M)"), LVCFMT_LEFT, 140);
	m_list.InsertColumn(5, _T("变矩器输出功率 P2(kW)"), LVCFMT_LEFT, 160);
	m_list.InsertColumn(6, _T("柴油机耗油率 ge2(g/kW)"), LVCFMT_LEFT, 160);
	m_list.InsertColumn(7, _T("油门开度 %"), LVCFMT_LEFT, 120);

	m_list.DeleteAllItems();
	m_AllLoaded=true;

}

void CEngine2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEngine2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEngine2View 诊断

#ifdef _DEBUG
void CEngine2View::AssertValid() const
{
	CFormView::AssertValid();
}

void CEngine2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEngine2Doc* CEngine2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEngine2Doc)));
	return (CEngine2Doc*)m_pDocument;
}
#endif //_DEBUG


// CEngine2View 消息处理程序


void CEngine2View::OnNMCustomdrawSliderThrottle(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_AllLoaded)
	{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int tempThrottle;
	tempThrottle=m_cSliderThrottle.GetPos();
	CString tempStr;
	tempStr.Format(_T("%d0%%"),tempThrottle);
    m_cEditThrottle.SetWindowText(tempStr);
	 m_vSliderThrottle=tempThrottle*10;
	}
}


void CEngine2View::OnNMCustomdrawSliderLoad(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	if(m_AllLoaded)
	{	
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(m_StartCooperative)
	{
		double tempLoad;
	tempLoad=m_cSliderLoad.GetPos();
	CString tempStr;
	tempStr.Format(_T("%4.1fN·M"),tempLoad);
	m_cEditLoad.SetWindowText(tempStr);

	
	m_vSliderLoad=tempLoad;
	
	switch(m_vSliderThrottle)
	{
	case 100:m_TranRatio=5.885e-9*pow(tempLoad,3)-4.023e-6*pow(tempLoad,2)-0.001701*tempLoad+1.164; 
		if((m_TranRatio<=1)&&(m_TranRatio>=0)) 
			OutputRelation100();break;
	case 90: m_TranRatio=7.612e-9*pow(tempLoad,3)-4.457e-6*pow(tempLoad,2)-0.002087*tempLoad+1.184;  
		if((m_TranRatio<=1)&&(m_TranRatio>=0)) 
			OutputRelation90();break;
	case 80: m_TranRatio=1.015e-8*pow(tempLoad,3)-4.812e-6*pow(tempLoad,2)-0.002652*tempLoad+1.211; 
		if((m_TranRatio<=1)&&(m_TranRatio>=0))
			OutputRelation80();  break;
	case 70: m_TranRatio=1.379e-8*pow(tempLoad,3)-4.777e-6*pow(tempLoad,2)-0.003526*tempLoad+1.245;  
		if((m_TranRatio<=1)&&(m_TranRatio>=0))
			OutputRelation70();  break;
	case 60: m_TranRatio=1.849e-8*pow(tempLoad,3)-3.382e-6*pow(tempLoad,2)-0.004965*tempLoad+1.292;  
		if((m_TranRatio<=1)&&(m_TranRatio>=0))
			OutputRelation60();  break;
	case 50: m_TranRatio=2.096e-8*pow(tempLoad,3)+2.654e-6*pow(tempLoad,2)-0.007537*tempLoad+1.357; 
		if((m_TranRatio<=1)&&(m_TranRatio>=0))
			OutputRelation50();  break;
	default:break;
	}
	if( m_TranRatio<=0.9)
		m_TranRatioNumber=static_cast<int>( m_TranRatio*10)%10; //获取负荷调节值
	else
		m_TranRatioNumber=10;
	CString tempStr1;
	tempStr1.Format(_T("%4.2f"), m_TranRatio);
	m_cEditTranRatio.SetWindowText(tempStr1);

	
	}
	
	}
	
}

	
void CEngine2View::OnBnClickedButtonStartengine()         //开始发动机速度特性试验
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(m_nTimeId0,100,0);
	m_StartEngine=true;
	m_StartHydraulic=false;
	m_StartCooperative=false;
	//PlaySound(_T("E:\\文档\\音效测试：发动机声效 - 伴奏.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(),SND_RESOURCE| SND_ASYNC|SND_LOOP);
}



void CEngine2View::OnTimer(UINT_PTR nIDEvent)                   //定时器开始，试验部分都通过定时器完成
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_StartEngine)
	EngineTest();
	if(m_StartHydraulic)
		HydraulicTest();
	
	

	CFormView::OnTimer(nIDEvent);
}


void CEngine2View::OnBnClickedButtonStarthydraulic()                //开始液力变矩器原始特性试验
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(m_nTimeId1,100,0);
	m_StartHydraulic=true;
	m_StartEngine=false;
	m_StartCooperative=false;
	//PlaySound(_T("E:\\文档\\音效测试：发动机声效 - 伴奏.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(),SND_RESOURCE| SND_ASYNC|SND_LOOP);
}

void CEngine2View::EngineTest()//发动机速度特性试验程序
{
	if(m_Speed<3000)
		{
			
	m_Torque=13.42+0.1492*m_Speed-0.000037*m_Speed*m_Speed;
	m_Power=-32.29+0.04947*m_Speed-0.000008*m_Speed*m_Speed;
	m_SpecificFuelConsumption=380.3-0.1311*m_Speed+0.000028*m_Speed*m_Speed;
	m_Speed++;
	}
	
	
	else
	{
		m_Speed=3000;
		m_StartEngine=false;
		m_Torque=13.42+0.1492*m_Speed-0.000037*m_Speed*m_Speed;
		m_Power=-32.29+0.04947*m_Speed-0.000008*m_Speed*m_Speed;
		m_SpecificFuelConsumption=380.3-0.1311*m_Speed+0.000028*m_Speed*m_Speed;
		
	}
	m_EngineScope.Channels[0].Data.AddXYPoint(m_Speed,m_Torque);
	m_EngineScope.Channels[1].Data.AddXYPoint(m_Speed,m_Power);
	m_EngineScope.Channels[2].Data.AddXYPoint(m_Speed,m_SpecificFuelConsumption);
	
	m_EngineSpeedGauge.Value=m_Speed;
	m_EngineTorqueGauge.Value=m_Torque;
	

}

void CEngine2View::HydraulicTest()//液力变矩器原始特性试验程序
{   
	if(m_TurbineSpeed<2000)
		{
		i=m_TurbineSpeed/2000;	
	m_TorqueRatio=-14.53*pow(i,5)+28.8*pow(i,4)-18.82*pow(i,3)
		+5.496*pow(i,2)-3.793*i+3.186;

	m_Efficiency=-914.6*pow(i,5)+1732*pow(i,4)-960.4*pow(i,3)
			-126.7*pow(i,2)+308.9*i-0.102;
	m_PumpTorque=-3.756e-14*pow(m_TurbineSpeed,5)+1.354e-10*pow(m_TurbineSpeed,4)
		-1.794e-7*pow(m_TurbineSpeed,3)+9.301e-5*pow(m_TurbineSpeed,2)-0.01067*m_TurbineSpeed+165.4;
	//m_PumpRatio=m_PumpTorque/(0.872*9.8*pow(375.0,5)*pow(2000.0,2));

	m_TurbineSpeed++;
	}
	
	
	else
	{
		m_TurbineSpeed=2000;
		i=1;
		m_StartHydraulic=false;
		m_HydraulicFinish=true;
		m_TorqueRatio=3.171-0.001867*m_TurbineSpeed+2.134E-7*m_TurbineSpeed*m_TurbineSpeed;
		m_Efficiency=-914.6*pow(i,5)+1732*pow(i,4)-960.4*pow(i,3)
			-126.7*pow(i,2)+308.9*i-0.102;
	}
	

	m_HydraulicScope.Channels[0].Data.AddXYPoint(i,m_TorqueRatio);
	m_HydraulicScope.Channels[1].Data.AddXYPoint(i,m_Efficiency);
	m_HydraulicScope.Channels[2].Data.AddXYPoint(i,m_PumpTorque);
	m_EngineSpeedGauge.Value=2000;
	m_EngineTorqueGauge.Value=m_PumpTorque;
	m_TurbineSpeedGauge.Value=m_TurbineSpeed;
	m_TurbineTorqueGauge.Value=m_PumpTorque*m_TorqueRatio;
	}

void CEngine2View::CooperativeInput()             //共同输入特性曲线
{
	double m_SpeedArray[2201];
	double m_TorqueArray[2201];
	double m_PumpSpeedArray[3001];
	double m_PumpTorqueArray[3001];


	for(int i=0;i<2201;i++)
	{
		m_SpeedArray[i]=800+i;
		m_TorqueArray[i]=(13.42+0.1492*m_SpeedArray[i]-0.000037*m_SpeedArray[i]*m_SpeedArray[i])*m_vSliderThrottle/100;
	}

	switch(m_TranRatioNumber)
	{
	case 0: //m_TranRatio=0.0;
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.1275E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 1 ://m_TranRatio=0.1; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.17E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 2 ://m_TranRatio=0.2;
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.175E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 3 ://m_TranRatio=0.3;
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.1875E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 4 ://m_TranRatio=0.4; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.195E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	
	case 5: //m_TranRatio=0.5; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.175E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 6 ://m_TranRatio=0.6; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=4.1E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 7 ://m_TranRatio=0.7; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=3.9375E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	
	case 8 ://m_TranRatio=0.8; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=3.65E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	
	case 9 ://m_TranRatio=0.9; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=2.825E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	case 10 ://m_TranRatio=0.95; 
		for(int i=0;i<3001;i++)
	{
		m_PumpSpeedArray[i]=i;
		m_PumpTorqueArray[i]=2.1125E-5*m_PumpSpeedArray[i]*m_PumpSpeedArray[i];
	}break;
	default:MessageBox(_T("Wrong"));break;
	}

	if((m_TranRatio>=0)&&(m_TranRatio<=1))
	{
	m_InputScope.Channels[0].Data.SetXYData(m_SpeedArray,m_TorqueArray,2201);
	m_InputScope.Channels[m_TranRatioNumber+1].Data.SetXYData(m_PumpSpeedArray,m_PumpTorqueArray,3001);
	}
}

	
	
void CEngine2View::CooperativeOutput()  
{ 

	bool m_Limit;
	m_Limit=true;
	switch(m_vSliderThrottle)
	{
	case 100:if(!(m_vSliderLoad<537)) 
			 {MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	case 90:if(!(m_vSliderLoad<477)) 
			{MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	case 80:if(!(m_vSliderLoad<419)) 
			{MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	case 70:if(!(m_vSliderLoad<359)) 
			{MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	case 60:if(!(m_vSliderLoad<298))
			{MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	case 50:if(!(m_vSliderLoad<236)) 
			{MessageBox(_T("The Load is beyond the limitation of the device"),MB_OK);m_Limit=false;}break;
	default:break;
	}
		if(m_Limit)
	{
		m_OutputScope.Channels[0].ChannelMode=cmPoint;
		m_OutputScope.Channels[0].Points.Visible=true;
		m_OutputScope.Channels[1].ChannelMode=cmPoint;
		m_OutputScope.Channels[1].Points.Visible=true;
		m_OutputScope.Channels[2].ChannelMode=cmPoint;
		m_OutputScope.Channels[2].Points.Visible=true;
		m_OutputScope.Channels[3].ChannelMode=cmPoint;
		m_OutputScope.Channels[3].Points.Visible=true;
		m_OutputScope.Channels[4].ChannelMode=cmPoint;
		m_OutputScope.Channels[4].Points.Visible=true;
		m_OutputScope.Channels[0].Data.AddXYPoint(m_TurbineSpeed,m_PumpTorque);
		m_OutputScope.Channels[1].Data.AddXYPoint(m_TurbineSpeed,m_TurbineTorque);
		m_OutputScope.Channels[2].Data.AddXYPoint(m_TurbineSpeed,m_Efficiency);
		m_OutputScope.Channels[3].Data.AddXYPoint(m_TurbineSpeed,m_SpecificFuelConsumption2);
		m_OutputScope.Channels[4].Data.AddXYPoint(m_TurbineSpeed,m_Power2);

		m_OutputScope.Channels[0].ChannelMode=cmLine;
		m_OutputScope.Channels[0].Points.Visible=true;
		m_OutputScope.Channels[1].ChannelMode=cmLine;
		m_OutputScope.Channels[1].Points.Visible=true;
		m_OutputScope.Channels[2].ChannelMode=cmLine;
		m_OutputScope.Channels[2].Points.Visible=true;
		m_OutputScope.Channels[3].ChannelMode=cmLine;
		m_OutputScope.Channels[3].Points.Visible=true;
		m_OutputScope.Channels[4].ChannelMode=cmLine;
		m_OutputScope.Channels[4].Points.Visible=true;
		m_OutputScope.Channels[0].Data.AddXYPoint(m_TurbineSpeed,m_PumpTorque);
		m_OutputScope.Channels[1].Data.AddXYPoint(m_TurbineSpeed,m_TurbineTorque);
		m_OutputScope.Channels[2].Data.AddXYPoint(m_TurbineSpeed,m_Efficiency);
		m_OutputScope.Channels[3].Data.AddXYPoint(m_TurbineSpeed,m_SpecificFuelConsumption2);
		m_OutputScope.Channels[4].Data.AddXYPoint(m_TurbineSpeed,m_Power2);

		DataList();
	   
		}
	

	

}

void CEngine2View::OnBnClickedButtonCooperative()         //点击开始共同工作试验
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (m_HydraulicFinish)
	{m_StartCooperative=true;
	m_StartEngine=false;
	m_StartHydraulic=false;
	MessageBox(_T("请先选择油门开度，再调节负载进行试验"),MB_OK);
	//PlaySound(_T("E:\\文档\\音效测试：发动机声效 - 伴奏.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(),SND_RESOURCE| SND_ASYNC|SND_LOOP);
	}
	else
		MessageBox(_T("请先进行液力变矩器特性试验"),MB_OK);

	
}
void CEngine2View::OutputRelation100()  
{
		 m_TurbineTorque=m_vSliderLoad;//关于在100%油门负载的函数模型
		 m_PumpSpeed=3.408e4*pow(m_TranRatio,7)-8.25e4*pow(m_TranRatio,6)+7.478e4*pow(m_TranRatio,5)
			-2.932e4*pow(m_TranRatio,4)+3855*pow(m_TranRatio,3)+421.6*pow(m_TranRatio,2)-157.7*m_TranRatio+1992+(rand()-16000)%6;
		 m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_PumpSpeed=7.391e-15*pow( m_TurbineTorque,7)-1.711e-11*pow( m_TurbineTorque,6)+1.627e-8*pow(m_TurbineTorque,5)
			//-8.06e-6*pow(m_TurbineTorque,4)+0.002153*pow(m_TurbineTorque,3)-0.273*pow(m_TurbineTorque,2)+6.045*m_TurbineTorque+3228;
		//m_TurbineSpeed=1.69e4*pow(m_TranRatio,5)-3.271e4*pow(m_TranRatio,4)+2.262e4*pow(m_TranRatio,3)
			//-6388*pow(m_TranRatio,2)+2580*m_TranRatio-3.78;

	
		m_Power2=2.974+0.03609* m_TurbineSpeed-1.1e-5* m_TurbineSpeed* m_TurbineSpeed;
		
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=8.334e-16*pow(m_TurbineSpeed,5)-3.245e-12*pow(m_TurbineSpeed,4)
			+3.364e-9*pow(m_TurbineSpeed,3)-1.662e-6*pow(m_TurbineSpeed,2)+0.0008743*m_TurbineSpeed+230.3;

			m_TorqueRatio=0.0729+0.005655*m_TurbineTorque+1E-6*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;
		
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}
void CEngine2View::OutputRelation90()
{
		 m_TurbineTorque=m_vSliderLoad;//关于在90%油门负载的函数模型
		m_PumpSpeed=3.357e4*pow(m_TranRatio,7)-7.93e4*pow(m_TranRatio,6)+6.858e4*pow(m_TranRatio,5)
			-2.397e4*pow(m_TranRatio,4)+1610*pow(m_TranRatio,3)+850*pow(m_TranRatio,2)-186.5*m_TranRatio+1886+(rand()-16000)%6;
		 	m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_TurbineSpeed=1.729e4*pow(m_TranRatio,5)-3.353e4*pow(m_TranRatio,4)+2.323e4*pow(m_TranRatio,3)
			//-6571*pow(m_TranRatio,2)+2493*m_TranRatio-3.913;
		m_Power2=2.70+0.03148* m_TurbineSpeed-1e-5* m_TurbineSpeed* m_TurbineSpeed;
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=1.159e-15*pow(m_TurbineSpeed,5)-4.489e-12*pow(m_TurbineSpeed,4)
			+4.449e-9*pow(m_TurbineSpeed,3)-2.188e-6*pow(m_TurbineSpeed,2)+0.0012*m_TurbineSpeed+230.2;

		m_TorqueRatio=0.3985+0.006522*m_TurbineTorque+0*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;
		
		
	
	//m_PumpSpeed=-1.208e-10*pow(m_TurbineTorque,5)+3.671e-7*pow(m_TurbineTorque,4)
	//		-0.0003611*pow(m_TurbineTorque,3)+0.1593*pow(m_TurbineTorque,2)-32.68*m_TurbineTorque+4410;	
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}
void CEngine2View::OutputRelation80()
{
		 m_TurbineTorque=m_vSliderLoad;//关于在80%油门负载的函数模型
		m_PumpSpeed=3.756e4*pow(m_TranRatio,7)-9.148e4*pow(m_TranRatio,6)+8.327e4*pow(m_TranRatio,5)
			-3.282e4*pow(m_TranRatio,4)+4375*pow(m_TranRatio,3)+434.1*pow(m_TranRatio,2)-162.7*m_TranRatio+1770+(rand()-16000)%6;
		m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_TurbineSpeed=1.772e4*pow(m_TranRatio,5)-3.446e4*pow(m_TranRatio,4)+2.391e4*pow(m_TranRatio,3)
			//-6768*pow(m_TranRatio,2)+2395*m_TranRatio-4.043;
		m_Power2=2.391+0.02686* m_TurbineSpeed-0.9e-5* m_TurbineSpeed* m_TurbineSpeed;
		
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=1.68e-15*pow(m_TurbineSpeed,5)-6.383e-12*pow(m_TurbineSpeed,4)
			+6.019e-9*pow(m_TurbineSpeed,3)-2.926e-6*pow(m_TurbineSpeed,2)+0.00159*m_TurbineSpeed+231;

		m_TorqueRatio=0.00056+0.007702*m_TurbineTorque+0*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;
		
		//m_PumpSpeed=-4.456e-10*pow(m_TurbineTorque,5)+9.168e-7*pow(m_TurbineTorque,4)
			//-0.000697*pow(m_TurbineTorque,3)+0.2503*pow(m_TurbineTorque,2)-42.9*m_TurbineTorque+4583;
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}
void CEngine2View::OutputRelation70()
{
		m_TurbineTorque=m_vSliderLoad;//关于在70%油门负载的函数模型
		m_PumpSpeed=4.184e4*pow(m_TranRatio,7)-1.048e5*pow(m_TranRatio,6)+9.971e4*pow(m_TranRatio,5)
			-4.294e4*pow(m_TranRatio,4)+7604*pow(m_TranRatio,3)-62.42*pow(m_TranRatio,2)-133.2*m_TranRatio+1640+(rand()-16000)%6;
		m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_TurbineSpeed=1.803e4*pow(m_TranRatio,5)-3.514e4*pow(m_TranRatio,4)+2.441e4*pow(m_TranRatio,3)
		//	-6918*pow(m_TranRatio,2)+2280*m_TranRatio-4.145;
		m_Power2=2.047+0.02228* m_TurbineSpeed-7e-6* m_TurbineSpeed* m_TurbineSpeed;
		
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=2.5e-15*pow(m_TurbineSpeed,5)-9.052e-12*pow(m_TurbineSpeed,4)
			+7.729e-9*pow(m_TurbineSpeed,3)-3.543e-6*pow(m_TurbineSpeed,2)+0.001979*m_TurbineSpeed+234.1;

		m_TorqueRatio=0.00988+0.008707*m_TurbineTorque+3E-6*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;

		//m_PumpSpeed=-1.597e-9*pow(m_TurbineTorque,5)+2.455e-6*pow(m_TurbineTorque,4)
			//-0.001458*pow(m_TurbineTorque,3)+0.42*pow(m_TurbineTorque,2)-58.77*m_TurbineTorque+4826;
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}
void CEngine2View::OutputRelation60()
{
		 m_TurbineTorque=m_vSliderLoad;//关于在60%油门负载的函数模型
		m_PumpSpeed=4.626e4*pow(m_TranRatio,7)-1.189e5*pow(m_TranRatio,6)+1.174e5*pow(m_TranRatio,5)
			-5.404e4*pow(m_TranRatio,4)+1.121e4*pow(m_TranRatio,3)-628.7*pow(m_TranRatio,2)-98.2*m_TranRatio+1495+(rand()-16000)%6;
		m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_TurbineSpeed=1.819e4*pow(m_TranRatio,5)-3.554e4*pow(m_TranRatio,4)+2.473e4*pow(m_TranRatio,3)
			//-7014*pow(m_TranRatio,2)+2145*m_TranRatio-4.218;
		m_Power2=1.672+0.01779* m_TurbineSpeed-6e-6* m_TurbineSpeed* m_TurbineSpeed;
		//m_Efficiency=11.56+0.1319 *m_TurbineSpeed-5.5e-5*m_TurbineSpeed* m_TurbineSpeed;
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=3.877e-15*pow(m_TurbineSpeed,5)-1.3e-11*pow(m_TurbineSpeed,4)+
			9.477e-9*pow(m_TurbineSpeed,3)-3.898e-6*pow(m_TurbineSpeed,2)+0.00235*m_TurbineSpeed+244.9;

		m_TorqueRatio=0.01045+0.01188*m_TurbineTorque-2E-6*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;
	
		//m_PumpSpeed=-6.356e-9*pow(m_TurbineTorque,5)+7.437e-6*pow(m_TurbineTorque,4)
			//-0.003416*pow(m_TurbineTorque,3)+0.77*pow(m_TurbineTorque,2)-85.16*m_TurbineTorque+5178;
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}
void CEngine2View::OutputRelation50()
{
		m_TurbineTorque=m_vSliderLoad;//关于在50%油门负载的函数模型
		m_PumpSpeed=4.626e4*pow(m_TranRatio,7)-1.189e5*pow(m_TranRatio,6)+1.174e5*pow(m_TranRatio,5)
			-5.404e4*pow(m_TranRatio,4)+1.121e4*pow(m_TranRatio,3)-628.7*pow(m_TranRatio,2)-98.2*m_TranRatio+1495+(rand()-16000)%6;
		m_TurbineSpeed=m_PumpSpeed*m_TranRatio;
		//m_TurbineSpeed=1.808e4*pow(m_TranRatio,5)-3.543e4*pow(m_TranRatio,4)+2.47e4*pow(m_TranRatio,3)
			//-7012*pow(m_TranRatio,2)+1983*m_TranRatio-4.236;
		m_Power2=1.279+0.01350* m_TurbineSpeed-5e-6* m_TurbineSpeed* m_TurbineSpeed;
		//m_Efficiency=12.38+0.1437 *m_TurbineSpeed-6.6e-5*m_TurbineSpeed* m_TurbineSpeed;
		m_Efficiency=-914.6*pow(m_TranRatio,5)+1732*pow(m_TranRatio,4)-960.4*pow(m_TranRatio,3)
			-126.7*pow(m_TranRatio,2)+308.9*m_TranRatio-0.102;
		m_SpecificFuelConsumption2=6.411e-15*pow(m_TurbineSpeed,5)-1.919e-11*pow(m_TurbineSpeed,4)
			+1.094e-8*pow(m_TurbineSpeed,3)-3.749e-6*pow(m_TurbineSpeed,2)+0.002692*m_TurbineSpeed+260.4;

		m_TorqueRatio=-0.1758+0.01589*m_TurbineTorque-7E-6*m_TurbineTorque*m_TurbineTorque;
		m_PumpTorque= m_TurbineTorque/m_TorqueRatio+(rand()-16000)%10*0.1;

		//m_PumpSpeed=-3.074e-8*pow(m_TurbineTorque,5)+2.696e-5*pow(m_TurbineTorque,4)
			//-0.009308*pow(m_TurbineTorque,3)+1.582*pow(m_TurbineTorque,2)-132.5*m_TurbineTorque+5692;
		
		m_EngineSpeedGauge.Value=m_PumpSpeed;   //转速表
		m_TurbineSpeedGauge.Value=m_TurbineSpeed;   //涡轮转速标
		m_TurbineTorqueGauge.Value=m_TurbineTorque;  //涡轮转矩
		m_EngineTorqueGauge.Value=m_PumpTorque;     //发动机/泵轮转矩

}

void CEngine2View::OnBnClickedButtonRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CooperativeInput();
	CooperativeOutput();
	if(m_vSliderThrottle==100)
	{
		if(m_TranRatio<0.01)
	MTmax=m_TurbineTorque;
	}
	else MTmax=534.00+(rand()-16000)%10*0.1;
	
}




void CEngine2View::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputScope.Channels[ 0 ].Data.Clear();
	m_InputScope.Hold = false;
	m_OutputScope.Channels[ 0 ].Data.Clear();
	m_OutputScope.Channels[ 1 ].Data.Clear();
	m_OutputScope.Channels[ 2 ].Data.Clear();
	m_OutputScope.Channels[ 3 ].Data.Clear();
	m_OutputScope.Channels[ 4 ].Data.Clear();
	m_OutputScope.Hold = false;
	m_list.DeleteAllItems();
	counter=0;
}


void CEngine2View::OnBnClickedButtonShow()
{
	// TODO: 在此添加控件通知处理程序代码
	
	double dM,dn,PT1,PT2;
	//MTmax=534.00+(rand()-16000)%10*0.1;
	dM=2.3077+(rand()-16000)%6*0.01;
	dn=2.6844+(rand()-16000)%6*0.01;
	PT1=24.1090+(rand()-16000)%10*0.01;
	PT2=30.4292+(rand()-16000)%10*0.01;

	CString tempStr1, tempStr2, tempStr3, tempStr4, tempStr5;
	tempStr1.Format(_T("%5.2f"),MTmax);
	m_cEditMTmax.SetWindowText(tempStr1);

	tempStr2.Format(_T("%5.4f"),dM);
	m_cEditdM.SetWindowText(tempStr2);

	tempStr3.Format(_T("%5.4f"),dn);
	m_cEditdn.SetWindowText(tempStr3);

	tempStr4.Format(_T("%5.4f"),PT1);
	m_cEditPT1.SetWindowText(tempStr4);

	tempStr5.Format(_T("%5.4f"),PT2);
	m_cEditPT2.SetWindowText(tempStr5);
}

void CEngine2View::DataList()
{
	CString TranRatio,PumpSpeed,PumpTorque,TurbineSpeed,TurbineTorque;
	CString Power2,ge2,Throttle;
	TranRatio.Format(_T("%4.2f"),m_TranRatio);
	PumpSpeed.Format(_T("%4.2f"),m_PumpSpeed);
	PumpTorque.Format(_T("%4.2f"),m_PumpTorque);
	TurbineSpeed.Format(_T("%4.2f"),m_TurbineSpeed);
	TurbineTorque.Format(_T("%4.2f"),m_TurbineTorque);
	Power2.Format(_T("%4.2f"),m_Power2);
	ge2.Format(_T("%4.2f"),m_SpecificFuelConsumption2);
	Throttle.Format(_T("%d"),m_vSliderThrottle);
	
	m_list.InsertItem(counter,TranRatio);
	m_list.SetItemText(counter,1,PumpSpeed);
	m_list.SetItemText(counter,2,PumpTorque);
	m_list.SetItemText(counter,3,TurbineSpeed);
	m_list.SetItemText(counter,4,TurbineTorque);
	m_list.SetItemText(counter,5,Power2);
	m_list.SetItemText(counter,6,ge2);
	m_list.SetItemText(counter,7,Throttle);
	counter++;

}
void CEngine2View::GaugeToZero()
{
	m_EngineSpeedGauge.Value=0;   //转速表
	m_TurbineSpeedGauge.Value=0;   //涡轮转速标
	m_TurbineTorqueGauge.Value=0;  //涡轮转矩
	m_EngineTorqueGauge.Value=0;     //发动机/泵轮转矩
}

void CEngine2View::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EngineSpeedGauge.Value=0;   //转速表
	m_TurbineSpeedGauge.Value=0;   //涡轮转速标
	m_TurbineTorqueGauge.Value=0;  //涡轮转矩
	m_EngineTorqueGauge.Value=0;     //发动机/泵轮转矩
PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
	 m_StartEngine=false;
	m_StartHydraulic=false;
}
