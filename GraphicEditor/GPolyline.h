/* [ GPolyline Ŭ���� by ����ö ]
** GPolyline Ŭ������ �������ο� ���� ������ �Ӽ��� ���� Ŭ�����Դϴ�.
*/

#pragma once
#include "GObject.h"
#include "QuickSort.h"

class GPolyline : public GObject
{
protected:
	DECLARE_SERIAL(GPolyline);
public:
	GPolyline(void);						//������
	GPolyline(const GPolyline& pPolyline);	//���� ������

	virtual void Serialize(CArchive& ar);	//����ȭ

	//Accessor �Լ�
	virtual CRgn* GetRegion(void);								//������ ����
	int GetPenStyle() const {return m_nsPenStyle; }	//�� ��Ÿ�� ��ȯ
	virtual CPoint GetEndPoint(void) const;						//�� �� ��ȯ
	CArray<CPoint, POINT>* GetArrayPoint(void);					//��ǥ ������ ��ȯ
	int FindSelectPoint(CPoint pt);								//���� ������ ��ǥ ������ �ε��� ��ȯ
	CArray<CPoint, POINT>* GetRgnPoint(void);					//���� ����Ʈ ��ȯ
	void FindStartEndPt(void);									//�׷�ȭ �� ����� ����,������ ã��
	void MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX); //x,y��ǥ�� �迭�� ����
	int FindFixedPoint(int nCoord, BOOL bIsX);
	int GetStartCap(void) const {return m_nsStartCap; } //���� ���� ��� ��ȯ
	int GetEndCap(void) const {return m_nsEndCap; } //���� �� ��� ��ȯ
	
	//Mutator �Լ�
	void AddOnePt(CPoint pt);									//���� �׸� �� �� �߰�
	void SetPolyStartPoint(CPoint pt);							//������ ����
	void SetPolyEndPoint(CPoint pt);							//���� ����
	virtual void Move(int dX, int dY);							//��ü ��ǥ �̵�
	void Resize(int index, int dX, int dY);						//�� �� �̵�
	BOOL DelOnePt(int index);									//�� �����

	void SetPenStyle(int penStyle);								//�� ��Ÿ�� ����
	void SetStartCap(int startCap);								//�� ���� ��� ����
	void SetEndCap(int endCap);									//�� �� ��� ����

	virtual void Draw(CDC* cdc);								//�� �Լ����� �׷����� ��ü �׸�
	virtual void DrawSelectLine(CDC *pDC);						// ���� �� �������� ������ �׸�
	void DrawingLine(CDC* pDC, CPoint point);					//�׸��� ���߿� ������ �Ӽ����� Ŀ���� ���� ������ �׸�

	//���������� �׷�ȭ �������� �� ���
	void SavePolylinePoints();
	void ApplyPolylinePoints(double px, double py, int sx, int sy);

protected:
	CArray<CPoint, POINT> m_sArrayPoint;	//��ǥ ������
	CArray<CPoint, POINT> m_sPrevArrayPoint;	//���� ��ǥ ������
	int m_nsPenStyle;						//�� ��Ÿ�� 
	CArray<CPoint, POINT> m_sArrayRgnPoint;	//���� ��ǥ

	//for GDI+, 120528 �߰� by ����ö
	int m_nsStartCap;	//���� ���� ���
	int m_nsEndCap;		//���� �� ���
public:

	void DrawSelectedPoint(CDC* pDC, int index);
};

