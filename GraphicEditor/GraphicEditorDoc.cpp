
// GraphicEditorDoc.cpp : CGraphicEditorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GraphicEditor.h"
#endif

#include "GraphicEditorDoc.h"
#include "GraphicEditorView.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGraphicEditorDoc

IMPLEMENT_DYNCREATE(CGraphicEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphicEditorDoc, CDocument)
END_MESSAGE_MAP()


// CGraphicEditorDoc 생성/소멸

CGraphicEditorDoc::CGraphicEditorDoc()
{
	m_sCurrType = SELECT;
}

CGraphicEditorDoc::~CGraphicEditorDoc()
{
	POSITION pos = m_sGObjectList.GetHeadPosition();
	while(pos)
	{
		delete m_sGObjectList.GetNext(pos);
	}
	m_sGObjectList.RemoveAll();
}

BOOL CGraphicEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	//현재 그래피컬 개체 초기화
	m_sCurrObject = NULL;
	//그래피컬 개체 리스트 초기화
	POSITION pos = m_sGObjectList.GetHeadPosition();
	while(pos)
		delete m_sGObjectList.GetNext(pos);
	m_sGObjectList.RemoveAll();

	//선택 개체 리스트 초기화
	m_sSelectedList.RemoveAll();
	
	//뷰영역의 변수들 초기화
	CMainFrame* psMainFrame = (CMainFrame*) AfxGetMainWnd();
	CGraphicEditorView* psView = (CGraphicEditorView*)psMainFrame->GetActiveView();
	if(psView->GetSafeHwnd())
		psView->InitializeAllVariable();

	this->CheckPoint();

	return TRUE;
}


// CGraphicEditorDoc serialization

void CGraphicEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_sGObjectList.Serialize(ar);
	}
	else
	{
		m_sGObjectList.Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGraphicEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGraphicEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGraphicEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGraphicEditorDoc 진단

#ifdef _DEBUG
void CGraphicEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphicEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGraphicEditorDoc 명령

//-------------------------------------------------------------------------
//　그래피컬 개체들(라인, 폴리라인, 사각형, 등)을 반환
//-------------------------------------------------------------------------
//라인
GLine* CGraphicEditorDoc::GetLine(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GLine(); //새로운 GLine 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GLine*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == LINE) //현재 타입이 라인이고, NULL이 아닐 때
		return (GLine*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}

//폴리라인
GPolyline* CGraphicEditorDoc::GetPolyLine(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GPolyline(); //새로운 GPolyline 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GPolyline*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == POLYLINE) //현재 타입이 폴리라인이고, NULL이 아닐 때
		return (GPolyline*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}

//사각형
GRectangle* CGraphicEditorDoc::GetRectangle(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GRectangle(); //새로운 GRectangle 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GRectangle*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == RECTANGLE) //현재 타입이 사각형이고, NULL이 아닐 때
		return (GRectangle*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}

//타원
GEllipse* CGraphicEditorDoc::GetEllipse(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GEllipse(); //새로운 GEllipse 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GEllipse*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == ELLIPSE) //현재 타입이 타원이고, NULL이 아닐 때
		return (GEllipse*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}

//텍스트
GText* CGraphicEditorDoc::GetText(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GText(); //새로운 GText 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GText*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == TEXT) //현재 타입이 텍스트이고, NULL이 아닐 때
		return (GText*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}

//그룹
GGroup* CGraphicEditorDoc::GetGroup(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GGroup(); //새로운 그룹 객체 생성
		m_sGObjectList.AddTail(m_sCurrObject); //리스트에 추가

		return (GGroup*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == SELECT) //현재 타입이 SELECT이고, NULL이 아닐 때
		return (GGroup*)m_sCurrObject;

	return NULL; //잘못된 사용 시에는 NULL을 반환하도록 함
}
//-------------------------------------------------------------------------
//　point 좌표가 선택된 Gobejct에 존재하는지 검사하는 함수
//-------------------------------------------------------------------------
BOOL CGraphicEditorDoc::IsExistSelectedGObject(CPoint point)
{
	POSITION pos = m_sGObjectList.GetTailPosition();
	while(pos)
	{
		GObject* pGObj = (GObject*)m_sGObjectList.GetPrev(pos);
		if(pGObj->PtInGObj(point) == TRUE)
		{
			m_sCurrObject = pGObj;
			return TRUE;
		}
	}
	return FALSE;
}
//-------------------------------------------------------------------------
//　점 하나 찾기
//-------------------------------------------------------------------------
int CGraphicEditorDoc::FindSelectPoint(CPoint point)
{
	POSITION pos = m_sGObjectList.GetTailPosition();
	while(pos)
	{
		GObject* pGObj = (GObject*)m_sGObjectList.GetPrev(pos);
		if(pGObj->FindSelectPoint(point) != -1 && pGObj->IsGrouped() == FALSE)
		{
			m_sCurrObject = pGObj;
			return pGObj->FindSelectPoint(point);
		}
	}
	return -1;
}
//-------------------------------------------------------------------------
//　드래그한 영역에 GObject가 있는지 검사
//-------------------------------------------------------------------------
BOOL CGraphicEditorDoc::FindGObjInFocusRect(CRect rect)
{
	//여기에 각 그래피컬 개체의 점을 받아와 PtInRect 수행
	POSITION pos = m_sGObjectList.GetTailPosition();
	while(pos)
	{
		GObject* pGObj = (GObject*)m_sGObjectList.GetPrev(pos);
		switch(pGObj->GetType())
		{
		case LINE:
			if (rect.PtInRect(pGObj->GetStartPoint()) && 
				rect.PtInRect(pGObj->GetEndPoint()) &&
				pGObj->IsGrouped() == FALSE)
				m_sSelectedList.AddHead(pGObj);
			break;
		case POLYLINE:
			{
				GPolyline* pGPolyline = (GPolyline*)pGObj;
				CArray<CPoint, POINT>* ptList = pGPolyline->GetArrayPoint();
				int nTrueCount=0;
				for(int i=0;i<ptList->GetSize();i++)
				{
					if(rect.PtInRect(ptList->GetAt(i)) == TRUE)
						nTrueCount++;
				}
				if(nTrueCount == ptList->GetSize() && pGObj->IsGrouped() == FALSE)
					m_sSelectedList.AddHead(pGObj);
			}
			break;
		case RECTANGLE:
		case ELLIPSE:
		case TEXT:
		case GGROUP:
			{
				CRect tempRect(pGObj->GetStartPoint(), pGObj->GetEndPoint());
				CPoint sPointArray[8];
				sPointArray[0] = tempRect.TopLeft();
				sPointArray[1] = CPoint(tempRect.left, tempRect.CenterPoint().y);
				sPointArray[2] = CPoint(tempRect.left, tempRect.bottom);
				sPointArray[3] = CPoint(tempRect.CenterPoint().x, tempRect.bottom);
				sPointArray[4] = CPoint(tempRect.BottomRight());
				sPointArray[5] = CPoint(tempRect.right, tempRect.CenterPoint().y);
				sPointArray[6] = CPoint(tempRect.right, tempRect.top);
				sPointArray[7] = CPoint(tempRect.CenterPoint().x, tempRect.top);
				int nTrueCount = 0;
				for(int i=0;i<8;i++)
				{
					if(rect.PtInRect(sPointArray[i]) == TRUE)
						nTrueCount++;
				}
				if(nTrueCount == 8 && pGObj->IsGrouped() == FALSE)
					m_sSelectedList.AddHead(pGObj);
			}
		}
	}
	
	if(m_sSelectedList.GetSize() != 0)
		return TRUE;
	else
		return FALSE;
}

//-------------------------------------------------------------------------
//　도큐먼트 내용을 파기하고 초기화
//-------------------------------------------------------------------------
void CGraphicEditorDoc::DeleteContents()
{
	while(!m_sGObjectList.IsEmpty())
		delete m_sGObjectList.RemoveHead();

	m_sSelectedList.RemoveAll();
	m_sCurrObject = NULL;

	CDocument::DeleteContents();
}

//-------------------------------------------------------------------------
//　파일 열기 시 작업
//-------------------------------------------------------------------------
BOOL CGraphicEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	this->CheckPoint();
	return TRUE;
}
