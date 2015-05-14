
// GraphicEditorView.cpp : CGraphicEditorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GraphicEditor.h"
#endif

#include "GraphicEditorDoc.h"
#include "GraphicEditorView.h"
#include "DrawPropertyValue.h"
#include "MainFrm.h"
#include "LinePattern.h"
#include "BrushPattern.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 영역에 대한 8개의 좌표를 열거
enum POSITION_LIST {TOPLEFT, LEFT, BOTTOMLEFT, BOTTOM, BOTTOMRIGHT,
	RIGHT, TOPRIGHT, TOP};

CLIPFORMAT CGraphicEditorView::m_cfsDraw = (CLIPFORMAT)
	::RegisterClipboardFormat(_T("GraphicEditor"));

// CGraphicEditorView

IMPLEMENT_DYNCREATE(CGraphicEditorView, CView)

BEGIN_MESSAGE_MAP(CGraphicEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DRAWTOOL_SELECT, &CGraphicEditorView::OnDrawtoolSelect)
	ON_COMMAND(ID_DRAWTOOL_LINE, &CGraphicEditorView::OnDrawtoolLine)
	ON_COMMAND(ID_DRAWTOOL_POLYLINE, &CGraphicEditorView::OnDrawtoolPolyline)
	ON_COMMAND(ID_DRAWTOOL_RECTANGLE, &CGraphicEditorView::OnDrawtoolRectangle)
	ON_COMMAND(ID_DRAWTOOL_ELLIPSE, &CGraphicEditorView::OnDrawtoolEllipse)
	ON_COMMAND(ID_DRAWTOOL_TEXT, &CGraphicEditorView::OnDrawtoolText)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_SELECT, &CGraphicEditorView::OnUpdateDrawtoolSelect)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_LINE, &CGraphicEditorView::OnUpdateDrawtoolLine)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_POLYLINE, &CGraphicEditorView::OnUpdateDrawtoolPolyline)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_RECTANGLE, &CGraphicEditorView::OnUpdateDrawtoolRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_ELLIPSE, &CGraphicEditorView::OnUpdateDrawtoolEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAWTOOL_TEXT, &CGraphicEditorView::OnUpdateDrawtoolText)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_COMMAND(ID_OBJ_DELETE, &CGraphicEditorView::OnObjDelete)
	ON_UPDATE_COMMAND_UI(ID_OBJ_DELETE, &CGraphicEditorView::OnUpdateObjDelete)
	ON_COMMAND(ID_EDIT_CUT, &CGraphicEditorView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CGraphicEditorView::OnEditPaste)
	ON_COMMAND(ID_DRAWPROPERTY_LINEPATTERN, &CGraphicEditorView::OnDrawpropertyLinepattern)
	ON_UPDATE_COMMAND_UI(ID_DRAWPROPERTY_LINEPATTERN, &CGraphicEditorView::OnUpdateDrawpropertyLinepattern)
	ON_COMMAND(ID_EDIT_COPY, &CGraphicEditorView::OnEditCopy)
	ON_COMMAND(ID_EDIT_GROUP, &CGraphicEditorView::OnEditGroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GROUP, &CGraphicEditorView::OnUpdateEditGroup)
	ON_COMMAND(ID_EDIT_UNGROUP, &CGraphicEditorView::OnEditUngroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNGROUP, &CGraphicEditorView::OnUpdateEditUngroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CGraphicEditorView::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CGraphicEditorView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CGraphicEditorView::OnUpdateEditPaste)
	ON_COMMAND(ID_DRAWPROPERTY_BRUSHPATTERN, &CGraphicEditorView::OnDrawpropertyBrushpattern)
	ON_UPDATE_COMMAND_UI(ID_DRAWPROPERTY_BRUSHPATTERN, &CGraphicEditorView::OnUpdateDrawpropertyBrushpattern)
	ON_COMMAND(ID_EDIT_UNDO, &CGraphicEditorView::OnEditUndo)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_REDO, &CGraphicEditorView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CGraphicEditorView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CGraphicEditorView::OnUpdateEditUndo)
	ON_COMMAND(ID_DRAWPROPERTY_FONT, &CGraphicEditorView::OnDrawpropertyFont)
	ON_UPDATE_COMMAND_UI(ID_DRAWPROPERTY_FONT, &CGraphicEditorView::OnUpdateDrawpropertyFont)
	ON_MESSAGE(UM_CHANGEALPHA, &CGraphicEditorView::OnChangeAlpha)
	ON_MESSAGE(UM_CHANGELINEJOIN, &CGraphicEditorView::OnChangeLineJoin)
	ON_MESSAGE(UM_CHANGELINEWEIGHT, &CGraphicEditorView::OnChangeLineWeight)
	ON_MESSAGE(UM_CHANGELINECAPSTART, &CGraphicEditorView::OnChangeLineCapStart)
	ON_MESSAGE(UM_CHANGELINECAPEND, &CGraphicEditorView::OnChangeLineCapEnd)
	ON_MESSAGE(UM_CHANGELINECOLOR, &CGraphicEditorView::OnChangeLineColor)
	ON_MESSAGE(UM_CHANGEFILLCOLOR, &CGraphicEditorView::OnChangeFillColor)
	ON_WM_SIZE()
	ON_MESSAGE(UM_CHANGEGRDTCOLOR, &CGraphicEditorView::OnChangeGrdtColor)
END_MESSAGE_MAP()

// CGraphicEditorView 생성/소멸

// 생성자
CGraphicEditorView::CGraphicEditorView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bsDrawMode = m_bsSelectMode = m_bsPolyDraw = m_bsDBClicked = 
	m_bsPointSelected = m_bsMakeFocusRect = m_bsMultiSelectMode = 
	m_bExternalCmd = m_bsMove = m_bsResize = FALSE;
	
	m_dlgBrushPtn = NULL;
	m_dlgLinePtn = NULL;

	CDrawPropertyValue::InitializeLogFont(); //로그폰트 초기화
}

// 소멸자
CGraphicEditorView::~CGraphicEditorView()
{
}

BOOL CGraphicEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

//-------------------------------------------------------------------------
// 　CGraphicEditorView 그리기
//-------------------------------------------------------------------------
void CGraphicEditorView::OnDraw(CDC* pDC)
{
	CGraphicEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//////////////////////////////////////////////////////////////////////////
	//더블 버퍼링	
	CRect rect;
	GetClientRect(rect); //현재 클라이언트의 크기를 구함

	m_psBitmap->SetBitmapDimension(rect.Width(), rect.Height());
	m_psOldBitmap = m_psMemDC->SelectObject(m_psBitmap); //생성한 비트맵을 선택
	//생성한 비트맵은 검은색이므로 흰색 배경을 그림
	CGdiObject* psOldBrush = m_psMemDC->SelectStockObject(WHITE_BRUSH);
	CGdiObject* psOldPen = m_psMemDC->SelectStockObject(WHITE_PEN);
	m_psMemDC->Rectangle(rect);
	//이전 펜과 브러쉬로 되돌림
	m_psMemDC->SelectObject(psOldBrush);
	m_psMemDC->SelectObject(psOldPen);

	//////////////////////////////////////////////////////////////////////////
	//리스트에 저장된 그림을 그림
	POSITION pos = pDoc->m_sGObjectList.GetHeadPosition();
	while(pos)
	{
		GObject* psGObj = (GObject*)pDoc->m_sGObjectList.GetNext(pos);
		psGObj->Draw(m_psMemDC);
	}

	//////////////////////////////////////////////////////////////////////////
	// Polyline을 그릴 때 마우스 커서 위치에 직선이 그어지도록 하기 위해 사용
	if(m_bsPolyDraw == TRUE && m_sCurrPoint != CPoint(0,0) && m_bsDBClicked == FALSE)
	{
		pDoc->GetPolyLine()->DrawingLine(m_psMemDC, m_sCurrPoint); //현재 마우스 커서 위치로 라인을 그림
	}

	//////////////////////////////////////////////////////////////////////////
	//단일 선택 모드일 경우
	if(m_bsSelectMode == TRUE && pDoc->m_sCurrObject != NULL)
	{
		GObject* psGObj = (GObject*)pDoc->m_sCurrObject;
		psGObj->DrawSelectLine(m_psMemDC);
	}

	//////////////////////////////////////////////////////////////////////////
	//선택 모드에서 "클릭->드래그" 할 경우
	if(m_bsMakeFocusRect == TRUE)
	{
		CRect selectRect = MakeSelectRect();
		//점선 영역 그리기
		m_psMemDC->DrawFocusRect(selectRect);
	}

	//////////////////////////////////////////////////////////////////////////
	// 다중 선택 모드일 경우
	if(m_bsMultiSelectMode == TRUE)
	{
		POSITION pos = pDoc->m_sSelectedList.GetTailPosition();
		while(pos)
		{
			GObject* psGObj = (GObject*)pDoc->m_sSelectedList.GetPrev(pos);
			if(psGObj->IsGrouped() == FALSE)
				psGObj->DrawSelectLine(m_psMemDC);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 폴리 라인의 선택된 점을 그림
	if(m_bsPointSelected == TRUE && pDoc->m_sCurrObject->GetType() == POLYLINE)
	{
		GPolyline* psGPolyline = (GPolyline*)pDoc->m_sCurrObject;
		psGPolyline->DrawSelectedPoint(m_psMemDC, m_nsSelectPtIndex);
	}
	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 현재 그려진 것을 그림
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_psMemDC, 0, 0, SRCCOPY);
}

void CGraphicEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphicEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{

#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicEditorView 진단

#ifdef _DEBUG
void CGraphicEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicEditorDoc* CGraphicEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicEditorDoc)));
	return (CGraphicEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicEditorView 메시지 처리기
//-------------------------------------------------------------------------
//　마우스 왼쪽 버튼 눌렀을 때의 처리
//-------------------------------------------------------------------------
void CGraphicEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture(); // 마우스 캡쳐 시작
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	m_bsDrawMode = TRUE; //그리기 모드 시작
	psDoc->m_sClickedPoint = point;

	switch(psDoc->m_sCurrType)
	{
	case SELECT:
		m_bsPointSelected = FALSE;
		if(m_bsTextDraw == TRUE)	//텍스트 입력 모드일 경우
		{
			DestroyCaret();
			m_bsTextDraw = FALSE;	//텍스트 입력 모드 해제
		}

		//////////////////////////////////////////////////////////////////////////
		//ctrl Key를 누른 상태에서의 다중 선택 개체 추가/제거 by 신희철
		if(HIBYTE(GetKeyState(VK_CONTROL)) & 0x01)
		{
			m_sSelectStartPt = point;
			//클릭 지점에 그래피컬 개체가 있을 경우
			if(psDoc->IsExistSelectedGObject(point) == TRUE)
			{
				m_bsMakeFocusRect = FALSE; //FocusRect 그리기 취소
				BOOL bsExist = FALSE;
				POSITION delPos;
				//선택 리스트를 순회하여 클릭한 개체가 선택 리스트에 있을 경우 선택 리스트에서 제거
				POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
				while(pos)
				{
					delPos = pos;
					GObject* psGObj = psDoc->m_sSelectedList.GetPrev(pos);
					if(psGObj == psDoc->m_sCurrObject)
					{
						psDoc->m_sSelectedList.RemoveAt(delPos);
						bsExist = TRUE; //리스트에 존재했었음을 표시
						break;
					}
				}
				//선택리스트에 없었을 경우 클릭한 개체를 선택 리스트에 추가
				if(bsExist == FALSE)
					psDoc->m_sSelectedList.AddHead(psDoc->m_sCurrObject);
				
				//선택 리스트에 들어있는 그래피컬 개체의 수로 단일선택/다중선택/선택해제 결정
				int nSelectCount = psDoc->m_sSelectedList.GetSize();
				if(nSelectCount >= 2) //2개 이상 있을 경우
				{
					m_bsSelectMode = FALSE; //선택 모드 해제
					m_bsMultiSelectMode = TRUE; //다중 선택 모드
				}
				else if(nSelectCount == 1) //1개일 경우
				{
					m_bsSelectMode = TRUE; //단일 선택 모드
					m_bsMultiSelectMode = FALSE; //다중 선택 모드 해제
					psDoc->m_sCurrObject = psDoc->m_sSelectedList.GetHead(); //선택된 개체를 현재 선택한 것으로 지정
				}
				else if(nSelectCount < 1) //없을 경우
				{
					m_bsSelectMode = FALSE; //단일 선택모드 해제
					m_bsMultiSelectMode = FALSE; //다중 선택모드 해제
				}
			}
			//클릭 지점에 그래피컬 개체가 없을 경우
			else
			{
				m_bsMultiSelectMode = FALSE;
				m_bsSelectMode = FALSE;
				psDoc->m_sSelectedList.RemoveAll();
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//다중 선택된 개체가 없을 경우 by 신희철
		else if(psDoc->m_sSelectedList.GetSize() < 2)
		{
			m_bsMultiSelectMode = FALSE;
			//////////////////////////////////////////////////////////////////////////
			//단일 선택에서 선택 지점이 그래피컬 개체의 점인 경우
			if((m_nsSelectPtIndex = psDoc->FindSelectPoint(point)) != -1)
			{
				m_bsMakeFocusRect = FALSE;
				m_bsSelectMode = TRUE;
				m_bsPointSelected = TRUE;
			}
			//////////////////////////////////////////////////////////////////////////
			//단일 선택에서 선택 지점에 그래피컬 개체가 있을 경우 
			else if(psDoc->IsExistSelectedGObject(point) == TRUE)
			{
				m_bsSelectMode = TRUE; //선택 모드
				m_bsMakeFocusRect = FALSE; //다중 선택 모드 아님
				if(psDoc->m_sSelectedList.GetSize() != 0)
					psDoc->m_sSelectedList.RemoveAll();
				psDoc->m_sSelectedList.AddHead(psDoc->m_sCurrObject);
			}
			//////////////////////////////////////////////////////////////////////////
			//아무 개체의 선택도 아닌 경우
			else
			{
				m_sSelectStartPt = point;
				psDoc->m_sSelectedList.RemoveAll();
				m_bsSelectMode = FALSE;
				m_bsMultiSelectMode = FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//다중 선택된 개체가 있을 경우 by 신희철
		else if(psDoc->m_sSelectedList.GetSize() >= 2)
		{
			POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
			BOOL bPtInSelected = FALSE;
			while(pos)
			{
				//다중 선택된 개체들의 리스트를 순회
				GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
				if(psGObj->PtInGObj(point) == TRUE) //클릭지점이 다중 선택된 개체 안에 있을 경우
					bPtInSelected = TRUE; //클릭 지점이 있다는 것을 표시
			}
			//클릭 지점이 다중 선택된 개체 안에 있을 경우, 다중 선택 모드
			if(bPtInSelected == TRUE)
			{
				m_bsMakeFocusRect = FALSE;
				m_bsMultiSelectMode = TRUE; //다중 선택 모드
			}
			else if(psDoc->IsExistSelectedGObject(point) == TRUE)
			{
				m_bsSelectMode = TRUE; //선택 모드
				m_bsMakeFocusRect = FALSE; //다중 선택 모드 아님
				psDoc->m_sSelectedList.RemoveAll();
				psDoc->m_sSelectedList.AddHead(psDoc->m_sCurrObject);
			}
			//클릭지점이 다중 선택된 개체 안에 없을 경우, 다중 선택모드를 해제하고, 다중선택 리스트의 내용을 지움
			else
			{
				m_bsMultiSelectMode = FALSE;
				psDoc->m_sSelectedList.RemoveAll();
				m_sSelectStartPt = point;
			}
		}

		// PHD
		// 그룹화된 그래피컬 객체들을 리사이징하기 위한 함수 호출
		if(m_bsPointSelected == TRUE) {
			if(psDoc->m_sCurrObject->GetType() == GGROUP) {
				((GGroup *)psDoc->m_sCurrObject)->StartResizing(point.x, point.y, m_nsSelectPtIndex);
			}
		}

		break;
	case LINE:
		psDoc->GetLine(TRUE /*새로 생성*/)->SetStartPoint(point);
		//속성에서 변경한 내용 가져와 지정
		psDoc->GetLine()->SetAlpha(CDrawPropertyValue::m_nsAlpha);
		psDoc->GetLine()->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
		psDoc->GetLine()->SetLineColor(CDrawPropertyValue::m_sLineColor);
		psDoc->GetLine()->SetThickness(CDrawPropertyValue::m_nsThickness);
		psDoc->GetLine()->SetStartCap(CDrawPropertyValue::m_nsStartCap);
		psDoc->GetLine()->SetEndCap(CDrawPropertyValue::m_nsEndCap);
		break;
	case POLYLINE:
		if(m_bsPolyDraw == FALSE)
		{
			psDoc->GetPolyLine(TRUE /*새로 생성*/)->AddOnePt(point);
			//속성에서 변경한 내용 가져와 지정
			psDoc->GetPolyLine()->SetAlpha(CDrawPropertyValue::m_nsAlpha);
			psDoc->GetPolyLine()->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
			psDoc->GetPolyLine()->SetLineColor(CDrawPropertyValue::m_sLineColor);
			psDoc->GetPolyLine()->SetThickness(CDrawPropertyValue::m_nsThickness);
			psDoc->GetPolyLine()->SetStartCap(CDrawPropertyValue::m_nsStartCap);
			psDoc->GetPolyLine()->SetEndCap(CDrawPropertyValue::m_nsEndCap);
			m_bsPolyDraw = TRUE;
		}
		else
		{
			psDoc->GetPolyLine()->AddOnePt(point);
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}
		break;
	case RECTANGLE:
		psDoc->GetRectangle(TRUE /*새로 생성*/)->SetStartPoint(point);
		//속성에서 변경한 내용 가져와 지정
		psDoc->GetRectangle()->SetAlpha(CDrawPropertyValue::m_nsAlpha);
		psDoc->GetRectangle()->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
		psDoc->GetRectangle()->SetLineColor(CDrawPropertyValue::m_sLineColor);
		psDoc->GetRectangle()->SetThickness(CDrawPropertyValue::m_nsThickness);
		psDoc->GetRectangle()->SetBrushColor(CDrawPropertyValue::m_sBrushColor);
		psDoc->GetRectangle()->SetFaceType(CDrawPropertyValue::m_nsFaceType);
		psDoc->GetRectangle()->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
		if(CDrawPropertyValue::m_nsBrushStyle == 1) //HATCH 인 경우 Hatch 스타일 설정
			psDoc->GetRectangle()->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
		else if(CDrawPropertyValue::m_nsBrushStyle == 2) //GRADIENT인 경우 그라디언트 스타일 설정
		{
			psDoc->GetRectangle()->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
			psDoc->GetRectangle()->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
		}
		psDoc->GetRectangle()->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
		break;
	case ELLIPSE:
		psDoc->GetEllipse(TRUE /*새로 생성*/)->SetStartPoint(point);
		//속성에서 변경한 내용 가져와 지정
		psDoc->GetEllipse()->SetAlpha(CDrawPropertyValue::m_nsAlpha);
		psDoc->GetEllipse()->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
		psDoc->GetEllipse()->SetLineColor(CDrawPropertyValue::m_sLineColor);
		psDoc->GetEllipse()->SetThickness(CDrawPropertyValue::m_nsThickness);
		psDoc->GetEllipse()->SetBrushColor(CDrawPropertyValue::m_sBrushColor);
		psDoc->GetEllipse()->SetFaceType(CDrawPropertyValue::m_nsFaceType);
		psDoc->GetEllipse()->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
		if(CDrawPropertyValue::m_nsBrushStyle == 1) //HATCH 인 경우 Hatch 스타일 설정
			psDoc->GetEllipse()->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
		else if(CDrawPropertyValue::m_nsBrushStyle == 2) //GRADIENT인 경우 그라디언트 스타일 설정
		{
			psDoc->GetEllipse()->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
			psDoc->GetEllipse()->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
		}
		psDoc->GetEllipse()->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
		break;
	case TEXT:
		{
			m_bsTextDraw = TRUE;
			psDoc->GetText(TRUE /*새로 생성*/)->SetStartPoint(point);
			//속성에서 변경한 내용 가져와 지정
			psDoc->GetText()->SetLogFont(CDrawPropertyValue::m_sLogFont);
			psDoc->GetText()->SetLineColor(CDrawPropertyValue::m_sLineColor);
			psDoc->GetText()->SetBgColor(CDrawPropertyValue::m_sBrushColor);

			//캐럿 위치 세팅
			m_sCurrPoint = point;
			CClientDC dc(this);
			LOGFONT lf = psDoc->GetText()->GetLogfont();
			int yPixelPerInch = dc.GetDeviceCaps(LOGPIXELSY);
			int fontHeight = int(abs(lf.lfHeight * 72.0 / yPixelPerInch) + 0.5); //폰트 높이를 논리 좌표로 바꿈

			CreateSolidCaret(1, CDrawPropertyValue::m_nsFontSize); //캐럿생성
			SetCaretPos(point);
			ShowCaret(); //캐럿 출력

			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			break;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

//-------------------------------------------------------------------------
//　마우스 움직일 때의 처리
//-------------------------------------------------------------------------
void CGraphicEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bsDrawMode == TRUE)
	{
		CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	
		//이동 시 delta x,y 구하기
		int dx = point.x - psDoc->m_sClickedPoint.x,
			dy = point.y - psDoc->m_sClickedPoint.y;
		psDoc->m_sClickedPoint = point; //클릭지점은 현재 포인트로
		switch(psDoc->m_sCurrType)
		{
		case LINE:
			psDoc->GetLine()->SetEndPoint(point); //이동 중에 계속 종료점을 재지정
			break;
		case POLYLINE:
			if(m_bsPolyDraw == TRUE)
			{
				m_bsDBClicked = FALSE;
				m_sCurrPoint = point;
			}
				break;
		case RECTANGLE:
			psDoc->GetRectangle()->SetEndPoint(point);
			break;
		case ELLIPSE:
			psDoc->GetEllipse()->SetEndPoint(point);
			break;
		case SELECT:
			if(m_bsPointSelected == TRUE) //점이 선택된 경우에만 동작
			{
				m_bsResize = TRUE; //리사이징
				// 현재 선택된 객체가 GGROUP 타입이면
				// GROUP을 위한 리사이징 함수를 호출
				// 아니면 일반적인 리사이징 함수 호출
				// PHD
				if(psDoc->m_sCurrObject->GetType() == GGROUP) {
					((GGroup *)psDoc->m_sCurrObject)->DoResizing(point.x, point.y);
				} else {
					//한 점 이동 또는 크기 변경
					if(psDoc->m_sCurrObject->GetType() != TEXT)
						psDoc->m_sCurrObject->Resize(m_nsSelectPtIndex, dx, dy);
				}
			}
			else if(m_bsSelectMode == TRUE) //선택모드일 경우에만 동작
			{
				m_bsMove = TRUE;
				psDoc->m_sCurrObject->Move(dx, dy); //이동
				psDoc->m_sCurrObject->GetRegion(); //이동한 지점에서 다시 리전을 구함
			}
			//다중 선택일 경우에만 동작
			else if(m_bsMultiSelectMode == TRUE)
			{
				m_bsMove = TRUE;
				POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
				while(pos)
				{
					GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
					psGObj->Move(dx, dy); //이동
				}
			}

			BOOL bsSomethingSelected = m_bsSelectMode | m_bsMultiSelectMode | m_bsPointSelected;
			if(nFlags & MK_LBUTTON && bsSomethingSelected==FALSE) //왼쪽 버튼을 누르고, 아무것도 선택이 아닐 때만 동작
			{
				m_bsMakeFocusRect = TRUE; //FocusRect를 그림
				m_sSelectEndPt = point; //끝 지점
			}
			break;
		}
		Invalidate(); //화면을 다시 그림
	}

	CView::OnMouseMove(nFlags, point);
}

//-------------------------------------------------------------------------
// 마우스 왼쪽 버튼 뗄 때의 처리
//-------------------------------------------------------------------------
void CGraphicEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE("FOCUS: %d\n", m_bsMakeFocusRect);
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	if(m_bsDrawMode == TRUE)
	{
		CRect selectRect = MakeSelectRect();
		switch(psDoc->m_sCurrType)
		{
		case SELECT:
			m_sSelectEndPt = point; //다중 선택영역의 끝지점 지정
			//Focus 사각형을 그리고 있고, Focus 사각형에 그래피컬 개체가 있을 경우
			if (m_bsMakeFocusRect == TRUE &&
				psDoc->FindGObjInFocusRect(selectRect) == TRUE)
			{
				if(psDoc->m_sSelectedList.GetSize() >= 2) //선택 리스트에 2개 이상 존재 시 다중 선택 모드
					m_bsMultiSelectMode = TRUE;
				else //선택 모드에 1개만 존재 시 단일 선택 모드
				{
					psDoc->m_sCurrObject = (GObject*)psDoc->m_sSelectedList.GetHead();
					m_bsSelectMode = TRUE;
				}
			}
			m_bsMakeFocusRect = FALSE; //Focus 사각형 그리기 해제
			
			//선택 모드이고 현재 그래피컬 개체의 타입이 POLYLINE이 아닐 경우
			if(m_bsSelectMode == TRUE && psDoc->m_sCurrObject->GetType() != POLYLINE)
				m_bsPointSelected = FALSE; //점 선택 해제
			
			//그래피컬 개체의 이동 중이었을 경우
			if(m_bsMove == TRUE)
			{
				psDoc->CheckPoint(); //Undo 가능함을 알림
				GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
				m_bsMove = FALSE; //이동이 끝남을 알림
			}
			//그래피컬 개체의 크기조절 작업이었을 경우
			else if(m_bsResize == TRUE)
			{
				psDoc->CheckPoint(); //Undo 가능함을 알림
				GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
				m_bsResize = FALSE; //크기조정이 끝남을 알림
			}

			//////////////////////////////////////////////////////////////////////////
			// 선택한 개체의 정보를 다이얼로그로 전송
			if(m_bsMultiSelectMode == TRUE || m_bsSelectMode == TRUE)
			{
				this->DrawPropertyInfoSetting();
			}

			break;
		case LINE:
			psDoc->GetLine()->SetEndPoint(point); // 완전한 종료점을 지정
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			break;
		case RECTANGLE:
			psDoc->GetRectangle()->SetEndPoint(point);  //완전한 종료점을 지정
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			break;
		case ELLIPSE:
			psDoc->GetEllipse()->SetEndPoint(point); //완전한 종료점을 지정
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			break;
		}
	}

	//폴리라인이 아닐 경우 캡쳐 해제 및 그리기 모드 종료
	if(psDoc->m_sCurrType != POLYLINE)
	{
		m_bsDrawMode = FALSE;
		::ReleaseCapture();
	}
	Invalidate(); //화면을 다시 그림
	CView::OnLButtonUp(nFlags, point);
}

//-------------------------------------------------------------------------
//　마우스 왼쪽 버튼 더블 클릭 시 처리 by 신희철
//-------------------------------------------------------------------------
void CGraphicEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	if(m_bsDrawMode == TRUE && m_bsPolyDraw == TRUE) //폴리라인이고 그리기 모드 일 경우
	{
		m_bsPolyDraw = FALSE; //폴리라인 그리기 종료
		m_bsDrawMode = FALSE; //그리기 모드 종료
		m_bsDBClicked = TRUE; //더블 클릭 했음을 표시
		::ReleaseCapture(); //캡쳐 해제
	}

	//선택 모드이고, 현재 클릭지점에 그래피컬 개체가 있고, 그 그래피컬 개체의 타입이 TEXT인 경우 텍스트 편집
	else if(m_bsSelectMode == TRUE && psDoc->IsExistSelectedGObject(point) && psDoc->m_sCurrObject->GetType() == TEXT)
	{
		m_bsSelectMode = FALSE; //선택모드 해제
		m_bsTextDraw = TRUE;	//텍스트 쓰기 모드 시작
		
		//캐럿 출력을 위한 작업
		GText* psGText = (GText*)psDoc->m_sCurrObject;
		CClientDC dc(this);
		CFont font;
		font.CreateFontIndirect(&psGText->GetLogfont());
		CFont* oldFont = dc.SelectObject(&font);
		CSize strSize = dc.GetTextExtent(psGText->GetString(),psGText->GetStringCount());
		dc.SelectObject(oldFont);
		
		LOGFONT lf = psGText->GetLogfont();
		int yPixelPerInch = dc.GetDeviceCaps(LOGPIXELSY);
		int fontHeight = int(abs(lf.lfHeight * 72.0 / yPixelPerInch) + 0.5);

		//캐럿 출력
		CreateSolidCaret(1, fontHeight); //캐럿생성
		SetCaretPos(CPoint(psGText->GetStartPoint().x + strSize.cx, psGText->GetStartPoint().y));
		psGText->SetEndPoint(CPoint(psGText->GetStartPoint().x + strSize.cx, psGText->GetStartPoint().y));
		ShowCaret();

		Invalidate();

		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	CView::OnLButtonDblClk(nFlags, point);
}

//-------------------------------------------------------------------------
//　그리기 도구 선택 시 처리
//-------------------------------------------------------------------------
// Select
void CGraphicEditorView::OnDrawtoolSelect()
{
	GetDocument()->m_sCurrType = SELECT;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 25);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"선택");
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 25);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"선택");
	if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

// Line
void CGraphicEditorView::OnDrawtoolLine()
{
	GetDocument()->m_sCurrType = LINE;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 50);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"선 그리기");
	if(m_bsSelectMode == TRUE || m_bsMultiSelectMode == TRUE) //선택모드였을 경우
	{
		m_bsSelectMode = FALSE;	//FALSE로 지정
		m_bsMultiSelectMode = FALSE; //FALSE로 지정
		GetDocument()->m_sSelectedList.RemoveAll(); //선택 리스트에 있던 내용 제거
		Invalidate(); //다시 그리기
	}
	else if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

// PolyLine
void CGraphicEditorView::OnDrawtoolPolyline()
{
	GetDocument()->m_sCurrType = POLYLINE;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 90);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"폴리라인 그리기");

	//CToolTipCtrl pToolTip;
	//pToolTip.AddTool(GetDlgItem(ID_DRAWTOOL_POLYLINE), L"폴리라인을 그립니다");
	if(m_bsSelectMode == TRUE || m_bsMultiSelectMode == TRUE) //선택모드였을 경우
	{
		m_bsSelectMode = FALSE;	//FALSE로 지정
		m_bsMultiSelectMode = FALSE; //FALSE로 지정
		GetDocument()->m_sSelectedList.RemoveAll(); //선택 리스트에 있던 내용 제거
		Invalidate(); //다시 그리기
	}
	else if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

// Rectangle
void CGraphicEditorView::OnDrawtoolRectangle()
{
	GetDocument()->m_sCurrType = RECTANGLE;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 80);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"사각형 그리기");
	if(m_bsSelectMode == TRUE || m_bsMultiSelectMode == TRUE) //선택모드였을 경우
	{
		m_bsSelectMode = FALSE;	//FALSE로 지정
		m_bsMultiSelectMode = FALSE; //FALSE로 지정
		GetDocument()->m_sSelectedList.RemoveAll(); //선택 리스트에 있던 내용 제거
		Invalidate(); //다시 그리기
	}
	else if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

// Ellipse
void CGraphicEditorView::OnDrawtoolEllipse()
{
	GetDocument()->m_sCurrType = ELLIPSE;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 50);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"원 그리기");
	if(m_bsSelectMode == TRUE || m_bsMultiSelectMode == TRUE) //선택모드였을 경우
	{
		m_bsSelectMode = FALSE;	//FALSE로 지정
		m_bsMultiSelectMode = FALSE; //FALSE로 지정
		GetDocument()->m_sSelectedList.RemoveAll(); //선택 리스트에 있던 내용 제거
		Invalidate(); //다시 그리기
	}
	else if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

// Text
void CGraphicEditorView::OnDrawtoolText()
{
	GetDocument()->m_sCurrType = TEXT;
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	ppMainFrame->m_wndStatusBar.SetPaneWidth(1, 38);
	ppMainFrame->m_wndStatusBar.SetPaneText(1, L"글쓰기");
	if(m_bsSelectMode == TRUE || m_bsMultiSelectMode == TRUE) //선택모드였을 경우
	{
		m_bsSelectMode = FALSE;	//FALSE로 지정
		m_bsMultiSelectMode = FALSE; //FALSE로 지정
		GetDocument()->m_sSelectedList.RemoveAll(); //선택 리스트에 있던 내용 제거
		Invalidate(); //다시 그리기
	}
	else if(m_bsTextDraw == TRUE) //텍스트 그리기 였을 경우
	{
		m_bsTextDraw = FALSE; //텍스트 그리기 종료
		::DestroyCaret(); //캐럿 제거
		Invalidate(); //다시 그리기
	}
}

//-------------------------------------------------------------------------
//　그리기 도구 업데이트 처리
//-------------------------------------------------------------------------
//선택일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolSelect(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == SELECT;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(SELECT); //그리기 속성 툴바 상태 변경
}

//라인일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolLine(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == LINE;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(LINE); //그리기 속성 툴바 상태 변경
}

//폴리라인일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolPolyline(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == POLYLINE;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(POLYLINE); //그리기 속성 툴바 상태 변경
}

//사각형일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolRectangle(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == RECTANGLE;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(RECTANGLE); //그리기 속성 툴바 상태 변경
}

//타원일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolEllipse(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == ELLIPSE;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(ELLIPSE); //그리기 속성 툴바 상태 변경
}

//텍스트일 경우 체크
void CGraphicEditorView::OnUpdateDrawtoolText(CCmdUI *pCmdUI)
{
	BOOL bsEnable = GetDocument()->m_sCurrType == TEXT;
	pCmdUI->SetCheck(bsEnable);
	if(bsEnable)
		this->DrawProperyControlSetting(TEXT); //그리기 속성 툴바 상태 변경
}

//-------------------------------------------------------------------------
//　더블 버퍼링을 하므로 배경을 다시 그리지 않음
//-------------------------------------------------------------------------
BOOL CGraphicEditorView::OnEraseBkgnd(CDC* pDC)
{
// 	return CView::OnEraseBkgnd(pDC);
	return FALSE;
}

//-------------------------------------------------------------------------
//　더블 버퍼링을 위한 준비
//-------------------------------------------------------------------------
void CGraphicEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	//dc를 만듬
	m_psMemDC = new CDC();
	m_psMemDC->CreateCompatibleDC(GetDC());
		
	//비트맵을 만듬
	m_psBitmap = new CBitmap();
	int width = GetSystemMetrics(SM_CXSCREEN); //화면 폭을 구함
	int height= GetSystemMetrics(SM_CYSCREEN); //화면 높이를 구함
	m_psBitmap->CreateCompatibleBitmap(GetDC(), width, height);
}

//-------------------------------------------------------------------------
//　더블 버퍼링에 썼던 메모리 해제
//-------------------------------------------------------------------------
void CGraphicEditorView::OnDestroy()
{
	//더블 버퍼링을 위한 변수 해제
	m_psBitmap->DeleteObject();
	m_psMemDC->DeleteDC();

	CMainFrame* pMainFrame = (CMainFrame *)GetParentFrame();

	CView::OnDestroy();
}

//-------------------------------------------------------------------------
//　키 누르는 이벤트 발생 처리
//-------------------------------------------------------------------------
void CGraphicEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	GObject *ppGObj = (GObject *)psDoc->m_sCurrObject;
	
	// Delete키 눌린 경우
	if(nChar == VK_DELETE) 
	{
		//단일 선택의 경우
		if(m_bsSelectMode == TRUE)
		{
			if(m_bsPointSelected == TRUE //폴리라인의 한 점 제거
				&& psDoc->m_sCurrObject->GetType() == POLYLINE)
			{
				GPolyline* psGPolyline = (GPolyline*)psDoc->m_sCurrObject;
				BOOL bsOnePtDeleted = psGPolyline->DelOnePt(m_nsSelectPtIndex);
				if(bsOnePtDeleted == FALSE)
				{
					POSITION delPos = psDoc->m_sGObjectList.Find(psGPolyline);
					psDoc->m_sGObjectList.RemoveAt(delPos);
					m_bsSelectMode = FALSE; //셀렉트 모드가 아님
				}
				m_bsPointSelected = FALSE;
				psDoc->CheckPoint(); //Undo 가능함을 알림
				GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			}
			else //일반 제거
			{
				POSITION pos = psDoc->m_sGObjectList.Find(ppGObj); //리스트에서 현재 선택된 오브젝트의 위치를 찾음
				psDoc->m_sGObjectList.RemoveAt(pos); //리스트에서 제거
				if(ppGObj->GetType() == GGROUP) //그룹일 경우
				{
					GGroup* psGGroup = (GGroup*)ppGObj; //그룹 포인터형으로 교체
					CTypedPtrList<CObList, GObject*> deleteList; //그룹 데이터를 저장할 포인터 리스트
					psGGroup->GetGroupData(&deleteList); //그룹데이터를 받아옴
					POSITION groupDataPos = deleteList.GetHeadPosition(); //그룹데이터를 순회하여 제거
					while(groupDataPos)
					{
						GObject* psDelGObj = (GObject*)deleteList.GetNext(groupDataPos); //그룹데이터를 하나씩 받아옴
						POSITION delPos = psDoc->m_sGObjectList.Find(psDelGObj); //그래피컬 개체 리스트에서 제거위치를 찾음
						psDoc->m_sGObjectList.RemoveAt(delPos); //그래피컬 개체 리스트에서 제거
						delete psDelGObj; //그래피컬 개체 리스트에서 제거한 개체의 메모리 해제
					}

					deleteList.RemoveAll();
				}
				delete ppGObj; //메모리 해제
				m_bsSelectMode = FALSE; //셀렉트 모드가 아님
				psDoc->CheckPoint(); //Undo 가능함을 알림
				GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
			}
		}
		//다중 선택의 경우
		else if(m_bsMultiSelectMode == TRUE)
		{
			POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
			while(pos)//다중 선택된 리스트를 검색
			{
				GObject* psGObj = (GObject*) psDoc->m_sGObjectList.GetPrev(pos); //다중 선택 리스트 하나하나씩 빼내어 리스트에서 제거
				POSITION delPos = psDoc->m_sGObjectList.Find(psGObj); //일반 GObject 리스트에서 현재 선택된 오브젝트의 위치를 찾음
				psDoc->m_sGObjectList.RemoveAt(delPos); //일반 GObject 리스트에서 제거
				if(psGObj->GetType() == GGROUP)
				{
					GGroup* psGGroup = (GGroup*)psGObj; //그룹 포인터형으로 교체
					CTypedPtrList<CObList, GObject*> deleteList; //그룹 데이터를 저장할 포인터 리스트
					psGGroup->GetGroupData(&deleteList); //그룹데이터를 받아옴
					POSITION groupDataPos = deleteList.GetHeadPosition(); //그룹데이터를 순회하여 제거
					while(groupDataPos)
					{
						GObject* psDelGObj = (GObject*)deleteList.GetNext(groupDataPos); //그룹데이터를 하나씩 받아옴
						POSITION delPos = psDoc->m_sGObjectList.Find(psDelGObj); //그래피컬 개체 리스트에서 제거위치를 찾음
						psDoc->m_sGObjectList.RemoveAt(delPos); //그래피컬 개체 리스트에서 제거
						delete psDelGObj; //그래피컬 개체 리스트에서 제거한 개체의 메모리 해제
					}
					deleteList.RemoveAll();
				}
				delete psGObj; //메모리 해제
			}
			m_bsMultiSelectMode = FALSE; //다중 선택 모드 아님
			psDoc->m_sSelectedList.RemoveAll(); //다중 선택 리스트 비우기
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}
		Invalidate(); //다시 그리기
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//-------------------------------------------------------------------------
//　문자 입력처리 함수
//-------------------------------------------------------------------------
void CGraphicEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	CClientDC dc(this);
	CPoint pCaretPos;
	CSize pStringSize;

	if(m_bsTextDraw == TRUE)
	{
		GText* psGText = (GText*)psDoc->m_sCurrObject;
		switch(nChar) {

		// 백스페이스 입력시
		case VK_BACK :
			psGText->DelChar(); // 문자삭제
			break;

		// 한줄 입력이므로 엔터키는 배열에 들어가지 않아도됨
		case VK_RETURN :
		case VK_CONTROL:
			break;

		// 위의 경우 이외의 문자는 배열에 저장
		default :
			psGText->AddChar(nChar);
		}

		// 캐럿의 위치를 세팅
		CFont font;
		font.CreateFontIndirect(&psGText->GetLogfont());
		CFont* oldFont = dc.SelectObject(&font);

		// 문자열길이 계산
		CSize strSize = dc.GetTextExtent(psGText->GetString(), psGText->GetStringCount());

		dc.SelectObject(oldFont);

		// 캐럿의 위치 지정
		SetCaretPos(CPoint(psGText->GetStartPoint().x + strSize.cx, psGText->GetStartPoint().y));

		psGText->SetEndPoint(CPoint(psGText->GetStartPoint().x + strSize.cx, psGText->GetStartPoint().y));
		ShowCaret();		// 캐럿의 위치 이동
		
		Invalidate();
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CGraphicEditorView::OnObjDelete()
{
	this->SendMessage(WM_KEYDOWN, VK_DELETE); //VK_DELETE 을 누른 것과 같은 메시지를 날림
}

void CGraphicEditorView::OnUpdateObjDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bsSelectMode || m_bsMultiSelectMode);
}

//-------------------------------------------------------------------------
//　라인 패턴 지정
//-------------------------------------------------------------------------
void CGraphicEditorView::OnDrawpropertyLinepattern()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
 	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();

	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기

	GLine *ppGLine;
	GEllipse *ppGEllipse;
	GRectangle *ppGRectangle;
	GPolyline *ppGPolyline;
	GGroup* psGGroup;

	int nsAnswer=0;
	if(m_bExternalCmd == FALSE && m_dlgLinePtn == NULL) //외부 명령이 아닐 때만 다이얼로그를 띄움
	{
		m_dlgLinePtn = new LinePattern;
		m_dlgLinePtn->Create(IDD_LINEPATTERN);
		CRect clientRect;
		GetClientRect(clientRect);
		ClientToScreen(&clientRect);

		CRect dlgRect;
		m_dlgLinePtn->GetWindowRect(dlgRect);

		int x = clientRect.right - dlgRect.Width(),
			y = clientRect.top;
		m_dlgLinePtn->SetWindowPos(NULL, x,y,0,0,SWP_NOZORDER | SWP_NOSIZE);
		m_dlgLinePtn->ShowWindow(SW_SHOW);
	}
	else if(m_bExternalCmd == FALSE && m_dlgLinePtn != NULL)
		m_dlgLinePtn->SetFocus();

	// 외부명령인 경우
	if(nsAnswer == IDOK || m_bExternalCmd == TRUE) {
		this->SendMessage(UM_CHANGELINECAPSTART);
		this->SendMessage(UM_CHANGELINECAPEND);
		this->SendMessage(UM_CHANGELINEWEIGHT);
		// 다중선택인 경우
		if(m_bsMultiSelectMode == TRUE) {
			POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
			while(pos)
			{
				GObject* psGObj = psDoc->m_sSelectedList.GetPrev(pos);
				switch(psGObj->GetType())
				{
				case LINE :
					ppGLine = (GLine *)psGObj;
					ppGLine->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);	
					break;

				case ELLIPSE :
					ppGEllipse = (GEllipse *)psGObj;
					ppGEllipse->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
					break;

				case RECTANGLE :
					ppGRectangle = (GRectangle *)psGObj;
					ppGRectangle->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
					break;

				case POLYLINE :
					ppGPolyline = (GPolyline *)psGObj;
					ppGPolyline->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
					break;

				case GGROUP:
					psGGroup = (GGroup*)psGObj;
					psGGroup->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
					break;
				}
			}
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}

		// 단일선택인 경우
		else if(m_bsSelectMode == TRUE) {
			switch(psDoc->m_sCurrObject->GetType()) 
			{
			case LINE :
				ppGLine = (GLine *)psDoc->m_sCurrObject;
				ppGLine->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);	
				break;

			case ELLIPSE :
				ppGEllipse = (GEllipse *)psDoc->m_sCurrObject;
				ppGEllipse->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
				break;

			case RECTANGLE :
				ppGRectangle = (GRectangle *)psDoc->m_sCurrObject;
				ppGRectangle->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
				break;

			case POLYLINE :
				ppGPolyline = (GPolyline *)psDoc->m_sCurrObject;
				ppGPolyline->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
				break;

			case GGROUP:
				psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetPenStyle(CDrawPropertyValue::m_nsPenStyle);
				break;
			}
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}
	}
	m_bExternalCmd = FALSE;
	Invalidate();
}


void CGraphicEditorView::OnUpdateDrawpropertyLinepattern(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}

//-------------------------------------------------------------------------
//　브러쉬 패턴 변경
//-------------------------------------------------------------------------
void CGraphicEditorView::OnDrawpropertyBrushpattern()
{
	CGraphicEditorDoc* psDoc = GetDocument();				// 도큐먼트 얻어오기
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();	// 프레임윈도우 얻기
	GEllipse *ppGEllipse;
	GRectangle *ppGRectangle;
	GGroup* psGGroup;

	int nsAnswer = 0;
	if(m_bExternalCmd == FALSE && m_dlgBrushPtn == NULL)
	{
		m_dlgBrushPtn = new BrushPattern;
		m_dlgBrushPtn->Create(IDD_BRUSHPATTERN);
		CRect clientRect;
		GetClientRect(clientRect);
		ClientToScreen(&clientRect);

		CRect dlgRect;
		m_dlgBrushPtn->GetWindowRect(dlgRect);

		int x = clientRect.right - dlgRect.Width(),
			y = clientRect.top + 208;
		m_dlgBrushPtn->SetWindowPos(NULL, x,y,0,0,SWP_NOZORDER | SWP_NOSIZE);
		m_dlgBrushPtn->ShowWindow(SW_SHOW);
	}
	else if(m_bExternalCmd == FALSE && m_dlgBrushPtn != NULL)
		m_dlgBrushPtn->SetFocus();

	// 외부명령인 경우
	if(nsAnswer == IDOK || m_bExternalCmd == TRUE) {

		// 다중선택
		if(m_bsMultiSelectMode == TRUE) {
			POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
			while(pos)
			{
				GObject* psGObj = psDoc->m_sSelectedList.GetPrev(pos);
				psGObj->SetAlpha(CDrawPropertyValue::m_nsAlpha); //불투명도 값 변경
				switch(psGObj->GetType())
				{
				case RECTANGLE: //사각형의 브러쉬 스타일 변경
					ppGRectangle = (GRectangle*)psGObj;
					ppGRectangle->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
					if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
						ppGRectangle->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
					else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
					{
						ppGRectangle->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
						ppGRectangle->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
					}

					//외곽선 처리(LineJoin) 변경
					ppGRectangle->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
					//면 타입 설정
					ppGRectangle->SetFaceType(CDrawPropertyValue::m_nsFaceType);
					break;
				case ELLIPSE: //타원의 브러쉬 스타일 변경
					ppGEllipse = (GEllipse*)psGObj;
					ppGEllipse->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
					if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
						ppGEllipse->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
					else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
					{
						ppGEllipse->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
						ppGEllipse->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
					}

					//외곽선 처리(LineJoin) 변경
					ppGEllipse->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
					//면 타입 설정
					ppGEllipse->SetFaceType(CDrawPropertyValue::m_nsFaceType);
					break;
				case GGROUP: //그룹의 브러쉬 스타일 변경
					psGGroup = (GGroup*)psGObj;
					psGGroup->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
					if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
						psGGroup->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
					else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
					{
						psGGroup->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
						psGGroup->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
					}

					//외곽선 처리(LineJoin) 변경
					psGGroup->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
					//면 타입 설정
					psGGroup->SetFaceType(CDrawPropertyValue::m_nsFaceType);
					break;
				}
			}
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}

		// 단일선택
		else if(m_bsSelectMode == TRUE) {
			psDoc->m_sCurrObject->SetAlpha(CDrawPropertyValue::m_nsAlpha); //불투명도 지정
			switch(psDoc->m_sCurrObject->GetType()) {

			case ELLIPSE :
				ppGEllipse = (GEllipse *)psDoc->m_sCurrObject;
				ppGEllipse->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
				if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
					ppGEllipse->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
				else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
				{
					ppGEllipse->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
					ppGEllipse->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
				}
				//외곽선 처리(LineJoin) 변경
				ppGEllipse->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				//면 타입 설정
				ppGEllipse->SetFaceType(CDrawPropertyValue::m_nsFaceType);
				break;

			case RECTANGLE :
				ppGRectangle = (GRectangle *)psDoc->m_sCurrObject;
				ppGRectangle->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
				if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
					ppGRectangle->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
				else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
				{
					ppGRectangle->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
					ppGRectangle->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
				}
				//외곽선 처리(LineJoin) 변경
				ppGRectangle->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				//면 타입 설정
				ppGRectangle->SetFaceType(CDrawPropertyValue::m_nsFaceType);

				break;

			case GGROUP:
				psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetBrushStyle(CDrawPropertyValue::m_nsBrushStyle);
				if(CDrawPropertyValue::m_nsBrushStyle == 1) //Hatch일 경우
					psGGroup->SetHatchStyle(CDrawPropertyValue::m_nsHatchStyle);
				else if(CDrawPropertyValue::m_nsBrushStyle == 2) //그라디언트일 경우
				{
					psGGroup->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor);
					psGGroup->SetGradientMode(CDrawPropertyValue::m_nsGradientType);
				}
				//외곽선 처리(LineJoin) 변경
				psGGroup->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				//면 타입 설정
				psGGroup->SetFaceType(CDrawPropertyValue::m_nsFaceType);
				break;
			}
			psDoc->CheckPoint(); //Undo 가능함을 알림
			GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
		}
		m_bExternalCmd = FALSE;
	}
	Invalidate();
}

void CGraphicEditorView::OnUpdateDrawpropertyBrushpattern(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}

//-------------------------------------------------------------------------
//　잘라내기
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditCut()
{
	OnEditCopy();	//복사
	OnObjDelete();	//지우기

	Invalidate();	//다시 그리기
}

//-------------------------------------------------------------------------
//　복사
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditCopy()
{
	CSharedFile file;
	CArchive ar(&file, CArchive::store); //CShareFile 객체에 저장하는 아카이브 선언

	//선택 리스트의 내용을 직렬화를 통해 저장
	CGraphicEditorDoc* psDoc = GetDocument();
	psDoc->m_sSelectedList.Serialize(ar);
	ar.Close();

	//CShareFile 객체에 저장된 내용을 클립보드에 지정
	COleDataSource* psDataSource = NULL;
	TRY
	{
		psDataSource = new COleDataSource; 
		psDataSource->CacheGlobalData(m_cfsDraw, file.Detach());
		OleInitialize(NULL);
		psDataSource->SetClipboard();
	}
	CATCH_ALL(e)
	{
		delete psDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL
}

//-------------------------------------------------------------------------
//　붙여넣기
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditPaste()
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	psDoc->m_sSelectedList.RemoveAll(); //선택 리스트 초기화

	//COleDataObject 객체로 클립보드 내용을 가져옴
	COleDataObject dataObject;
	dataObject.AttachClipboard();

	//클립보드 내용이 이 프로그램에 맞는 형식일 경우만 가능
	if (dataObject.IsDataAvailable(m_cfsDraw))
	{
		CFile* pFile = dataObject.GetFileData(m_cfsDraw);
		if (pFile == NULL)
			return;

		CArchive ar(pFile, CArchive::load); //pFile의 내용을 로드하는 아카이브 선언
		TRY
		{
			//선택 리스트로 직렬화를 수행하여 클립보드의 내용을 선택리스트로 저장
			ar.m_pDocument = GetDocument();
			psDoc->m_sSelectedList.Serialize(ar);
		}
		CATCH_ALL(e)
		{
			ar.Close();
			delete pFile;
			THROW_LAST();
		}
		END_CATCH_ALL

			ar.Close();
		delete pFile;

		//선택 리스트에 저장된 내용을 그래피컬 개체 리스트에 추가
		POSITION pos = psDoc->m_sSelectedList.GetHeadPosition();
		while (pos != NULL)
		{
			GObject* psGObj = psDoc->m_sSelectedList.GetNext(pos);
			if(psGObj->GetType() == GGROUP) //그룹일 경우
				((GGroup*)psGObj)->GetGroupData(&psDoc->m_sGObjectList); //그룹 데이터를 그래피컬 개체 리스트에 추가
			psDoc->m_sGObjectList.AddTail(psGObj); //그래피컬 개체 리스트에 추가
		}
	}

	psDoc->CheckPoint(); //Undo 가능함을 알림
	GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림

	if(psDoc->m_sSelectedList.GetSize() == 1) //단일 개체의 붙여넣기일 경우
	{
		psDoc->m_sCurrObject = psDoc->m_sSelectedList.GetHead(); //현재 선택된 오브젝트로 지정
		psDoc->m_sSelectedList.RemoveAll(); //선택리스트 비우기
		if(psDoc->m_sCurrType == SELECT) //선택 모드일 경우
			m_bsSelectMode = TRUE; //단일 선택모드 활성화
	}
	else //다중 개체의 붙여넣기일 경우
	{
		if(psDoc->m_sCurrType == SELECT) //선택 모드일 경우
			m_bsMultiSelectMode = TRUE; //다중 선택모드 활성화
	}
	Invalidate(); //다시 그리기
}
//-------------------------------------------------------------------------
//　그룹화 by 신희철
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditGroup()
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	psDoc->GetGroup(TRUE /*새로 생성*/)->Add(psDoc->m_sSelectedList); //그룹 생성
	psDoc->m_sSelectedList.RemoveAll(); //선택 리스트에 있는 내용 전부 제거
	psDoc->m_sSelectedList.AddHead(psDoc->GetGroup()); //선택 리스트에 현재 생성한 그룹을 추가
	m_bsSelectMode = TRUE; //단일 선택 모드
	m_bsMultiSelectMode = FALSE; //다중선택 모드 해제
	Invalidate();

	psDoc->CheckPoint(); //Undo 가능함을 알림
	GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
}
//-------------------------------------------------------------------------
//　그룹화 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditGroup(CCmdUI *pCmdUI)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	pCmdUI->Enable(m_bsMultiSelectMode && psDoc->m_sSelectedList.GetSize()>= 2);
}
//-------------------------------------------------------------------------
//　그룹해제 by 신희철
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditUngroup()
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	//그래피컬 개체 리스트에서 제거
	if(m_bsSelectMode == TRUE) //단일 선택의 경우
	{
		POSITION pos = psDoc->m_sGObjectList.Find(psDoc->GetGroup());
		psDoc->m_sGObjectList.RemoveAt(pos);
		//그룹화 해제
		psDoc->m_sSelectedList.RemoveAll();
		GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
		psGGroup->UnGroup(&psDoc->m_sSelectedList);
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택의 경우
	{
		CTypedPtrList<CObList, GObject*> tempList; //임시 저장 리스트
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //다중 선택 리스트를 순회
		while(pos)
		{
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			if(psGObj->GetType() == GGROUP) //다중 선택 리스트 내에 있는 개체가 그룹일 경우
			{
				POSITION delPos = psDoc->m_sGObjectList.Find(psGObj); //제거될 위치를 찾음
				psDoc->m_sGObjectList.RemoveAt(delPos); //그룹을 리스트에서 제외
				GGroup* psGGroup = (GGroup*)psGObj; //그룹 포인터형으로 바꿈
				psGGroup->UnGroup(&tempList); //그룹해제 함수 호출
			}
		}
		psDoc->m_sSelectedList.RemoveAll(); //다중 선택 리스트 모두 초기화
		
		POSITION copyPos = tempList.GetHeadPosition(); //임시 리스트 순회
		while(copyPos)
		{
			//임시리스트에 저장된 그룹화되었던 그래피컬 개체들을 다중 선택 리스트에 추가
			GObject* psGObj = (GObject*)tempList.GetNext(copyPos);
			psDoc->m_sSelectedList.AddHead(psGObj);
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	m_bsSelectMode = FALSE; //단일 선택 모드 해제
	m_bsMultiSelectMode = TRUE; //다중 선택 모드
	psDoc->m_sCurrObject = (GObject*)psDoc->m_sSelectedList.GetHead();
	Invalidate();
}
//-------------------------------------------------------------------------
//　그룹해제 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditUngroup(CCmdUI *pCmdUI)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	BOOL bsEnable=FALSE;
	if(m_bsSelectMode == TRUE) //단일 선택 모드일 경우
	{
		if(psDoc->m_sCurrObject->GetType() == GGROUP)
			bsEnable = TRUE;
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택 모드일 경우
	{
		POSITION pos = psDoc->m_sSelectedList.GetHeadPosition();
		while(pos)
		{
			GObject* psGObj = psDoc->m_sSelectedList.GetNext(pos);
			if (psGObj->GetType() == GGROUP)
			{
				bsEnable = TRUE;
				break;
			}
		}
	}
	pCmdUI->Enable(bsEnable);
}

//-------------------------------------------------------------------------
// 복사 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bsSelectMode || m_bsMultiSelectMode);
}

//-------------------------------------------------------------------------
// 잘라내기 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bsSelectMode || m_bsMultiSelectMode);
}

//-------------------------------------------------------------------------
// 붙여넣기 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	COleDataObject dataObject;
	BOOL bEnable = dataObject.AttachClipboard() &&
		(dataObject.IsDataAvailable(m_cfsDraw));
	pCmdUI->Enable(bEnable);
}

//-------------------------------------------------------------------------
//　DrawFocusRect를 할 경우나 그려진 원 또는 사각형이 선택된 이후 
//　대각선 방향으로 이동하는 것이 아니라
//  상하로 또는 좌우로 바뀌는 경우 TopLeft, BottomRight 좌표를 재조정 　
//-------------------------------------------------------------------------
CRect CGraphicEditorView::MakeSelectRect()
{
	CPoint sStartPt, sEndPt;
	//DrawFocusRect를 위해 left와 right 좌표(x좌표)를 구함
	if(m_sSelectStartPt.x < m_sSelectEndPt.x)
	{
		sStartPt.x = m_sSelectStartPt.x;
		sEndPt.x = m_sSelectEndPt.x;
	}
	else
	{
		sStartPt.x = m_sSelectEndPt.x;
		sEndPt.x = m_sSelectStartPt.x;
	}
	//DrawFocusRect를 위해 top과 bottom 좌표(y좌표)를 구함
	if(m_sSelectStartPt.y < m_sSelectEndPt.y)
	{
		sStartPt.y = m_sSelectStartPt.y;
		sEndPt.y = m_sSelectEndPt.y;
	}
	else
	{
		sStartPt.y = m_sSelectEndPt.y;
		sEndPt.y = m_sSelectStartPt.y;
	}

	return CRect(sStartPt, sEndPt);
}
//-------------------------------------------------------------------------
//　Undo(실행취소)
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditUndo()
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	psDoc->Undo(); //Undo 작업 수행
	psDoc->UpdateAllViews(NULL);
	
	//단일 선택모드였을 경우
	if(m_bsSelectMode == TRUE)
	{
		if(psDoc->m_sGObjectList.GetSize() > 1)
			psDoc->m_sCurrObject = psDoc->m_sGObjectList.GetTail();
		else
			m_bsSelectMode = FALSE;
	}
	//다중 선택모드였을 경우
	else if(m_bsMultiSelectMode == TRUE)
	{
		psDoc->m_sSelectedList.RemoveAll();
		m_bsMultiSelectMode = FALSE;
	}
}

//-------------------------------------------------------------------------
//　Undo 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	pCmdUI->Enable(psDoc->CanUndo()); //Undo 가능할 때만 활성화
}

//-------------------------------------------------------------------------
//　Redo(다시 실행)
//-------------------------------------------------------------------------
void CGraphicEditorView::OnEditRedo()
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	psDoc->Redo(); //Redo 작업 수행
	psDoc->UpdateAllViews(NULL);
	
	//단일 선택모드였을 경우
	if(m_bsSelectMode == TRUE)
	{
		if(psDoc->m_sGObjectList.GetSize() > 1)
			psDoc->m_sCurrObject = psDoc->m_sGObjectList.GetTail();
		else
			m_bsSelectMode = FALSE;
	}
	//다중 선택모드였을 경우
	else if(m_bsMultiSelectMode == TRUE)
	{
		psDoc->m_sSelectedList.RemoveAll();
		m_bsMultiSelectMode = FALSE;
	}
}

//-------------------------------------------------------------------------
//　Redo 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	pCmdUI->Enable(psDoc->CanRedo()); //Redo 가능할 떄만 활성화
}

//-------------------------------------------------------------------------
//　커서의 모양변경 by 박현덕
//-------------------------------------------------------------------------
BOOL CGraphicEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	HCURSOR hCursor;
	CPoint point;

	CRgn *ppCursorInbound;		// IDC_SIZEALL 체크를 위한 CRgn 포인터 선언
	CRect rect;
	CGraphicEditorDoc *ppDoc = this->GetDocument();
	GLine *ppGLine;
	GRectangle *ppGRectangle;
	GPolyline *ppGPolyline;
	GEllipse *ppGEllipse;
	GGroup* psGGroup;
	int pos;

	// 커서의 위치를 Client 영역 위치로 변경
	GetCursorPos(&point);
	ScreenToClient(&point);

	// 클라이언트 영역을 구함
	GetClientRect(&rect);

	// 기본 마우스커서인 화살표 모양
	hCursor = LoadCursor(NULL, IDC_ARROW);

	// 선택시
	if(m_bsSelectMode == TRUE) {
		switch(ppDoc->m_sCurrObject->GetType()) {
		case LINE :
			ppGLine = (GLine *)ppDoc->m_sCurrObject;
			ppCursorInbound = ppGLine->GetRegion();
			pos = ppGLine->FindSelectPoint(point);

			// -1이 아니면 크기를 변경할 수 있는 점 위치
			if(pos != -1) {
				hCursor = LoadCursor(NULL, IDC_SIZENS);
			}
			else if(ppCursorInbound->PtInRegion(point)) {
				hCursor = LoadCursor(NULL, IDC_SIZEALL);
			}
			break;

		case RECTANGLE :
			ppGRectangle = (GRectangle *)ppDoc->m_sCurrObject;

			// 사각형을 선택한 이후 위치를 상하로 바꾸는 경우
			// 좌표 재설정
			{ 
				m_sSelectStartPt = ppGRectangle->GetStartPoint();
				m_sSelectEndPt = ppGRectangle->GetEndPoint();
				CRect rect = MakeSelectRect(); //이 함수 참조 바람
				ppGRectangle->SetBothPoint(rect.TopLeft(), rect.BottomRight());
			}
			ppCursorInbound = ppGRectangle->GetRegion();
			pos = ppGRectangle->FindSelectPoint(point);

			// 선택영역의 8개 점이 아닌경우는 이동을 표현하는 IDC_SIZEALL 마우스 커서로드
			if(ppCursorInbound->PtInRegion(point)) {
				hCursor = LoadCursor(NULL, IDC_SIZEALL);
			}

			// 8방향 좌표에 관한 마우스 커서 변경 switch 문
			switch(pos) {
			case TOPLEFT :
			case BOTTOMRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENWSE);
				break;

			case LEFT :
			case RIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;

			case BOTTOMLEFT :
			case TOPRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENESW);
				break;

			case BOTTOM :
			case TOP :
				hCursor = LoadCursor(NULL, IDC_SIZENS);
			}
			break;

		case ELLIPSE :
			ppGEllipse = (GEllipse *)ppDoc->m_sCurrObject;

			// 리사이즈시 상하로 도형을 뒤집는 경우 마우스 커서의 모양을 재설정
			{ 
				m_sSelectStartPt = ppGEllipse->GetStartPoint();
				m_sSelectEndPt = ppGEllipse->GetEndPoint();
				CRect rect = MakeSelectRect(); //이 함수 참조 바람
				ppGEllipse->SetBothPoint(rect.TopLeft(), rect.BottomRight());
			}
			ppCursorInbound = ppGEllipse->GetRegion();
			pos = ppGEllipse->FindSelectPoint(point);

			// 선택영역 안에 마우스 커서가 위치하는 경우
			// 다음과 같이 이동시 쓰이는 커서로 변경
			if(ppCursorInbound->PtInRegion(point)) {
				hCursor = LoadCursor(NULL, IDC_SIZEALL);
			}

			// 8방향 좌표에 관한 마우스 커서 변경 switch 문
			switch(pos) {
			case TOPLEFT :
			case BOTTOMRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENWSE);
				break;

			case LEFT :
			case RIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;

			case BOTTOMLEFT :
			case TOPRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENESW);
				break;

			case BOTTOM :
			case TOP :
				hCursor = LoadCursor(NULL, IDC_SIZENS);
			}
			break;

		case POLYLINE :
			ppGPolyline = (GPolyline *)ppDoc->m_sCurrObject;
			ppCursorInbound = ppGPolyline->GetRegion();
			pos = ppGPolyline->FindSelectPoint(point);

			// 마우스 커서가 좌표 위에 있는 경우
			if(pos != -1) {
				hCursor = LoadCursor(NULL, IDC_SIZENS);
			}
			else if(ppCursorInbound->PtInRegion(point)) {
				hCursor = LoadCursor(NULL, IDC_SIZEALL);
			}
			break;

		case GGROUP:
			psGGroup = (GGroup*)ppDoc->m_sCurrObject;
			// 리사이즈시 상하로 도형을 뒤집는 경우 마우스 커서의 모양을 재설정
			{ 
				m_sSelectStartPt = psGGroup->GetStartPoint();
				m_sSelectEndPt = psGGroup->GetEndPoint();

				CRect rect = MakeSelectRect();		// 좌표의 정규화
				psGGroup->SetBothPoint(rect.TopLeft(), rect.BottomRight());
			}
			ppCursorInbound = psGGroup->GetRegion();
			pos = psGGroup->FindSelectPoint(point);

			// 선택영역 안에 마우스 커서가 위치하는 경우
			// 다음과 같이 이동시 쓰이는 커서로 변경
			if(ppCursorInbound->PtInRegion(point)) {
				hCursor = LoadCursor(NULL, IDC_SIZEALL);
			}

			// 8방향 좌표에 관한 마우스 커서 변경 switch 문
			switch(pos) {
			case TOPLEFT :
			case BOTTOMRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENWSE);
				break;

			case LEFT :
			case RIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;

			case BOTTOMLEFT :
			case TOPRIGHT :
				hCursor = LoadCursor(NULL, IDC_SIZENESW);
				break;

			case BOTTOM :
			case TOP :
				hCursor = LoadCursor(NULL, IDC_SIZENS);
			}
			break;
		}
	}
	SetCursor(hCursor);
	
	return TRUE;
}
//-------------------------------------------------------------------------
//　그리기 속성 바 항목별 활성화/비활성화 세팅
//-------------------------------------------------------------------------
void CGraphicEditorView::DrawProperyControlSetting(GraphicalObjectType type)
{
	CMainFrame* psMainFrm = (CMainFrame*)GetParent();
	CDrawProperty* psDrawProp = &psMainFrm->m_dlgDrawProperty;

	switch(type)
	{
	case SELECT:
	case GGROUP:
		psDrawProp->AdjustControl();
		psDrawProp->m_sStcFont.EnableWindow();				//폰트 정적 텍스트
		psDrawProp->m_sFontBtn.EnableWindow();				//폰트 버튼
		psDrawProp->m_sLineColorBtn.EnableWindow();			//라인색 버튼
		psDrawProp->m_sStcLinePtn.EnableWindow();			//라인 패턴 정적 텍스트
		psDrawProp->m_sLinePtnCombo.EnableWindow();			//라인 패턴 콤보박스
		psDrawProp->m_sLineWeightSpin.EnableWindow();		//라인 굵기 스핀
		psDrawProp->m_sStcThickness.EnableWindow();			//라인 굵기 정적 텍스트
		psDrawProp->m_sThicknessEdit.EnableWindow();		//라인 굵기 에디트 컨트롤
		psDrawProp->m_sStcStartCap.EnableWindow();			//라인 시작 모양 정적 텍스트
		psDrawProp->m_sStartCapCombo.EnableWindow();		//라인 시작 모양 콤보박스
		psDrawProp->m_sStcEndCap.EnableWindow();			//라인 끝 모양 정적 텍스트
		psDrawProp->m_sEndCapCombo.EnableWindow();			//라인 끝 모양 콤보박스
		psDrawProp->m_sStcLineJoin.EnableWindow();			//외곽선 모양 정적 텍스트
		psDrawProp->m_sLineJoinCombo.EnableWindow();		//외곽선 모양 콤보박스
		psDrawProp->m_sAlphaSpin.EnableWindow();		//불투명도 스핀
		psDrawProp->m_sStcOpacity.EnableWindow();		//불투명도 정적 텍스트
		psDrawProp->m_sAlphaEdit.EnableWindow();		//불투명도 에디트 컨트롤

		psDrawProp->m_sFaceTypeFillOnly.EnableWindow();
		psDrawProp->m_sFaceTypeOutline.EnableWindow();
		psDrawProp->m_sFaceTypeOutlinedFill.EnableWindow();

		if(psDrawProp->m_nsFaceType != 0)
		{
			psDrawProp->m_sFillTypeSolid.EnableWindow();
			psDrawProp->m_sFillTypeHatch.EnableWindow();
			psDrawProp->m_sFillTypeGradient.EnableWindow();
		}
		break;

	case LINE:
	case POLYLINE:
		psDrawProp->m_sFillColorBtn.EnableWindow(FALSE);	//채우기색 버튼
		psDrawProp->m_sGradientEndColorBtn.EnableWindow(FALSE);	//그라디언트 끝 색 버튼
		psDrawProp->m_sStcFillPtn.EnableWindow(FALSE);		//채우기 패턴 정적 텍스트
		psDrawProp->m_sFillPtnCombo.EnableWindow(FALSE);	//채우기 패턴 콤보박스
		psDrawProp->m_sStcGradientMode.EnableWindow(FALSE);	//그라디언트 모드 정적 텍스트
		psDrawProp->m_sGradientCombo.EnableWindow(FALSE);	//그라디언트 모드 콤보 박스
		psDrawProp->m_sStcFont.EnableWindow(FALSE);			//폰트 정적 텍스트
		psDrawProp->m_sFontBtn.EnableWindow(FALSE);			//폰트 버튼
		psDrawProp->m_sLineColorBtn.EnableWindow();			//라인색 버튼
		psDrawProp->m_sStcLinePtn.EnableWindow();			//라인 패턴 정적 텍스트
		psDrawProp->m_sLinePtnCombo.EnableWindow();			//라인 패턴 콤보박스
		psDrawProp->m_sLineWeightSpin.EnableWindow();		//라인 굵기 스핀
		psDrawProp->m_sStcThickness.EnableWindow();			//라인 굵기 정적 텍스트
		psDrawProp->m_sThicknessEdit.EnableWindow();		//라인 굵기 에디트 컨트롤
		psDrawProp->m_sStcStartCap.EnableWindow();			//라인 시작 모양 정적 텍스트
		psDrawProp->m_sStartCapCombo.EnableWindow();		//라인 시작 모양 콤보박스
		psDrawProp->m_sStcEndCap.EnableWindow();			//라인 끝 모양 정적 텍스트
		psDrawProp->m_sEndCapCombo.EnableWindow();			//라인 끝 모양 콤보박스
		psDrawProp->m_sStcLineJoin.EnableWindow(FALSE);		//외곽선 모양 정적 텍스트
		psDrawProp->m_sLineJoinCombo.EnableWindow(FALSE);	//외곽선 모양 콤보박스
		psDrawProp->m_sAlphaSpin.EnableWindow();			//불투명도 스핀
		psDrawProp->m_sStcOpacity.EnableWindow();			//불투명도 정적 텍스트
		psDrawProp->m_sAlphaEdit.EnableWindow();			//불투명도 에디트 컨트롤

		psDrawProp->m_sFaceTypeFillOnly.EnableWindow(FALSE);
		psDrawProp->m_sFaceTypeOutline.EnableWindow(FALSE);
		psDrawProp->m_sFaceTypeOutlinedFill.EnableWindow(FALSE);

		psDrawProp->m_sFillTypeSolid.EnableWindow(FALSE);
		psDrawProp->m_sFillTypeHatch.EnableWindow(FALSE);
		psDrawProp->m_sFillTypeGradient.EnableWindow(FALSE);
		break;

	case ELLIPSE:
	case RECTANGLE:
		psDrawProp->AdjustControl();
		psDrawProp->m_sStcFont.EnableWindow(FALSE);			//폰트 정적 텍스트
		psDrawProp->m_sFontBtn.EnableWindow(FALSE);			//폰트 버튼
		psDrawProp->m_sLineColorBtn.EnableWindow();			//라인색 버튼
		psDrawProp->m_sStcLinePtn.EnableWindow();			//라인 패턴 정적 텍스트
		psDrawProp->m_sLinePtnCombo.EnableWindow();			//라인 패턴 콤보박스
		psDrawProp->m_sLineWeightSpin.EnableWindow();		//라인 굵기 스핀
		psDrawProp->m_sStcThickness.EnableWindow();			//라인 굵기 정적 텍스트
		psDrawProp->m_sThicknessEdit.EnableWindow();		//라인 굵기 에디트 컨트롤
		psDrawProp->m_sStcStartCap.EnableWindow(FALSE);		//라인 시작 모양 정적 텍스트
		psDrawProp->m_sStartCapCombo.EnableWindow(FALSE);	//라인 시작 모양 콤보박스
		psDrawProp->m_sStcEndCap.EnableWindow(FALSE);		//라인 끝 모양 정적 텍스트
		psDrawProp->m_sEndCapCombo.EnableWindow(FALSE);		//라인 끝 모양 콤보박스
		psDrawProp->m_sStcLineJoin.EnableWindow();			//외곽선 모양 정적 텍스트
		psDrawProp->m_sLineJoinCombo.EnableWindow();		//외곽선 모양 콤보박스
		psDrawProp->m_sAlphaSpin.EnableWindow();			//불투명도 스핀
		psDrawProp->m_sStcOpacity.EnableWindow();			//불투명도 정적 텍스트
		psDrawProp->m_sAlphaEdit.EnableWindow();			//불투명도 에디트 컨트롤

		psDrawProp->m_sFaceTypeFillOnly.EnableWindow();
		psDrawProp->m_sFaceTypeOutline.EnableWindow();
		psDrawProp->m_sFaceTypeOutlinedFill.EnableWindow();
		if(psDrawProp->m_nsFaceType != 0)
		{
			psDrawProp->m_sFillTypeSolid.EnableWindow();
			psDrawProp->m_sFillTypeHatch.EnableWindow();
			psDrawProp->m_sFillTypeGradient.EnableWindow();
		}
		break;

	case TEXT:
		psDrawProp->m_sFillColorBtn.EnableWindow();			//채우기색 버튼
		psDrawProp->m_sGradientEndColorBtn.EnableWindow(FALSE);	//그라디언트 끝 색 버튼
		psDrawProp->m_sStcFillPtn.EnableWindow(FALSE);		//채우기 패턴 정적 텍스트
		psDrawProp->m_sFillPtnCombo.EnableWindow(FALSE);	//채우기 패턴 콤보박스
		psDrawProp->m_sStcGradientMode.EnableWindow(FALSE);	//그라디언트 모드 정적 텍스트
		psDrawProp->m_sGradientCombo.EnableWindow(FALSE);	//그라디언트 모드 콤보 박스
		psDrawProp->m_sStcFont.EnableWindow();				//폰트 정적 텍스트
		psDrawProp->m_sFontBtn.EnableWindow();				//폰트 버튼
		psDrawProp->m_sLineColorBtn.EnableWindow();			//라인색 버튼
		psDrawProp->m_sStcLinePtn.EnableWindow(FALSE);			//라인 패턴 정적 텍스트
		psDrawProp->m_sLinePtnCombo.EnableWindow(FALSE);	//라인 패턴 콤보박스
		psDrawProp->m_sLineWeightSpin.EnableWindow(FALSE);	//라인 굵기 스핀
		psDrawProp->m_sStcThickness.EnableWindow(FALSE);	//라인 굵기 정적 텍스트
		psDrawProp->m_sThicknessEdit.EnableWindow(FALSE);	//라인 굵기 에디트 컨트롤
		psDrawProp->m_sStcStartCap.EnableWindow(FALSE);		//라인 시작 모양 정적 텍스트
		psDrawProp->m_sStartCapCombo.EnableWindow(FALSE);	//라인 시작 모양 콤보박스
		psDrawProp->m_sStcEndCap.EnableWindow(FALSE);		//라인 끝 모양 정적 텍스트
		psDrawProp->m_sEndCapCombo.EnableWindow(FALSE);		//라인 끝 모양 콤보박스
		psDrawProp->m_sStcLineJoin.EnableWindow(FALSE);		//외곽선 모양 정적 텍스트
		psDrawProp->m_sLineJoinCombo.EnableWindow(FALSE);	//외곽선 모양 콤보박스
		psDrawProp->m_sAlphaSpin.EnableWindow(FALSE);		//불투명도 스핀
		psDrawProp->m_sStcOpacity.EnableWindow(FALSE);		//불투명도 정적 텍스트
		psDrawProp->m_sAlphaEdit.EnableWindow(FALSE);		//불투명도 에디트 컨트롤

		psDrawProp->m_sFaceTypeFillOnly.EnableWindow(FALSE);
		psDrawProp->m_sFaceTypeOutline.EnableWindow(FALSE);
		psDrawProp->m_sFaceTypeOutlinedFill.EnableWindow(FALSE);

		psDrawProp->m_sFillTypeSolid.EnableWindow(FALSE);
		psDrawProp->m_sFillTypeHatch.EnableWindow(FALSE);
		psDrawProp->m_sFillTypeGradient.EnableWindow(FALSE);
		break;
	}
}

//-------------------------------------------------------------------------
//　폰트 설정
//-------------------------------------------------------------------------
void CGraphicEditorView::OnDrawpropertyFont()
{
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(m_bExternalCmd == FALSE)
	{
		CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
		pMainFrm->m_dlgDrawProperty.OnBnClickedButtonFont();
	}
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	if(m_bsSelectMode == TRUE) //단일 선택일 경우
	{
		switch(psDoc->m_sCurrObject->GetType())
		{
		case TEXT:
			{
				GText* psGText = (GText*)psDoc->m_sCurrObject;
				psGText->SetLogFont(CDrawPropertyValue::m_sLogFont); //선택된 그래피컬 개체의 폰트를 바꿈
			}
			break;
		case GGROUP:
			{
				GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetLogFont(CDrawPropertyValue::m_sLogFont); //그룹 내 그래피컬 개체의 폰트를 바꿈
			}
			break;
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE)
	{
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos){
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			switch(psGObj->GetType())
			{
			case TEXT:
				{
					GText* psGText = (GText*)psGObj;
					psGText->SetLogFont(CDrawPropertyValue::m_sLogFont); //선택된 그래피컬 개체들의 폰트를 바꿈
				}
				break;

			case GGROUP:
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetLogFont(CDrawPropertyValue::m_sLogFont); //그룹 내 그래피컬 개체들의 채우기색을 바꿈
				break;
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	m_bExternalCmd = FALSE;
	Invalidate();
}

//-------------------------------------------------------------------------
//　폰트 설정 업데이트
//-------------------------------------------------------------------------
void CGraphicEditorView::OnUpdateDrawpropertyFont(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

//-------------------------------------------------------------------------
//　불투명도 변경 메시지 처리
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeAlpha(WPARAM wParam, LPARAM lParam)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	if(m_bsSelectMode == TRUE) //단일 선택일 경우
	{
		if(psDoc->m_sCurrObject->GetType() == GGROUP) //그룹인 경우
		{
			GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
			psGGroup->SetGroupAlpha(CDrawPropertyValue::m_nsAlpha);
		}
		else //그룹이 아닌 경우
			psDoc->m_sCurrObject->SetAlpha(CDrawPropertyValue::m_nsAlpha);

		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택일 경우
	{
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
		while(pos)
		{
			GObject* psGObj = psDoc->m_sSelectedList.GetPrev(pos);
			if(psGObj->GetType() == GGROUP) //그룹인 경우
			{
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetGroupAlpha(CDrawPropertyValue::m_nsAlpha);
			}
			else //그룹이 아닌 경우
				psGObj->SetAlpha(CDrawPropertyValue::m_nsAlpha);
		}

		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	pMainFrm->m_dlgDrawProperty.SetAlphaSpin();
	Invalidate();
	return 0;
}

//-------------------------------------------------------------------------
//　외곽선 처리(LineJoin) 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeLineJoin(WPARAM wParam, LPARAM lParam)
{
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	if(m_bsSelectMode == TRUE) //단일 선택일 경우
	{
		switch(psDoc->m_sCurrObject->GetType())
		{
		case RECTANGLE: //사각형
			{
				GRectangle* psGRectangle = (GRectangle*)psDoc->m_sCurrObject;
				psGRectangle->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
			}
			break;
		case ELLIPSE: //타원형
			{
				GEllipse* psGEllipse = (GEllipse*)psDoc->m_sCurrObject;
				psGEllipse->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
			}
			break;
		case GGROUP: //그룹
			{
				GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
			}
			break;
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택일 경우
	{
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition();
		while(pos)
		{
			GObject* psGObj = psDoc->m_sSelectedList.GetPrev(pos);
			switch(psGObj->GetType())
			{
			case RECTANGLE: //사각형
				{
					GRectangle* psGRectangle = (GRectangle*)psGObj;
					psGRectangle->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				}
				break;
			case ELLIPSE: //타원
				{
					GEllipse* psGEllipse = (GEllipse*)psGObj;
					psGEllipse->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				}
				break;
			case GGROUP: //그룹
				{
					GGroup* psGGroup = (GGroup*)psGObj;
					psGGroup->SetLineJoinStyle(CDrawPropertyValue::m_nsLineJoin);
				}
				break;
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	Invalidate();
	return 0;
}

//-------------------------------------------------------------------------
//　라인 굵기 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeLineWeight(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	//단일 선택의 경우
	if(m_bsSelectMode == TRUE) {
		if(psDoc->m_sCurrObject->GetType() == GGROUP){ //그룹일 경우
			GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
			psGGroup->SetGroupThickness(CDrawPropertyValue::m_nsThickness);
		}

		// 현재 선택된 객체가 TEXT가 아니라면
		// 두께 설정가능
		else if(psDoc->m_sCurrObject->GetType() != TEXT) {
			psDoc->m_sCurrObject->SetThickness(CDrawPropertyValue::m_nsThickness);
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	//다중 선택의 경우
	else if(m_bsMultiSelectMode == TRUE) {
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos) {
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			if(psGObj->GetType() == GGROUP){ //그룹일 경우
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetThickness(CDrawPropertyValue::m_nsThickness); //점선으로 영역을 제대로 그리기 위함
				psGGroup->SetGroupThickness(CDrawPropertyValue::m_nsThickness);
			}
			else if(psGObj->GetType() != TEXT) {
				psGObj->SetThickness(CDrawPropertyValue::m_nsThickness);
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	Invalidate();
	return 0;
}

//-------------------------------------------------------------------------
//　라인 시작 모양 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeLineCapStart(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	
	if(m_bsSelectMode == TRUE) //단일 선택의 경우
	{
		switch(psDoc->m_sCurrObject->GetType())
		{
		case LINE: //라인
			{
				GLine* psGLine = (GLine*)psDoc->m_sCurrObject;
				psGLine->SetStartCap(CDrawPropertyValue::m_nsStartCap);
			}
			break;
		case POLYLINE: //폴리 라인
			{
				GPolyline* psGPolyline = (GPolyline*)psDoc->m_sCurrObject;
				psGPolyline->SetStartCap(CDrawPropertyValue::m_nsStartCap);
			}
			break;
		case GGROUP: //그룹
			{
				GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetStartCap(CDrawPropertyValue::m_nsStartCap);
			}
			break;		
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택일 경우
	{
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos)
		{
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			switch(psDoc->m_sCurrObject->GetType())
			{
			case LINE: //라인
				{
					GLine* psGLine = (GLine*)psGObj;
					psGLine->SetStartCap(CDrawPropertyValue::m_nsStartCap);
				}
				break;
			case POLYLINE: //폴리라인
				{
					GPolyline* psGPolyline = (GPolyline*)psGObj;
					psGPolyline->SetStartCap(CDrawPropertyValue::m_nsStartCap);
				}
				break;
			case GGROUP: //그룹
				{
					GGroup* psGGroup = (GGroup*)psGObj;
					psGGroup->SetStartCap(CDrawPropertyValue::m_nsStartCap);
				}
				break;		
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	Invalidate();

	return 0;
}

//-------------------------------------------------------------------------
//　라인 끝 모양 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeLineCapEnd(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기

	if(m_bsSelectMode == TRUE) //단일 선택의 경우
	{
		switch(psDoc->m_sCurrObject->GetType())
		{
		case LINE:
			{
				GLine* psGLine = (GLine*)psDoc->m_sCurrObject;
				psGLine->SetEndCap(CDrawPropertyValue::m_nsEndCap);
			}
			break;
		case POLYLINE:
			{
				GPolyline* psGPolyline = (GPolyline*)psDoc->m_sCurrObject;
				psGPolyline->SetEndCap(CDrawPropertyValue::m_nsEndCap);
			}
			break;
		case GGROUP:
			{
				GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
				psGGroup->SetEndCap(CDrawPropertyValue::m_nsEndCap);
			}
			break;		
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	else if(m_bsMultiSelectMode == TRUE) //다중 선택일 경우
	{
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos)
		{
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			switch(psDoc->m_sCurrObject->GetType())
			{
			case LINE:
				{
					GLine* psGLine = (GLine*)psGObj;
					psGLine->SetEndCap(CDrawPropertyValue::m_nsEndCap);
				}
				break;
			case POLYLINE:
				{
					GPolyline* psGPolyline = (GPolyline*)psGObj;
					psGPolyline->SetEndCap(CDrawPropertyValue::m_nsEndCap);
				}
				break;
			case GGROUP:
				{
					GGroup* psGGroup = (GGroup*)psGObj;
					psGGroup->SetEndCap(CDrawPropertyValue::m_nsEndCap);
				}
				break;		
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	Invalidate();

	return 0;
}

//-------------------------------------------------------------------------
//　라인 컬러 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeLineColor(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	//라인 색 변경
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	//다중 선택의 경우
	if(m_bsMultiSelectMode == TRUE) {
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos){
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			//그룹이 아닐 경우
			if(psGObj->GetType() != GGROUP)
				psGObj->SetLineColor(CDrawPropertyValue::m_sLineColor); //선택된 그래피컬 개체들의 색을 바꿈
			//그룹일 경우
			else
			{
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetGroupLineColor(CDrawPropertyValue::m_sLineColor); //그룹 내 그래피컬 개체들의 색을 바꿈
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	//단일 선택의 경우
	else if(m_bsSelectMode == TRUE)
	{
		//그룹이 아닐 경우
		if(psDoc->m_sCurrObject->GetType() != GGROUP)
			psDoc->m_sCurrObject->SetLineColor(CDrawPropertyValue::m_sLineColor); //현재 그래피컬 개체의 색만 바꿈
		//그룹일 경우
		else
		{
			GGroup* psGGroup = (GGroup*)psDoc->m_sCurrObject;
			psGGroup->SetGroupLineColor(CDrawPropertyValue::m_sLineColor); //그룹 내 그래피컬 개체들의 색을 바꿈
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	ppMainFrame->m_wndStatusBar.SetPaneBackgroundColor(2, CDrawPropertyValue::m_sLineColor);
	COLORREF invertColor = CDrawPropertyValue::m_sLineColor ^ 0x00FFFFFF; //색반전
	ppMainFrame->m_wndStatusBar.SetPaneTextColor(2, invertColor);
	Invalidate();

	return 0;
}

//-------------------------------------------------------------------------
//　채우기 색 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeFillColor(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기

	GRectangle *psGRectangle;
	GEllipse *psGEllipse;
	GText *psGText;
	GGroup *psGGroup;

	// 다중선택
	if(m_bsMultiSelectMode == TRUE) {
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos){
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			switch(psGObj->GetType()) {

			case RECTANGLE:
				psGRectangle = (GRectangle *)psGObj;
				psGRectangle->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
				break;

			case ELLIPSE:
				psGEllipse = (GEllipse *)psGObj;
				psGEllipse->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
				break;

			case TEXT:
				psGText = (GText*)psGObj;
				psGText->SetBgColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
				break;

			case GGROUP:
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //그룹 내 그래피컬 개체들의 채우기색을 바꿈
				break;
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	// 단일선택시
	else if(m_bsSelectMode == TRUE) {
		switch(psDoc->m_sCurrObject->GetType()) {

		case RECTANGLE :
			psGRectangle = (GRectangle *)psDoc->m_sCurrObject;
			psGRectangle->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체의 채우기색을 바꿈
			break;

		case ELLIPSE :
			psGEllipse = (GEllipse *)psDoc->m_sCurrObject;
			psGEllipse->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체의 채우기색을 바꿈
			break;

		case TEXT:
			psGText = (GText*)psDoc->m_sCurrObject;
			psGText->SetBgColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체의 채우기색을 바꿈
			break;

		case GGROUP:
			psGGroup = (GGroup*)psDoc->m_sCurrObject;
			psGGroup->SetBrushColor(CDrawPropertyValue::m_sBrushColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
			break;
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	ppMainFrame->m_wndStatusBar.SetPaneBackgroundColor(3, CDrawPropertyValue::m_sBrushColor);
	COLORREF invertColor = CDrawPropertyValue::m_sBrushColor ^ 0x00FFFFFF; //색반전
	ppMainFrame->m_wndStatusBar.SetPaneTextColor(3, invertColor);
	Invalidate();

	return 0;
}

//-------------------------------------------------------------------------
//　그라디언트 컬러 변경
//-------------------------------------------------------------------------
afx_msg LRESULT CGraphicEditorView::OnChangeGrdtColor(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *ppMainFrame = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기

	GRectangle *psGRectangle;
	GEllipse *psGEllipse;
	GGroup *psGGroup;

	// 다중선택
	if(m_bsMultiSelectMode == TRUE) {
		POSITION pos = psDoc->m_sSelectedList.GetTailPosition(); //선택된 그래피컬 개체들이 저장된 리스트를 순회
		while(pos){
			GObject* psGObj = (GObject*)psDoc->m_sSelectedList.GetPrev(pos);
			switch(psGObj->GetType()) {

			case RECTANGLE:
				psGRectangle = (GRectangle *)psGObj;
				psGRectangle->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
				break;

			case ELLIPSE:
				psGEllipse = (GEllipse *)psGObj;
				psGEllipse->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
				break;

			case GGROUP:
				GGroup* psGGroup = (GGroup*)psGObj;
				psGGroup->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //그룹 내 그래피컬 개체들의 채우기색을 바꿈
				break;
			}
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}

	// 단일선택시
	else if(m_bsSelectMode == TRUE) {
		switch(psDoc->m_sCurrObject->GetType()) {

		case RECTANGLE :
			psGRectangle = (GRectangle *)psDoc->m_sCurrObject;
			psGRectangle->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //선택된 그래피컬 개체의 채우기색을 바꿈
			break;

		case ELLIPSE :
			psGEllipse = (GEllipse *)psDoc->m_sCurrObject;
			psGEllipse->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //선택된 그래피컬 개체의 채우기색을 바꿈
			break;

		case GGROUP:
			psGGroup = (GGroup*)psDoc->m_sCurrObject;
			psGGroup->SetGrdEndColor(CDrawPropertyValue::m_sGradientEndColor); //선택된 그래피컬 개체들의 채우기색을 바꿈
			break;
		}
		psDoc->CheckPoint(); //Undo 가능함을 알림
		GetDocument()->SetModifiedFlag(); //도큐먼트에 변경되었음을 알림
	}
	Invalidate();

	return 0;
}

//-------------------------------------------------------------------------
//　사이즈 조정 시 스타일 다이얼로그들이 윈도우 프레임 우측에 붙도록함
//-------------------------------------------------------------------------
void CGraphicEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect clientRect;
	GetClientRect(clientRect);
	ClientToScreen(&clientRect);

	//채우기 스타일 다이얼로그를 윈도우 프레임 우측에 붙도록
	if(m_dlgBrushPtn != NULL)
	{
		CRect dlgRect;
		m_dlgBrushPtn->GetWindowRect(dlgRect);

		int x = clientRect.right - dlgRect.Width(),
			y = clientRect.top + 208;
		m_dlgBrushPtn->SetWindowPos(NULL, x,y,0,0,SWP_NOZORDER | SWP_NOSIZE);
	}

	//라인 스타일 다이얼로그를 윈도우 프레임 우측에 붙도록
	if(m_dlgLinePtn != NULL)
	{
		CRect dlgRect;
		m_dlgLinePtn->GetWindowRect(dlgRect);

		int x = clientRect.right - dlgRect.Width(),
			y = clientRect.top;
		m_dlgLinePtn->SetWindowPos(NULL, x,y,0,0,SWP_NOZORDER | SWP_NOSIZE);
	}
}

//-------------------------------------------------------------------------
//　선택된 개체의 정보를 속성 다이얼로그로 전송
//-------------------------------------------------------------------------
void CGraphicEditorView::DrawPropertyInfoSetting(void)
{
	CMainFrame *psMainFrame = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorDoc* psDoc = GetDocument(); //도큐먼트 얻어오기
	
	GObject* psGObj;
	if(m_bsSelectMode == TRUE)
	{
		psGObj = psDoc->m_sCurrObject;
		if(psGObj->GetType() != GGROUP)
		{
			CDrawPropertyValue::m_nsAlpha = psGObj->GetAplha();
			CDrawPropertyValue::m_nsThickness = psGObj->GetThickness();
			CDrawPropertyValue::m_sLineColor = psGObj->GetLineColor();

			psMainFrame->m_dlgDrawProperty.SetAlphaSpin();
			psMainFrame->m_dlgDrawProperty.SetLineWeightSpin();
			psMainFrame->m_dlgDrawProperty.m_sLineColorBtn.Invalidate();
		}
		switch(psGObj->GetType())
		{
		case LINE: //라인일 경우
			{
				GLine* psGLine = (GLine*)psGObj;
				CDrawPropertyValue::m_nsStartCap = psGLine->GetStartCap();
				CDrawPropertyValue::m_nsEndCap = psGLine->GetEndCap();
				CDrawPropertyValue::m_nsPenStyle = psGLine->GetPenStyle();
				psMainFrame->m_dlgDrawProperty.SetStartCap(CDrawPropertyValue::m_nsStartCap, TRUE);
				psMainFrame->m_dlgDrawProperty.SetEndCap(CDrawPropertyValue::m_nsEndCap, TRUE);
				psMainFrame->m_dlgDrawProperty.SetLinePattern();

				//우측 라인 스타일 다이얼로그를 바꿈
				if(m_dlgLinePtn != NULL)
				{
					m_dlgLinePtn->SetStartCap();
					m_dlgLinePtn->SetEndCap();
					m_dlgLinePtn->SetLinePattern();
				}
			}
			break;
		case POLYLINE: //폴리 라인일 경우
			{
				GPolyline* psGPolyline = (GPolyline*)psGObj;
				CDrawPropertyValue::m_nsStartCap = psGPolyline->GetStartCap();
				CDrawPropertyValue::m_nsEndCap = psGPolyline->GetEndCap();
				CDrawPropertyValue::m_nsPenStyle = psGPolyline->GetPenStyle();
				psMainFrame->m_dlgDrawProperty.SetStartCap(CDrawPropertyValue::m_nsStartCap, TRUE);
				psMainFrame->m_dlgDrawProperty.SetEndCap(CDrawPropertyValue::m_nsEndCap, TRUE);
				psMainFrame->m_dlgDrawProperty.SetLinePattern();

				//우측 라인 스타일 다이얼로그를 바꿈
				if(m_dlgLinePtn != NULL)
				{
					m_dlgLinePtn->SetStartCap();
					m_dlgLinePtn->SetEndCap();
					m_dlgLinePtn->SetLinePattern();
				}
			}
			break;
		case RECTANGLE: //사각형일 경우
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				CDrawPropertyValue::m_nsPenStyle = psGRectangle->GetPenStyle();
				CDrawPropertyValue::m_sBrushColor = psGRectangle->GetFillColor();
				CDrawPropertyValue::m_nsLineJoin = psGRectangle->GetLineJoin();
				CDrawPropertyValue::m_nsBrushStyle = psGRectangle->GetBrushStyle();
				if(CDrawPropertyValue::m_nsBrushStyle == 1)
				{
					CDrawPropertyValue::m_nsHatchStyle = psGRectangle->GetHatchStyle();
					psMainFrame->m_dlgDrawProperty.SetHatchPattern();
					if(m_dlgBrushPtn != NULL)
						m_dlgBrushPtn->SetHatchPattern();
				}
				else if(CDrawPropertyValue::m_nsBrushStyle == 2)
				{
					CDrawPropertyValue::m_sGradientEndColor = psGRectangle->GetGradientColor();
					psMainFrame->m_dlgDrawProperty.m_sGradientEndColorBtn.Invalidate();
					CDrawPropertyValue::m_nsGradientType = psGRectangle->GetGradientMode();
					psMainFrame->m_dlgDrawProperty.SetGradientPattern(CDrawPropertyValue::m_nsGradientType);
					if(m_dlgBrushPtn != NULL)
						m_dlgBrushPtn->SetGradientPattern();
				}
				CDrawPropertyValue::m_nsFaceType = psGRectangle->GetFaceType();
				psMainFrame->m_dlgDrawProperty.SetFillStyle(CDrawPropertyValue::m_nsBrushStyle);
				psMainFrame->m_dlgDrawProperty.SetLinePattern();
				psMainFrame->m_dlgDrawProperty.m_sFillColorBtn.Invalidate();
				psMainFrame->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
				psMainFrame->m_dlgDrawProperty.SetLineJoin(CDrawPropertyValue::m_nsLineJoin);
				psMainFrame->m_dlgDrawProperty.AdjustControl();
				if(m_dlgBrushPtn != NULL)
				{
					m_dlgBrushPtn->SetDrawStyle();
					m_dlgBrushPtn->SetFillStyle();
					m_dlgBrushPtn->SetLineJoin();
				}
				if(m_dlgLinePtn != NULL)
					m_dlgLinePtn->SetLinePattern();
			}
			break;
		case ELLIPSE: //타원일 경우
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				CDrawPropertyValue::m_nsPenStyle = psGEllipse->GetPenStyle();
				CDrawPropertyValue::m_sBrushColor = psGEllipse->GetFillColor();
				CDrawPropertyValue::m_nsLineJoin = psGEllipse->GetLineJoin();
				CDrawPropertyValue::m_nsBrushStyle = psGEllipse->GetBrushStyle();
				if(CDrawPropertyValue::m_nsBrushStyle == 1)
				{
					CDrawPropertyValue::m_nsHatchStyle = psGEllipse->GetHatchStyle();
					psMainFrame->m_dlgDrawProperty.SetHatchPattern();
					if(m_dlgBrushPtn != NULL)
						m_dlgBrushPtn->SetHatchPattern();
				}
				else if(CDrawPropertyValue::m_nsBrushStyle == 2)
				{
					CDrawPropertyValue::m_sGradientEndColor = psGEllipse->GetGradientColor();
					psMainFrame->m_dlgDrawProperty.m_sGradientEndColorBtn.Invalidate();
					CDrawPropertyValue::m_nsGradientType = psGEllipse->GetGradientMode();
					psMainFrame->m_dlgDrawProperty.SetGradientPattern(CDrawPropertyValue::m_nsGradientType);
					if(m_dlgBrushPtn != NULL)
						m_dlgBrushPtn->SetGradientPattern();
				}
				CDrawPropertyValue::m_nsFaceType = psGEllipse->GetFaceType();
				psMainFrame->m_dlgDrawProperty.SetFillStyle(CDrawPropertyValue::m_nsBrushStyle);
				psMainFrame->m_dlgDrawProperty.SetLinePattern();
				psMainFrame->m_dlgDrawProperty.m_sFillColorBtn.Invalidate();
				psMainFrame->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
				psMainFrame->m_dlgDrawProperty.SetLineJoin(CDrawPropertyValue::m_nsLineJoin);
				psMainFrame->m_dlgDrawProperty.AdjustControl();
				if(m_dlgBrushPtn != NULL)
				{
					m_dlgBrushPtn->SetDrawStyle();
					m_dlgBrushPtn->SetFillStyle();
					m_dlgBrushPtn->SetLineJoin();
				}
				if(m_dlgLinePtn != NULL)
					m_dlgLinePtn->SetLinePattern();
			}
			break;
		}
	}

}

//새 문서 시 버그 수정
void CGraphicEditorView::InitializeAllVariable(void)
{
	m_bsDrawMode = m_bsSelectMode = m_bsPolyDraw = m_bsDBClicked = 
	m_bsPointSelected = m_bsMakeFocusRect = m_bsMultiSelectMode = 
	m_nsSelectPtIndex = m_bExternalCmd = m_bsMove = m_bsResize = FALSE;
}
