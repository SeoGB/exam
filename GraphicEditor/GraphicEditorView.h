
// GraphicEditorView.h : CGraphicEditorView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CGraphicEditorView();
	DECLARE_DYNCREATE(CGraphicEditorView)

// Ư���Դϴ�.
public:
	CGraphicEditorDoc* GetDocument() const;
	BOOL m_bsDrawMode;	//�׸�����
	BOOL m_bsSelectMode;//���� ���
	
	BOOL m_bsPolyDraw;	//�������� �׸��� ����
	BOOL m_bsDBClicked; //���� Ŭ�� ����
	CPoint m_sCurrPoint;//�������� �׸� �� ���� ���콺 ����

	BOOL m_bsTextDraw;	//�ؽ�Ʈ �׸��� ����
	BOOL m_bsMove;		//�̵� ���� ����
	BOOL m_bsResize;	//�������� ���� ����

	//�̵� �� ����� ����
	int m_nsSelectPtIndex;
	BOOL m_bsPointSelected;

	//�׷�ȭ�� ���� ����
	BOOL m_bsMakeFocusRect;		//�巡�� ����
	BOOL m_bsMultiSelectMode;	//���� ���� ����

	//�߶󳻱�/����/�ٿ��ֱ� �� ���
	static CLIPFORMAT m_cfsDraw; // custom Ŭ������ ����

	//�ܺ� ��� ����
	BOOL m_bExternalCmd;

	//FocusRect�� ���� �� ���
	CPoint	m_sSelectStartPt,
			m_sSelectEndPt;
	//View�� ������ ������ ���� ����
	CDC* m_psMemDC;	
	CBitmap *m_psBitmap, 
			*m_psOldBitmap;

	BrushPattern* m_dlgBrushPtn;
	LinePattern* m_dlgLinePtn;

// �۾��Դϴ�.
public:
	CRect MakeSelectRect(void);

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CGraphicEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // GraphicEditorView.cpp�� ����� ����
inline CGraphicEditorDoc* CGraphicEditorView::GetDocument() const
   { return reinterpret_cast<CGraphicEditorDoc*>(m_pDocument); }
#endif

