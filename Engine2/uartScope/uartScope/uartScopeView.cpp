// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// uartScopeView.cpp : CuartScopeView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CuartScopeView ����/����

CuartScopeView::CuartScopeView()
	: CFormView(CuartScopeView::IDD)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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


// CuartScopeView ���

#ifdef _DEBUG
void CuartScopeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CuartScopeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CuartScopeDoc* CuartScopeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CuartScopeDoc)));
	return (CuartScopeDoc*)m_pDocument;
}
#endif //_DEBUG


// CuartScopeView ��Ϣ�������
