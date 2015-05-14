
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "DrawProperty.h"

class CGraphicEditorView;
class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	//CStatusBar m_wndStatusBar;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public :  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	CMFCToolBar		  m_wndDrawToolBar;		//좌측 그리기 툴 바
	CMFCReBar		  m_wndReBar;			//상단 메뉴바/표준툴바/그리기 속성 다이얼로그를 묶어줄 Rebar
	CToolTipCtrl	  m_wndToolTipCtrl;		//툴팁 컨트롤
public :
	CDrawProperty	  m_dlgDrawProperty;	//그리기 속성 다이얼로그
// 생성된 메시지 맵 함수
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


