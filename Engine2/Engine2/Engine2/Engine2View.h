
// Engine2View.h : CEngine2View ��Ľӿ�
//

#pragma once
#pragma comment(lib,"winmm.lib")
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include<CSLScope.h>
#include <CILAngularGauge.h>
#include<Mmsystem.h>
class CEngine2View : public CFormView
{
protected: // �������л�����
	CEngine2View();
	DECLARE_DYNCREATE(CEngine2View)

public:
	enum{ IDD = IDD_ENGINE2_FORM };

// ����
public:
	CEngine2Doc* GetDocument() const;

// ����
public:
	void EngineTest();
	void HydraulicTest();
	void CooperativeInput();
	void CooperativeOutput();
	void OutputRelation100();
	void OutputRelation90();
	void OutputRelation80();
	void OutputRelation70();
	void OutputRelation60();
	void OutputRelation50();
	void DataList();
	void GaugeToZero();

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CEngine2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CTSLScope     m_EngineScope;  
	CTSLScope     m_HydraulicScope;
	CTSLScope     m_InputScope;
	CTSLScope     m_OutputScope;
	CTILAngularGauge m_EngineSpeedGauge;   //ת�ٱ�
	CTILAngularGauge m_TurbineSpeedGauge;   //����ת�ٱ�
    CTILAngularGauge m_TurbineTorqueGauge;  //����ת��
	CTILAngularGauge m_EngineTorqueGauge; //������ת��



// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_CStaticEngine;
	CSliderCtrl m_cSliderThrottle;
	int m_vSliderThrottle;
	CEdit m_cEditThrottle;
	int m_vEditThrottle;
	afx_msg void OnNMCustomdrawSliderThrottle(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_cSliderLoad;
	double m_vSliderLoad;
	CEdit m_cEditLoad;
	double m_vEditLoad;
	afx_msg void OnNMCustomdrawSliderLoad(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStartengine();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nTimeId0;
	int m_nTimeId1;
	double m_Speed;
	double m_Torque;
	double m_Power;
	double m_PumpRatio;
	
	double m_SpecificFuelConsumption;
	int m_TranRatioNumber;
	double m_PumpTorque;
	// �ѳ�ʼ����־
	bool m_AllLoaded;
	double m_TranRatio;
	CStatic m_CStaticHydraulic;
	CStatic m_CStaticInput;
	CStatic m_CStaticOutput;
	CStatic m_CStaticEngineTorqueGauge;
	CStatic m_CStaticEngineSpeedGauge;
	CStatic m_CStaticTurbineTorqueGauge;
	CStatic m_CStaticTurbineSpeedGauge;
	afx_msg void OnBnClickedButtonStarthydraulic();
	bool m_StartEngine;
	bool m_StartHydraulic;
	bool m_StartCooperative;
	double m_TorqueRatio;
	double m_InputPumpTorque;
	double m_Efficiency;
	double m_TurbineSpeed;
	double i;//Һ�������ԭʼ���Ե�i
	double MTmax;

	
	
	
	afx_msg void OnBnClickedButtonCooperative();
	double m_TurbineTorque;
	double m_Power2;
	CEdit m_cEditTranRatio;
	double m_vEditTranRatio;
	double m_PumpSpeed;
	afx_msg void OnBnClickedButtonRecord();
	
	afx_msg void OnBnClickedClear();
	CEdit m_cEditMTmax;
	CEdit m_cEditdM;
	CEdit m_cEditdn;
	CEdit m_cEditPT1;
	CEdit m_cEditPT2;
	afx_msg void OnBnClickedButtonShow();
	bool m_HydraulicFinish;
	double m_SpecificFuelConsumption2;
	CListCtrl m_list;
	int counter;
	afx_msg void OnBnClickedButtonStop();
};

#ifndef _DEBUG  // Engine2View.cpp �еĵ��԰汾
inline CEngine2Doc* CEngine2View::GetDocument() const
   { return reinterpret_cast<CEngine2Doc*>(m_pDocument); }
#endif

