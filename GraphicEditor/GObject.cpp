/* [ GObject Ŭ���� by ����ö ]
** GObject Ŭ������ �������Դϴ�.
*/

#include "StdAfx.h"
#include "GObject.h"

IMPLEMENT_SERIAL(GObject, CObject, 0)

//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������
GObject::GObject(void)
{
	m_nsThickness = 1;
	m_sLineColor = RGB(0,0,0);
	m_bsGrouped = FALSE;
	m_nsAlpha = 100;
}
//�Ҹ���
GObject::~GObject(void)
{}
//--------------------------------------------------------------------------
//������ȭ
//--------------------------------------------------------------------------
void GObject::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring()) //����
	{
		ar << (WORD)m_sType;
		ar << m_sStartPoint;
		ar << m_sEndPoint;
		ar << m_sLineColor;
		ar << m_nsThickness;
		ar << (WORD)m_bsGrouped;
		ar << m_nsAlpha;
	}
	else //�ε�
	{
		WORD wTemp;
		ar >> wTemp; m_sType = (GraphicalObjectType)wTemp;
		ar >> m_sStartPoint;
		ar >> m_sEndPoint;
		ar >> m_sLineColor;
		ar >> m_nsThickness;
		ar >> wTemp; m_bsGrouped = (BOOL)wTemp;
		ar >> m_nsAlpha;
	}
}
//--------------------------------------------------------------------------
//��Ŭ�� ������ �����ȿ� �ִ��� ���� ��ȯ
//--------------------------------------------------------------------------
BOOL GObject::PtInGObj(CPoint pt)
{
	CRgn *rgn = GetRegion(); //�׷����� ��ü ������� ���� ����
	return rgn->PtInRegion(pt); //Ŭ�� ������ ���� �ȿ� �ִ��� ��ȯ
}

//--------------------------------------------------------------------------
//��COLORREF���� GDI+���� ����ϴ� Color������ ��ȯ
//--------------------------------------------------------------------------
Color GObject::COLORREFtoColor(COLORREF color)
{
	//�������� ����
	int nAlpha = (int)((m_nsAlpha / 100.0) * 255);

	//R,G,B���� ����
	int nR = GetRValue(color);
	int nG = GetGValue(color);
	int nB = GetBValue(color);

	//�÷� ��ȯ
	return Color(nAlpha, nR, nG, nB);
}
//--------------------------------------------------------------------------
//��CPoint���� GDI+���� ����ϴ� Point������ ��ȯ
//--------------------------------------------------------------------------
Point GObject::CPointToPoint(CPoint point)
{
	return Point(point.x, point.y);
}

//--------------------------------------------------------------------------
//�������� ����
//--------------------------------------------------------------------------
void GObject::SetStartPoint(CPoint pt)
{
	m_sStartPoint = pt;
}

//--------------------------------------------------------------------------
//�������� ����
//--------------------------------------------------------------------------
void GObject::SetEndPoint(CPoint endPoint)
{
	m_sEndPoint = endPoint;
}

//--------------------------------------------------------------------------
//�������� �� ������ ����
//--------------------------------------------------------------------------
void GObject::SetBothPoint(CPoint startPt, CPoint endPt)
{
	m_sStartPoint = startPt;
	m_sEndPoint = endPt;
}

//--------------------------------------------------------------------------
//������ �÷� ����
//--------------------------------------------------------------------------
void GObject::SetLineColor(COLORREF color)
{
	m_sLineColor = color;
}

//--------------------------------------------------------------------------
//������ ���� ����
//--------------------------------------------------------------------------
void GObject::SetThickness(int thickness)
{
	m_nsThickness = thickness;
}
//--------------------------------------------------------------------------
//���׷� ���� ����
//--------------------------------------------------------------------------
void GObject::SetGrouped(BOOL isGrouped) 
{ 
	m_bsGrouped = isGrouped;
}

//--------------------------------------------------------------------------
//���� ����
//--------------------------------------------------------------------------
void GObject::SetDashStyle(Pen& pen, int nPenStyle)
{
	//���Ⱑ 1�� ��쿡�� ��ȭ�� �̼��Ͽ� Ŀ���� ��Ÿ�� ����
	if(m_nsThickness == 1 && nPenStyle > 0)
	{
		int nCount;
		REAL* arrayDash;
		switch(nPenStyle)
		{
		case 1: //DashStyleDash
			nCount = 2;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 18.0f; //dash
			arrayDash[1] = 6.0f; //����
			
			break;
		case 2: //DashStyleDot
			nCount = 2;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 3.0f; //dot
			arrayDash[1] = 3.0f; //����
			pen.SetDashPattern(arrayDash,3);
			break;
		case 3: //DashStyleDashDot
			nCount = 4;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 9.0f; //dash
			arrayDash[2] = 3.0f; //dot
			arrayDash[1] = arrayDash[3] = 6.0f; //����
			pen.SetDashPattern(arrayDash,4);
			break;
		case 4: //DashStyleDashDotDot
			nCount = 6;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 9.0f; //dash
			arrayDash[2] = arrayDash[4] = 3.0f; //dot
			arrayDash[1] = arrayDash[3] = arrayDash[5] = 3.0f; //����
			pen.SetDashPattern(arrayDash,6);
			break;
		}
		pen.SetDashStyle(DashStyleCustom); //Ŀ���� ��Ÿ��
		pen.SetDashPattern(arrayDash,nCount);
	}
	//���Ⱑ 2 �̻��̰ų� DashStyleSolid�� ��� dash style �״�� ����
	else
		pen.SetDashStyle((DashStyle)nPenStyle);
}

//--------------------------------------------------------------------------
//�������� ����
//--------------------------------------------------------------------------
void GObject::SetAlpha(int alpha)
{
	m_nsAlpha = alpha;
}

//--------------------------------------------------------------------------
//���׷�ȭ ������¡�� ����(by ������)
//--------------------------------------------------------------------------
void GObject::SavePoints()
{
	this->m_pResizeStart = m_sStartPoint;
	this->m_pResizeEnd = m_sEndPoint;
}

void GObject::ApplyPoints(double px, double py, int sx, int sy)
{
	m_sStartPoint.x = (int)(sx + (this->m_pResizeStart.x - sx) * px);
	m_sEndPoint.x = (int)(sx + (this->m_pResizeEnd.x - sx) * px);

	m_sStartPoint.y = (int)(sy + (this->m_pResizeStart.y - sy) * py);
	m_sEndPoint.y = (int)(sy + (this->m_pResizeEnd.y - sy) * py);
}
