/* [ GGroup Ŭ���� by ����ö ]
** �׷����� ��ü�� �׷�� ���õ� Ŭ������ �׷��� ������ �Ӽ��� ������ �ֽ��ϴ�.
*/
#pragma once
#include "gobject.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GRectangle.h"
#include "GEllipse.h"
#include "GText.h"
#include "QuickSort.h"

// GGroup Ŭ����
class GGroup : public GObject
{
protected:
	DECLARE_SERIAL(GGroup);
public:
	//������ �� �Ҹ���
	GGroup(void);					//������
	GGroup(const GGroup& pGGroup);	//���� ������
	~GGroup(void);					//�Ҹ���

	//����ȭ
	virtual void Serialize(CArchive& ar);	//����ȭ

	//�׷�ȭ ���� �Լ�
	void Add(CTypedPtrList<CObList, GObject*>& listGObj);			//�߰�
	void Remove(CTypedPtrList<CObList, GObject*>& listGObj);		//����
	void UnGroup(CTypedPtrList<CObList, GObject*>* listGObj);		//�׷� ����
	void UnGroupChild(GGroup& childGroup, CTypedPtrList<CObList, GObject*>* listGObj);	//�׷� ����(���ϵ�)
	void GetChild(CTypedPtrList<CObList, GObject*>* listGObj);		//���ϵ� �׷� ��������
	BOOL IsLeaf(void);												//�������� �Ǵ�

	//Accessor �Լ�
	void GetGroupData(CTypedPtrList<CObList, GObject*>* listGObj);	//�׷� �����͸� listGObj�� ����(��, ��ȯ���� listGObj)
	virtual CRgn* GetRegion();										//�׷��� ���� ��ȯ

	//Mutator �Լ�
	virtual void Move(int dX, int dY);
	virtual void Resize(int index, int dX, int dY);
	int FindSelectPoint(CPoint pt);

	void SetGroupLineColor(COLORREF color);				// ���� �÷� ����
	void SetGroupThickness(int thickness);				// ���� �β� ����
	void SetPenStyle(int penStyle);						// �� ��Ÿ�� ����
	void SetBrushColor(COLORREF brushColor);			// �귯�� �÷� ����

	void SetStartCap(int startCap);						// �� ���� ��� ����
	void SetEndCap(int endCap);							// �� �� ��� ����

	void SetFaceType(int faceType);						// �� Ÿ�� ����
	void SetBrushStyle(int brushStyle);					// �귯�� ��Ÿ�� ����
	void SetHatchStyle(int hatch);						// ��ġ ��Ÿ�� ����
	void SetLineJoinStyle(int lineJoin);				// ���� ���� ó�� ����
	void SetGradientMode(int grdMode);					// �׶���Ʈ ��� ����
	void SetGrdEndColor(COLORREF grdEndColor);			// �׶��̼��� �� �� ����

	void SetGroupAlpha(int nAlpha);						// ������ ����
	void SetLogFont(LOGFONT lf);						// �α� ��Ʈ ����

	//�׸��� �Լ�
	virtual void Draw(CDC* cdc);							//�� �Լ����� �׷����� ��ü �׸�
	virtual void DrawSelectLine(CDC *pDC);					//���� �� �������� ������ �׸�

	// �׷�ȭ ������¡ �Լ� by ������
	void StartResizing(int x, int y, int direction);		// ������¡ ���� ����
	void DoResizing(int x, int y);
	void StartChildResizing(int x, int y, int width, int height, int direction);

	int m_pResizeX, m_pReszieY;
	int m_pResizeWidth, m_pResizeHeight;
	int m_pResizeDirection;

private:
	void FindStartEndPt(); //������ �� ���ϵ� ��带 ���� ��ȸ�Ͽ� leftTop Point�� rightBottom Point�� ã��
	void MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX);
	CTypedPtrList<CObList, GObject*> m_sNodeData; //������ ���
	CTypedPtrList<CObList, GGroup*> m_sChild;	//���ϵ� ���
};
