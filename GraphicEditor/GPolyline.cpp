/* [ GPolyline Ŭ���� by ����ö ]
** GPolyline Ŭ������ �������Դϴ�.
*/

#include "StdAfx.h"
#include "GPolyline.h"


IMPLEMENT_SERIAL(GPolyline, GObject, 0)
//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������
GPolyline::GPolyline(void)
{
	m_sType = POLYLINE;
}
//���� ������
GPolyline::GPolyline(const GPolyline& pPolyline)
{
	this->GObject::m_sType = pPolyline.GObject::m_sType;
	this->GObject::m_sStartPoint = pPolyline.GObject::m_sStartPoint;
	this->GObject::m_sEndPoint = pPolyline.GObject::m_sEndPoint;
	this->GObject::m_sLineColor = pPolyline.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pPolyline.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pPolyline.GObject::m_bsGrouped;

	this->m_nsPenStyle = pPolyline.m_nsPenStyle;
	
	for(int i=0;i<pPolyline.m_sArrayPoint.GetSize(); i++)
	{
		this->m_sArrayPoint.Add(pPolyline.m_sArrayPoint.GetAt(i));
	}

	for(int i=0;i<pPolyline.m_sArrayRgnPoint.GetSize(); i++)
	{
		this->m_sArrayRgnPoint.Add(pPolyline.m_sArrayRgnPoint.GetAt(i));
	}
}

//--------------------------------------------------------------------------
//������ȭ
//--------------------------------------------------------------------------
void GPolyline::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //���� ��
	{
		ar << (WORD)m_nsPenStyle;
		ar << (WORD)m_nsStartCap;
		ar << (WORD)m_nsEndCap;
		m_sArrayPoint.Serialize(ar);
	}
	else //�ε� ��
	{
		WORD wTemp;
		ar >> wTemp; m_nsPenStyle = wTemp;
		ar >> wTemp; m_nsStartCap = wTemp;
		ar >> wTemp; m_nsEndCap = wTemp;
		m_sArrayPoint.Serialize(ar);
	}
}
//--------------------------------------------------------------------------
//��Polyline�� ������ ���ϴ� �Լ�
//--------------------------------------------------------------------------
CRgn* GPolyline::GetRegion(void)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //��ǥ �������� ũ�⸦ ����
	if(nsArraySize != 0) //��ǥ �������� ������� ���� ���
	{
		//��ǥ �������� ��ȸ�ϸ� polyline ������� ������ ����
		for(int i=0;i< nsArraySize-1 ; i++)
		{
			CPoint &currPt = m_sArrayPoint.GetAt(i); //���� ��ǥ
			CPoint &nextPt = m_sArrayPoint.GetAt(i+1); //���� ��ǥ

			int x = currPt.x;
			int y = currPt.y;
			int w = nextPt.x - currPt.x; //��
			int h = nextPt.y - currPt.y; //����

			//�� �� ������ ���⸦ ����
			double theta = w ? atan2((double)w, (double)h) : (~h+1) * M_PI / 2;
			if(theta < 0)
				theta = theta + 2*M_PI;
			
			//���⸦ ������ ��ȯ
			double angle = theta + M_PI/2.0;
			
			//�� �β��� �ʹ� ������ ������ ����Ƿ� �⺻ ũ��� 5
			int dx = (int)((m_nsThickness > 4 ? m_nsThickness : 5) * sin(angle));
			int dy = (int)((m_nsThickness > 4 ? m_nsThickness : 5) * cos(angle));
			
			//������ ����� ���� 4���� ������ ����
			CPoint pt[4];
			pt[0] = CPoint(x+dx, y+dy); //���� �ϴ�
			pt[1] = CPoint(x-dx, y-dy); //���� ���
			pt[2] = CPoint(x+w-dx, y+h-dy); //���� ���
			pt[3] = CPoint(x+w+dx, y+h+dy); //���� �ϴ�
			
			//���� ��ǥ �߰�
			if(i==0)
				m_sArrayRgnPoint.RemoveAll();
			for(int j=0;j<4;j++)
				m_sArrayRgnPoint.Add(pt[j]);

			if(i==0)
			{	//ù ��° ���� ��� ���� ���� 
				m_sRgn.DeleteObject();
				m_sRgn.CreatePolygonRgn(pt, 4, WINDING);
			}
			else //�� �ܿ��� ������ ���ļ� ����
			{
				CRgn prevRgn;
				prevRgn.CreatePolygonRgn(pt, 4, WINDING);
				m_sRgn.CombineRgn(&m_sRgn, &prevRgn, RGN_OR);
			}
		}
	}
	return &m_sRgn;
}

//--------------------------------------------------------------------------
//�������� ��ȯ �Լ�
//--------------------------------------------------------------------------
CPoint GPolyline::GetEndPoint(void) const
{
	if(m_bsGrouped == FALSE)
		return m_sArrayPoint.GetAt(m_sArrayPoint.GetSize()-1);
	else
		return m_sEndPoint;
}

//--------------------------------------------------------------------------
//����ǥ ������ ��ȯ �Լ�
//--------------------------------------------------------------------------
CArray<CPoint, POINT>* GPolyline::GetArrayPoint(void)
{
	return &m_sArrayPoint; //��ǥ ������ ��ȯ
}

//--------------------------------------------------------------------------
//������ ����Ʈ�� ������ �Լ�
//--------------------------------------------------------------------------
CArray<CPoint, POINT>* GPolyline::GetRgnPoint(void)
{
	return &m_sArrayRgnPoint;
}

//--------------------------------------------------------------------------
//������ ������ ��ǥ ������ �ε��� ��ȯ
//--------------------------------------------------------------------------
int GPolyline::FindSelectPoint(CPoint pt)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //��ǥ �������� ũ�⸦ ����
	if(nsArraySize != 0)
	{
		//��ǥ �������� �ϳ��� �˻��Ͽ� ���� ��ǥ�� �� ������ ��ǥ�����β� �ȿ� ������ �ش� �������� �ε��� ��ȯ 
		for(int i=0;i<nsArraySize;i++)
		{
			CRgn rgn;
			int tempThick = (m_nsThickness > 4 ? m_nsThickness : 5); //�� �β��� �ʹ� ������ ������ ����Ƿ� �⺻ ũ��� 5 
			CPoint &tempPt = m_sArrayPoint.GetAt(i);
			rgn.CreateEllipticRgn(tempPt.x-tempThick, tempPt.y-tempThick, tempPt.x+tempThick, tempPt.y+tempThick); //���� ����
			if(rgn.PtInRegion(pt) == TRUE) //�����ȿ� ���� ��ǥ�� ���� ���
				return i; //�ش� �������� �ε��� ��ȯ
		}
	}
	return -1; //���� ��� -1
}
//--------------------------------------------------------------------------
//���ּ� left,top ��ǥ�� �ִ� right,bottom ��ǥ�� ã��: �׷�ȭ �� �ʿ�
//--------------------------------------------------------------------------
void GPolyline::FindStartEndPt()
{
	CArray<int, int&>* arrayX = new CArray<int, int&>; //x��ǥ�� ���� �迭
	CArray<int, int&>* arrayY = new CArray<int, int&>; //y��ǥ�� ���� �迭
	MakeCoordinateArray(arrayX, TRUE);
	MakeCoordinateArray(arrayY, FALSE);

	CQuickSort* qSort = new CQuickSort();
	qSort->QuickSort(arrayX, 0, arrayX->GetSize()-1);
	qSort->QuickSort(arrayY, 0, arrayY->GetSize()-1);

	//������ ����(x,y�� �迭�� ���� ���� ���� ������)
	m_sStartPoint.x = arrayX->GetAt(0);
	m_sStartPoint.y = arrayY->GetAt(0);

	//���� ����(x,y�� �迭�� ���� ū ���� ����)
	m_sEndPoint.x = arrayX->GetAt(arrayX->GetSize()-1);
	m_sEndPoint.y = arrayY->GetAt(arrayX->GetSize()-1);

	delete qSort;
	delete arrayX;
	delete arrayY;
}
//--------------------------------------------------------------------------
//��x��ǥ �Ǵ� y��ǥ�� ���� �迭�� ����
//--------------------------------------------------------------------------
void GPolyline::MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX)
{
	if(bIsX == TRUE) //x��
	{
		//�� �������� x���� ���� array�� �߰�
		for(int i=0;i<m_sArrayPoint.GetSize();i++)
		{
			int x = m_sArrayPoint.GetAt(i).x;
			array->Add(x);
		}
	}
	else //y��
	{
		//�� �������� y���� ���� array�� �߰�
		for(int i=0;i<m_sArrayPoint.GetSize();i++)
		{
			int y = m_sArrayPoint.GetAt(i).y;
			array->Add(y);
		}
	}
}
//--------------------------------------------------------------------------
//���׷�ȭ �� ���
//--------------------------------------------------------------------------
int GPolyline::FindFixedPoint(int nCoord, BOOL bIsX)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //��ǥ �������� ũ�⸦ ����
	if(nsArraySize != 0)
	{
		for(int i=0;i<nsArraySize;i++)
		{
			CPoint tempPt = m_sArrayPoint.GetAt(i);
			if(bIsX == TRUE)
				if(tempPt.x == nCoord)
					return i;
			else
				if(tempPt.y == nCoord)
					return i;
		}
	}
	return -1;
}
//--------------------------------------------------------------------------
//������ �׸� �� �� �߰�
//--------------------------------------------------------------------------
void GPolyline::AddOnePt(CPoint pt)
{
	m_sArrayPoint.Add(pt);
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//�������� ���� �Լ�
//--------------------------------------------------------------------------
void GPolyline::SetPolyStartPoint(CPoint pt)
{
	m_sArrayPoint.SetAt(0, pt); //��ǥ �������� ������ ����
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//�������� ���� �Լ�
//--------------------------------------------------------------------------
void GPolyline::SetPolyEndPoint(CPoint pt)
{
	m_sArrayPoint.SetAt(m_sArrayPoint.GetSize()-1, pt); //��ǥ �������� ������ ����
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//���̵� �Լ�(��� ��ǥ)
//--------------------------------------------------------------------------
void GPolyline::Move(int dX, int dY)
{
	int nArraySize = m_sArrayPoint.GetSize(); //��ǥ �������� ũ�⸦ ����
	if(nArraySize > 0) //��ǥ �������� ������� ���� ���
	{
		for(int i=0;i<nArraySize;i++) //��� ��ǥ�� dX, dY��ŭ �̵�
		{
			CPoint &pt = m_sArrayPoint.GetAt(i); 
			pt.x += dX;
			pt.y += dY;
			m_sArrayPoint.SetAt(i, pt); //dx, dy��ŭ �̵��� ��ǥ�� �ٽ� �迭�� ����
		}
	}
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//���̵� �Լ�(���õ� �ϳ��� ��ǥ)
//--------------------------------------------------------------------------
void GPolyline::Resize(int index, int dX, int dY)
{
	if(m_bsGrouped == FALSE) //�׷�ȭ�Ǿ����� �������
	{
		CPoint &tempPt = m_sArrayPoint.GetAt(index); //�ش� �ε����� ��ǥ�� �޾ƿ�
		//�ش� ��ǥ�� dx,dy��ŭ �̵�
		tempPt.x += dX;
		tempPt.y += dY;
		m_sArrayPoint.SetAt(index, tempPt); //dx,dy��ŭ �̵��� ��ǥ�� �ٽ� �迭�� ����
		FindStartEndPt();
	}
}
//--------------------------------------------------------------------------
//������(���õ� �ϳ��� ��ǥ)
//--------------------------------------------------------------------------
BOOL GPolyline::DelOnePt(int index)
{
	if(m_sArrayPoint.GetSize() > 2) //���� 2�� �Ѱ� ���� ��� �� ���� ����
	{
		m_sArrayPoint.RemoveAt(index); //�ش� �ε����� ��ǥ ����]
		FindStartEndPt();
		return TRUE;
	}
	else
	{
		delete this; //���� 2���ۿ� ���� �ʾ����� �ش� ��ü ����
		return FALSE;
	}
}

//--------------------------------------------------------------------------
//���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GPolyline::SetPenStyle(int penStyle)
{
	m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//���� ���� ��� ���� �Լ�
//--------------------------------------------------------------------------
void GPolyline::SetStartCap(int startCap)
{
	m_nsStartCap = startCap;
}

//--------------------------------------------------------------------------
//���� �� ��� ���� �Լ�
//--------------------------------------------------------------------------
void GPolyline::SetEndCap(int endCap)
{
	m_nsEndCap = endCap;
}

//--------------------------------------------------------------------------
//���׸��� �Լ�
//--------------------------------------------------------------------------
void GPolyline::Draw(CDC *pDC)
{
	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality); //��Ƽ�ٸ����

	GraphicsPath p;

	//�� �÷� �� ���İ� ����
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	//������ ���۰� ���κ� ĸ����
	pen.SetStartCap((LineCap)m_nsStartCap);
	pen.SetEndCap((LineCap)m_nsEndCap);

	//�� ��Ÿ�� ����
	GObject::SetDashStyle(pen, m_nsPenStyle);

	//CPoint���� �� �������� Point���� �� �������� ��ȯ
	int nArraySize = m_sArrayPoint.GetSize();
	Point *pts = new Point[nArraySize];
	for(int i=0;i<nArraySize;i++)
		pts[i] = GObject::CPointToPoint(m_sArrayPoint.GetAt(i));

	graphics.DrawLines(&pen, pts, nArraySize);

	//////////////////////////////////////////////////////////////////////////
	// ������ ����ϴ� �׸���
//	CPen pen(m_nsPenStyle, m_nsThickness, m_sLineColor); //�� �׸���
//	CPen* oldPen = pDC->SelectObject(&pen); //�� ����
//	
//	pDC->Polyline(m_sArrayPoint.GetData(), m_sArrayPoint.GetSize()); //polyline �׸���
//
//	pDC->SelectObject(&oldPen);				//���� ������ �ǵ���
}

//--------------------------------------------------------------------------
//������ ���� ���� �׸��� �Լ�
//--------------------------------------------------------------------------
void GPolyline::DrawSelectLine(CDC *pDC)
{	
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	//�������� ���� ����� �׵θ��� �׸�
	this->GetRegion();
	for(int i=0;i<m_sArrayRgnPoint.GetSize();i+=4 /* �� 4���� ����ϹǷ� */) //���� �ϳ����� ���� �׵θ��� �׸�
	{
		CPoint sPt[4]; //�����ϳ� �� �׵θ��� �׸����� 4���� ���� �ʿ�
		for(int j=0;j<4;j++)
			sPt[j] = m_sArrayRgnPoint.GetAt(i+j); //�� �ϳ��� sPt�� ����
		pDC->Polygon(sPt, 4); //�׵θ� �׸���
	}

	//�׸��� �Ӽ� ����
	CBrush brush(RGB(51,94,168)); //�귯�� ����
	CBrush* oldBrush = pDC->SelectObject(&brush); //�귯�� ����
	pDC->SelectStockObject(NULL_PEN);

	//�� ���������� ���� �׸�
	for(int i=0;i<m_sArrayPoint.GetSize();i++) //�� �������� �����ŭ �ݺ�
	{
		CPoint sPt = m_sArrayPoint.GetAt(i);
		pDC->Ellipse(sPt.x-5, sPt.y-5, sPt.x+5, sPt.y+5); //�� ������ �׸���
	}

	//���� ��� �귯�� �Ӽ����� �ǵ���
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

//--------------------------------------------------------------------------
//���׸��� ���߿� ������ �Ӽ����� Ŀ���� ���� ������ �׸�
//--------------------------------------------------------------------------
void GPolyline::DrawingLine(CDC* pDC, CPoint point)
{
	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality); //��Ƽ�ٸ����

	//�� �÷� �� ���İ� ����
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	//�� ��Ÿ�� ����
	GObject::SetDashStyle(pen, m_nsPenStyle);

	//������ ���۰� ���κ� ĸ����
	pen.SetStartCap((LineCap)m_nsStartCap);
	pen.SetEndCap((LineCap)m_nsEndCap);

	Point endPoint = GObject::CPointToPoint(m_sArrayPoint.GetAt(m_sArrayPoint.GetSize()-1));
	Point currPoint = GObject::CPointToPoint(point);

	graphics.DrawLine(&pen, endPoint, currPoint);
}


//--------------------------------------------------------------------------
//�����õ� ���� ���� �׸�
//--------------------------------------------------------------------------
void GPolyline::DrawSelectedPoint(CDC* pDC, int index)
{
	//�׸��� �Ӽ� ����
	CBrush brush(RGB(255,0,0)); //�귯�� ����
	CBrush* oldBrush = pDC->SelectObject(&brush); //�귯�� ����

	CPoint pt = m_sArrayPoint[index];
	pDC->Ellipse(pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	//���� ������ �ǵ���
	pDC->SelectObject(oldBrush);
}


//--------------------------------------------------------------------------
//���׷� ������¡ �� ����Ʈ�� ����
//--------------------------------------------------------------------------
void GPolyline::SavePolylinePoints()
{
	int nArraySize = m_sArrayPoint.GetSize();
	if(nArraySize > 0) //��ǥ �������� ������� ���� ���
	{
		for(int i=0;i<nArraySize;i++)
		{
			CPoint pt = m_sArrayPoint[i];
			if(m_sPrevArrayPoint.GetSize() < nArraySize)
				m_sPrevArrayPoint.Add(pt);
			else
				m_sPrevArrayPoint.SetAt(i, pt);
		}
	}
}

//--------------------------------------------------------------------------
//���׷� ������¡ �� ����Ʈ�� ������
//--------------------------------------------------------------------------
void GPolyline::ApplyPolylinePoints(double px, double py, int sx, int sy)
{
	int nArraySize = m_sArrayPoint.GetSize(); //��ǥ �������� ũ�⸦ ����
	if(nArraySize > 0) //��ǥ �������� ������� ���� ���
	{
		for(int i=0;i<nArraySize;i++) //��� ��ǥ ����
		{
			CPoint &prevPt = m_sPrevArrayPoint.GetAt(i);
			CPoint &pt = m_sArrayPoint.GetAt(i); 
			pt.x = (int)(sx + (prevPt.x-sx) * px);
			pt.y = (int)(sy + (prevPt.y-sy) * py);
			m_sArrayPoint.SetAt(i, pt); //������ ��ǥ�� �ٽ� �迭�� ����
		}
	}
	FindStartEndPt();
}