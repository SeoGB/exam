/* [ GLine Ŭ���� by ������ ]
** GLine Ŭ������ �������Դϴ�.
*/

#include "StdAfx.h"
#include "GLine.h"

IMPLEMENT_SERIAL(GLine, GObject, 0)
//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������
GLine::GLine() {
	GObject::m_sType = LINE;
}

//���� ������
GLine::GLine(const GLine& pGLine)
{
	this->GObject::m_sType = pGLine.GObject::m_sType;
	this->GObject::m_nsThickness = pGLine.GObject::m_nsThickness;
	this->GObject::m_sLineColor = pGLine.GObject::m_sLineColor;
	this->GObject::m_sStartPoint = pGLine.GObject::m_sStartPoint;
	this->GObject::m_sEndPoint = pGLine.GObject::m_sEndPoint;
	this->GObject::m_bsGrouped = pGLine.GObject::m_bsGrouped;

	this->m_nsPenStyle = pGLine.m_nsPenStyle;
	for(int i=0;i<4;i++)
		this->m_pRgnPoint[i] = pGLine.m_pRgnPoint[i];
}

//�Ҹ���
GLine::~GLine() {}
//--------------------------------------------------------------------------
//������ȭ by ����ö
//--------------------------------------------------------------------------
void GLine::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_nsPenStyle;
		ar << m_nsStartCap;
		ar << m_nsEndCap;
	}
	else
	{
		ar >> m_nsPenStyle;
		ar >> m_nsStartCap;
		ar >> m_nsEndCap;
	}
}

//--------------------------------------------------------------------------
//���� ��ü�� ���� ������ ���� by ����ö
//--------------------------------------------------------------------------
CRgn* GLine::GetRegion() {

	//���� ������� ������ ����
	int x = m_sStartPoint.x;
	int y = m_sStartPoint.y;
	int w = GObject::m_sEndPoint.x - m_sStartPoint.x; //��
	int h = GObject::m_sEndPoint.y - m_sStartPoint.y; //����

	//�� �� ������ ���⸦ ����
	double theta = w ? atan2((double)w, (double)h) : (~h+1) * M_PI / 2;
	if(theta < 0)
		theta = theta + 2*M_PI;

	//����� ������ ����
	double angle = theta + M_PI/2;

	//�� �β��� �ʹ� ������ ������ ����Ƿ� �⺻ ũ��� 5
	int dx = (int)((m_nsThickness > 4 ? m_nsThickness+1 : 5) * sin(angle));
	int dy = (int)((m_nsThickness > 4 ? m_nsThickness+1 : 5) * cos(angle));

	//������ ����� ���� 4���� ������ ����
	CPoint pt [4];
	pt[0] = CPoint(x+dx, y+dy); //���� �ϴ�
	pt[1] = CPoint(x-dx, y-dy); //���� ���
	pt[2] = CPoint(x+w-dx, y+h-dy); //���� ���
	pt[3] = CPoint(x+w+dx, y+h+dy); //���� �ϴ�

	// �� ������ �������� �迭�� ����
	for(int i = 0; i < 4; i++) {
		this->m_pRgnPoint[i] = pt[i];
	}

	//���� ����
	GObject::m_sRgn.DeleteObject();
	GObject::m_sRgn.CreatePolygonRgn(pt, 4, WINDING);
	//���� ��ȯ
	return &m_sRgn;
}

//--------------------------------------------------------------------------
//������ ���� �� 4���� ��ǥ�� CPoint �迭�� ����
//--------------------------------------------------------------------------
CPoint* GLine::GetRgnPoint(void) {
	return this->m_pRgnPoint;
}

//--------------------------------------------------------------------------
//���̵� �Լ�(��� ��ǥ)
//--------------------------------------------------------------------------
void GLine::Move(int dX, int dY) {

	GObject::m_sStartPoint.Offset(dX, dY);	// �������� dX, dY��ŭ �̵�
	this->GObject::m_sEndPoint.Offset(dX, dY);		// �������� dX, dY��ŭ �̵�
}

//--------------------------------------------------------------------------
//���̵� �Լ�(���õ� �ϳ��� ��ǥ)
//--------------------------------------------------------------------------
void GLine::Resize(int index, int dX, int dY) {

	switch(index) {

	case 0 :
		GObject::m_sStartPoint.Offset(dX, dY);
		break;
	case 1 :
		GObject::m_sStartPoint.Offset(dX, 0);
		break;
	case 2 :
		GObject::m_sStartPoint.x += dX;
		GObject::m_sEndPoint.y += dY;
		break;
	case 3 :
		GObject::m_sEndPoint.Offset(0, dY);
		break;
	case 4 :
		GObject::m_sEndPoint.Offset(dX, dY);
		break;
	case 5 :
		GObject::m_sEndPoint.Offset(dX, 0);
		break;
	case 6 :
		GObject::m_sEndPoint.x += dX;
		GObject::m_sStartPoint.y += dY;
		break;
	case 7 :
		GObject::m_sStartPoint.Offset(0, dY);
		break;
	}
}



//--------------------------------------------------------------------------
//������ ������ ��ǥ ����Ʈ ��ȯ
//--------------------------------------------------------------------------
int GLine::FindSelectPoint(CPoint pt) {
	
	int tempThick = (GObject::m_nsThickness > 4 ? GObject::m_nsThickness : 5); // �⺻ ũ��� 5 
	
	CRgn rgn;
	CPoint tempPt = GObject::m_sStartPoint;
	rgn.CreateEllipticRgn(tempPt.x - tempThick, tempPt.y - tempThick, tempPt.x + tempThick, tempPt.y + tempThick); // ���� ����

	// �������� Rgn �ȿ� ������ �������� ����
	if(rgn.PtInRegion(pt) == TRUE) {
		return 0;
	}

	tempPt = GObject::m_sEndPoint;
	rgn.DeleteObject();
	rgn.CreateEllipticRgn(tempPt.x - tempThick, tempPt.y - tempThick, tempPt.x + tempThick, tempPt.y + tempThick); // ���� ����

	// ������ ���õ� ���
	if(rgn.PtInRegion(pt) == TRUE) {
		return 4;
	}
	return -1;
}

//--------------------------------------------------------------------------
//���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GLine::SetPenStyle(int penStyle)
{
	this->m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//���� ���� ��� ���� �Լ�
//--------------------------------------------------------------------------
void GLine::SetStartCap(int startCap)
{
	m_nsStartCap = startCap;
}

//--------------------------------------------------------------------------
//���� �� ��� ���� �Լ�
//--------------------------------------------------------------------------
void GLine::SetEndCap(int endCap)
{
	m_nsEndCap = endCap;
}

//--------------------------------------------------------------------------
// �׸��� �Լ�
//--------------------------------------------------------------------------
void GLine::Draw(CDC* pDC) {

	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// �� �÷� �� ���İ� ����
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	// ������ ���۰� ���κ� �������
	pen.SetStartCap((LineCap)this->m_nsStartCap);
	pen.SetEndCap((LineCap)this->m_nsEndCap);

	// �� ��Ÿ�� ����
	GObject::SetDashStyle(pen, this->m_nsPenStyle);

	graphics.DrawLine(&pen, Point(GObject::m_sStartPoint.x, GObject::m_sStartPoint.y), 
		Point(GObject::m_sEndPoint.x, GObject::m_sEndPoint.y));

// 	CPen pen(m_nsPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);	// �� ����
// 	CPen *oldPen = pDC->SelectObject(&pen);	// �� ����
// 
// 	pDC->MoveTo(GObject::m_sStartPoint);// ���׸���
// 	pDC->LineTo(GObject::m_sEndPoint);	// ���׸���
// 
// 	pDC->SelectObject(&oldPen); // ���������� �ǵ���
}

//--------------------------------------------------------------------------
// ���� ���� ���� �׸��� �Լ� by ����ö
//--------------------------------------------------------------------------
void GLine::DrawSelectLine(CDC *pDC)
{
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	//�������� ���� ����� �׵θ��� �׸�
	this->GetRegion();
	pDC->Polygon(this->GetRgnPoint(), 4);

	//�׸��� �Ӽ� ����
	CBrush brush(RGB(51,94,168)); //�귯�� ����
	CBrush* oldBrush = pDC->SelectObject(&brush); //�귯�� ����
	pDC->SelectStockObject(NULL_PEN);

	//�� �������� ������ ���� �׸�
	pDC->Ellipse(GObject::m_sStartPoint.x-5, GObject::m_sStartPoint.y-5, GObject::m_sStartPoint.x+5, GObject::m_sStartPoint.y+5); //������ �׸���
	pDC->Ellipse(GObject::m_sEndPoint.x-5, GObject::m_sEndPoint.y-5, GObject::m_sEndPoint.x+5, GObject::m_sEndPoint.y+5); //���� �׸���
	pDC->SelectObject(&oldBrush);

	//���� ��� �귯�� �Ӽ����� �ǵ���
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}
