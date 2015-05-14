
// GraphicEditorView.h : CGraphicEditorView 클래스의 인터페이스
//

#pragma once
#include "GObject.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GRectangle.h"
#include "GEllipse.h"
#include "GText.h"
#include "GGroup.h"
#include "DrawPropertyValue.h"
#include "BrushPattern.h"
#include "LinePattern.h"

class CGraphicEditorDoc;
class CMainFrame;
class CGraphicEditorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGraphicEditorView();
	DECLARE_DYNCREATE(CGraphicEditorView)

// 특성입니다.
public:
	CGraphicEditorDoc* GetDocument() const;
	BOOL m_bsDrawMode;	//그리기모드
	BOOL m_bsSelectMode;//선택 모드
	
	BOOL m_bsPolyDraw;	//폴리라인 그리기 상태
	BOOL m_bsDBClicked; //더블 클릭 상태
	CPoint m_sCurrPoint;//폴리라인 그릴 때 현재 마우스 지점

	BOOL m_bsTextDraw;	//텍스트 그리기 상태
	BOOL m_bsMove;		//이동 상태 여부
	BOOL m_bsResize;	//리사이즈 상태 여부

	//이동 시 사용할 변수
	int m_nsSelectPtIndex;
	BOOL m_bsPointSelected;

	//그룹화를 위한 변수
	BOOL m_bsMakeFocusRect;		//드래그 여부
	BOOL m_bsMultiSelectMode;	//다중 선택 여부

	//잘라내기/복사/붙여넣기 시 사용
	static CLIPFORMAT m_cfsDraw; // custom 클립보드 포맷

	//외부 명령 여부
	BOOL m_bExternalCmd;

	//FocusRect로 선택 시 사용
	CPoint	m_sSelectStartPt,
			m_sSelectEndPt;
	//View의 깜빡임 방지를 위한 변수
	CDC* m_psMemDC;	
	CBitmap *m_psBitmap, 
			*m_psOldBitmap;

	BrushPattern* m_dlgBrushPtn;
	LinePattern* m_dlgLinePtn;

// 작업입니다.
public:
	CRect MakeSelectRect(void);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CGraphicEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnDrawtoolSelect();
	afx_msg void OnDrawtoolLine();
	afx_msg void OnDrawtoolPolyline();
	afx_msg void OnDrawtoolRectangle();
	afx_msg void OnDrawtoolEllipse();
	afx_msg void OnDrawtoolText();

	afx_msg void OnUpdateDrawtoolSelect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawtoolLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawtoolPolyline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawtoolRectangle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawtoolEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawtoolText(CCmdUI *pCmdUI);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnObjDelete();
	afx_msg void OnUpdateObjDelete(CCmdUI *pCmdUI);
	afx_msg void OnDrawpropertyLinecolor();
	afx_msg void OnUpdateDrawpropertyLinecolor(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnDrawpropertyLinepattern();
	afx_msg void OnUpdateDrawpropertyLinepattern(CCmdUI *pCmdUI);
	afx_msg void OnEditGroup();
	afx_msg void OnUpdateEditGroup(CCmdUI *pCmdUI);
	afx_msg void OnEditUngroup();
	afx_msg void OnUpdateEditUngroup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnDrawpropertyBrushpattern();
	afx_msg void OnUpdateDrawpropertyBrushpattern(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	void DrawProperyControlSetting(GraphicalObjectType type);
	afx_msg void OnDrawpropertyFont();
	afx_msg void OnUpdateDrawpropertyFont(CCmdUI *pCmdUI);
protected:
	afx_msg LRESULT OnChangeAlpha(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLineJoin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLineWeight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLineCapStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLineCapEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLineColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeFillColor(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void DrawPropertyInfoSetting(void);
protected:
	afx_msg LRESULT OnChangeGrdtColor(WPARAM wParam, LPARAM lParam);
public:
	void InitializeAllVariable(void);
};

#ifndef _DEBUG  // GraphicEditorView.cpp의 디버그 버전
inline CGraphicEditorDoc* CGraphicEditorView::GetDocument() const
   { return reinterpret_cast<CGraphicEditorDoc*>(m_pDocument); }
#endif

