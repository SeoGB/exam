
// GraphicEditorDoc.h : CGraphicEditorDoc 클래스의 인터페이스
//


#pragma once
class CGraphicEditorView;

#include "GObject.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GRectangle.h"
#include "GEllipse.h"
#include "GText.h"
#include "GGroup.h"

class CGraphicEditorDoc : public CDocument, public CUndo
{
protected: // serialization에서만 만들어집니다.
	CGraphicEditorDoc();
	DECLARE_DYNCREATE(CGraphicEditorDoc)

// 특성입니다.
public:
	GObject* m_sCurrObject;				//현재 그리는 그래피컬 개체
	GraphicalObjectType m_sCurrType;	//현재 그리기 타입
	CTypedPtrList<CObList, GObject*> m_sGObjectList;	//그리기 저장 리스트
	CPoint m_sClickedPoint;				//클릭 지점

	CTypedPtrList<CObList, GObject*> m_sSelectedList; //다중 선택된 리스트(그룹화를 위해 사용)
// 작업입니다.
public:
	//그래피컬 개체들 반환
	GLine* GetLine(BOOL bIsNew = FALSE);
	GPolyline* GetPolyLine(BOOL bIsNew = FALSE);
	GRectangle* GetRectangle(BOOL bIsNew = FALSE);
	GEllipse* GetEllipse(BOOL bIsNew = FALSE);
	GText* GetText(BOOL bIsNew = FALSE);
	GGroup* GetGroup(BOOL bIsNew = FALSE);

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGraphicEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	BOOL IsExistSelectedGObject(CPoint point);	//point 좌표가 선택된 Gobejct에 존재하는지 검사하는 함수
	int FindSelectPoint(CPoint point);			//선택한 점을 찾는 함수
	BOOL FindGObjInFocusRect(CRect rect);		//드래그한 영역에 GObject가 있는지 검사
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
