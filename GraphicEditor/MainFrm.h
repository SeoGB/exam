
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "DrawProperty.h"

class CGraphicEditorView;
class CMainFrame : public CFrameWndEx
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	//CStatusBar m_wndStatusBar;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public :  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	CMFCToolBar		  m_wndDrawToolBar;		//���� �׸��� �� ��
	CMFCReBar		  m_wndReBar;			//��� �޴���/ǥ������/�׸��� �Ӽ� ���̾�α׸� ������ Rebar
	CToolTipCtrl	  m_wndToolTipCtrl;		//���� ��Ʈ��
public :
	CDrawProperty	  m_dlgDrawProperty;	//�׸��� �Ӽ� ���̾�α�
// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDrawpropertyLinecolor();
	afx_msg void OnUpdateDrawpropertyLinecolor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorTask(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorColor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorFillcolor(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveImage();
	BOOL GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
protected:
public:
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


