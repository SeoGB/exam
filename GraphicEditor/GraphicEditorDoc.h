
// GraphicEditorDoc.h : CGraphicEditorDoc Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CGraphicEditorDoc();
	DECLARE_DYNCREATE(CGraphicEditorDoc)

// Ư���Դϴ�.
public:
	GObject* m_sCurrObject;				//���� �׸��� �׷����� ��ü
	GraphicalObjectType m_sCurrType;	//���� �׸��� Ÿ��
	CTypedPtrList<CObList, GObject*> m_sGObjectList;	//�׸��� ���� ����Ʈ
	CPoint m_sClickedPoint;				//Ŭ�� ����

	CTypedPtrList<CObList, GObject*> m_sSelectedList; //���� ���õ� ����Ʈ(�׷�ȭ�� ���� ���)
// �۾��Դϴ�.
public:
	//�׷����� ��ü�� ��ȯ
	GLine* GetLine(BOOL bIsNew = FALSE);
	GPolyline* GetPolyLine(BOOL bIsNew = FALSE);
	GRectangle* GetRectangle(BOOL bIsNew = FALSE);
	GEllipse* GetEllipse(BOOL bIsNew = FALSE);
	GText* GetText(BOOL bIsNew = FALSE);
	GGroup* GetGroup(BOOL bIsNew = FALSE);

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CGraphicEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	BOOL IsExistSelectedGObject(CPoint point);	//point ��ǥ�� ���õ� Gobejct�� �����ϴ��� �˻��ϴ� �Լ�
	int FindSelectPoint(CPoint point);			//������ ���� ã�� �Լ�
	BOOL FindGObjInFocusRect(CRect rect);		//�巡���� ������ GObject�� �ִ��� �˻�
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
