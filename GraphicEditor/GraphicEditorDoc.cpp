
// GraphicEditorDoc.cpp : CGraphicEditorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CGraphicEditorDoc ����/�Ҹ�

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
	
	//���� �׷����� ��ü �ʱ�ȭ
	m_sCurrObject = NULL;
	//�׷����� ��ü ����Ʈ �ʱ�ȭ
	POSITION pos = m_sGObjectList.GetHeadPosition();
	while(pos)
		delete m_sGObjectList.GetNext(pos);
	m_sGObjectList.RemoveAll();

	//���� ��ü ����Ʈ �ʱ�ȭ
	m_sSelectedList.RemoveAll();
	
	//�俵���� ������ �ʱ�ȭ
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

// ����� �׸��� �����մϴ�.
void CGraphicEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CGraphicEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CGraphicEditorDoc ����

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


// CGraphicEditorDoc ���

//-------------------------------------------------------------------------
//���׷����� ��ü��(����, ��������, �簢��, ��)�� ��ȯ
//-------------------------------------------------------------------------
//����
GLine* CGraphicEditorDoc::GetLine(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GLine(); //���ο� GLine ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GLine*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == LINE) //���� Ÿ���� �����̰�, NULL�� �ƴ� ��
		return (GLine*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}

//��������
GPolyline* CGraphicEditorDoc::GetPolyLine(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GPolyline(); //���ο� GPolyline ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GPolyline*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == POLYLINE) //���� Ÿ���� ���������̰�, NULL�� �ƴ� ��
		return (GPolyline*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}

//�簢��
GRectangle* CGraphicEditorDoc::GetRectangle(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GRectangle(); //���ο� GRectangle ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GRectangle*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == RECTANGLE) //���� Ÿ���� �簢���̰�, NULL�� �ƴ� ��
		return (GRectangle*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}

//Ÿ��
GEllipse* CGraphicEditorDoc::GetEllipse(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GEllipse(); //���ο� GEllipse ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GEllipse*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == ELLIPSE) //���� Ÿ���� Ÿ���̰�, NULL�� �ƴ� ��
		return (GEllipse*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}

//�ؽ�Ʈ
GText* CGraphicEditorDoc::GetText(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GText(); //���ο� GText ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GText*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == TEXT) //���� Ÿ���� �ؽ�Ʈ�̰�, NULL�� �ƴ� ��
		return (GText*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}

//�׷�
GGroup* CGraphicEditorDoc::GetGroup(BOOL bIsNew)
{
	if(bIsNew == TRUE)
	{
		m_sCurrObject = new GGroup(); //���ο� �׷� ��ü ����
		m_sGObjectList.AddTail(m_sCurrObject); //����Ʈ�� �߰�

		return (GGroup*)m_sCurrObject;
	}
	else if(m_sCurrObject != NULL && m_sCurrType == SELECT) //���� Ÿ���� SELECT�̰�, NULL�� �ƴ� ��
		return (GGroup*)m_sCurrObject;

	return NULL; //�߸��� ��� �ÿ��� NULL�� ��ȯ�ϵ��� ��
}
//-------------------------------------------------------------------------
//��point ��ǥ�� ���õ� Gobejct�� �����ϴ��� �˻��ϴ� �Լ�
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
//���� �ϳ� ã��
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
//���巡���� ������ GObject�� �ִ��� �˻�
//-------------------------------------------------------------------------
BOOL CGraphicEditorDoc::FindGObjInFocusRect(CRect rect)
{
	//���⿡ �� �׷����� ��ü�� ���� �޾ƿ� PtInRect ����
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
//����ť��Ʈ ������ �ı��ϰ� �ʱ�ȭ
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
//������ ���� �� �۾�
//-------------------------------------------------------------------------
BOOL CGraphicEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	this->CheckPoint();
	return TRUE;
}
