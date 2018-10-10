// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// uartScopeView.cpp : CuartScopeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "uartScope.h"
#endif

#include "uartScopeDoc.h"
#include "uartScopeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuartScopeView

IMPLEMENT_DYNCREATE(CuartScopeView, CFormView)

BEGIN_MESSAGE_MAP(CuartScopeView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CuartScopeView 构造/析构

CuartScopeView::CuartScopeView()
	: CFormView(CuartScopeView::IDD)
{
	// TODO: 在此处添加构造代码

}

CuartScopeView::~CuartScopeView()
{
}

void CuartScopeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CuartScopeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CuartScopeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CuartScopeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CuartScopeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CuartScopeView 诊断

#ifdef _DEBUG
void CuartScopeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CuartScopeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CuartScopeDoc* CuartScopeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CuartScopeDoc)));
	return (CuartScopeDoc*)m_pDocument;
}
#endif //_DEBUG


// CuartScopeView 消息处理程序
