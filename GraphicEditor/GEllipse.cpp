/* [ GEllipse Ŭ���� by ������ ]
** GEllipse Ŭ������ ���� ���� ������ �Ӽ��� ���� Ŭ�����Դϴ�.
*/

#include "StdAfx.h"
#include "GEllipse.h"

enum POSITION_LIST {TOPLEFT, HALFTOPLEFT, BOTTOMLEFT, HALFBOTTOMLEFT, BOTTOMRIGHT,
	  HALFBOTTOMRIGHT, TOPRIGHT, HALFTOPRIGHT};

IMPLEMENT_SERIAL(GEllipse, GObject, 0)

//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������



//hhh
GEllipse::GEllipse(void) {
	GObject::m_sType = ELLIPSE;
}

//���� ������
GEllipse::GEllipse(const GEllipse& pGEllipse)
{
	this->GObject::m_sType = pGEllipse.GObject::m_sType;
	this->GObject::m_sStartPoint = pGEllipse.GObject::m_sStartPoint;
	this->GObject::m_sEndPoint = pGEllipse.GObject::m_sEndPoint;
	this->GObject::m_sLineColor = pGEllipse.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pGEllipse.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pGEllipse.GObject::m_bsGrouped;

	this->m_nsPenStyle = pGEllipse.m_nsPenStyle;
	this->m_pBrushColor = pGEllipse.m_pBrushColor;
}

// �Ҹ���
GEllipse::~GEllipse(void) {
}

//--------------------------------------------------------------------------
//������ȭ by ����ö
//--------------------------------------------------------------------------
void GEllipse::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //����
	{
		ar << (WORD)m_nsPenStyle;
		ar << m_pBrushColor;
		ar << (WORD)m_esBrushStyle;
		ar << (WORD)m_esFaceType;
		ar << (WORD)m_npGrdMode;
		ar << (WORD)m_nsHatchStyle;
		ar << (WORD)m_nsLineJoin;
		ar << this->m_sGrdEndColor;
	}
	else //�ε�
	{
		WORD wTemp;
		ar >> wTemp; m_nsPenStyle = wTemp;
		ar >> m_pBrushColor;
		ar >> wTemp; m_esBrushStyle = (GBrushType)wTemp;
		ar >> wTemp; m_esFaceType = (GFaceType)wTemp;
		ar >> wTemp; m_npGrdMode = wTemp;
		ar >> wTemp; m_nsHatchStyle = wTemp;
		ar >> wTemp; m_nsLineJoin = wTemp;
		ar >> m_sGrdEndColor;
	}
}

//--------------------------------------------------------------------------
//���� �� �� ���� �Լ�(�̵�)
//--------------------------------------------------------------------------
void GEllipse::Move(int dX, int dY){
	
	GObject::m_sStartPoint.Offset(dX, dY);
	GObject::m_sEndPoint.Offset(dX, dY);
}

//--------------------------------------------------------------------------
//������ ���� �Լ�
//--------------------------------------------------------------------------
CRgn* GEllipse::GetRegion() {

	// �簢�� ������� ������ ����
	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint);

	// �� �β��� �ʹ� ������ ������ ����Ƿ� �⺻ ũ��� 5
	int dx = (int)(m_nsThickness > 4 ? m_nsThickness : 5);
	int dy = (int)(m_nsThickness > 4 ? m_nsThickness : 5);

	m_sRgn.DeleteObject();				// �̹� ������� ������ �ִٸ� ����

	// ������ �籸��
	GObject::m_sRgn.CreateRectRgn(rect.TopLeft().x - m_nsThickness/2-1, rect.TopLeft().y - m_nsThickness/2-1, 
		rect.BottomRight().x + m_nsThickness/2+1, rect.BottomRight().y + m_nsThickness/2+1);

	return &m_sRgn;
}

//-------------------------------------------------------------
//������ ũ�� �缳�� �Լ�
//  : ���ڷ� �Ѿ�� ����Ʈ�� �������� ��� ��ġ���� �ľ�
//    8�������� �Ǿ��ִ� ������ �簢���� �����ϸ� �ȴ�
//	  *	 *	*
//	  *	 *	*
//	  *	 *	*
//	  �ݽð� �������� TOPLEFT -> HALFTOPLEFT -> BOTTOMLEFT
//	  HALFBOTTOMLEFT -> BOTTOMRIGHT -> HALFBOTTOMRIGHT
//	  TOPRIGHT -> HALFTOPRIGHT
//-------------------------------------------------------------
void GEllipse::Resize(int index, int dX, int dY) {

	if(GObject::m_bsGrouped == FALSE) //�׷�ȭ �Ǿ����� ���� ���
	{
		switch(index) {
		case TOPLEFT :
			GObject::m_sStartPoint.Offset(dX, dY);
			break;
		case HALFTOPLEFT :
			GObject::m_sStartPoint.Offset(dX, 0);
			break;
		case BOTTOMLEFT :
			GObject::m_sStartPoint.x += dX;
			GObject::m_sEndPoint.y += dY;
			break;
		case HALFBOTTOMLEFT :
			GObject::m_sEndPoint.Offset(0, dY);
			break;
		case BOTTOMRIGHT :
			GObject::m_sEndPoint.Offset(dX, dY);
			break;
		case HALFBOTTOMRIGHT :
			GObject::m_sEndPoint.Offset(dX, 0);
			break;
		case TOPRIGHT :
			GObject::m_sEndPoint.x += dX;
			GObject::m_sStartPoint.y += dY;
			break;
		case HALFTOPRIGHT :
			GObject::m_sStartPoint.Offset(0, dY);
			break;
		}
	}
	else		// �׷�ȭ �Ǿ��ִ� ���
	{
		switch(index)
		{
		case TOPLEFT:
			GObject::m_sStartPoint.Offset(dX, dY);
			break;
		}
	}
}

//--------------------------------------------------------------------------
//������Ʈ ��ġ ã�� �Լ�
//--------------------------------------------------------------------------
int GEllipse::FindSelectPoint(CPoint pt) {

	int pos = -1;
	int i;
	CRgn rgn[8];
	int tempThick = (GObject::m_nsThickness > 4 ? GObject::m_nsThickness : 5); // �⺻ ũ��� 5  

	// �������� Rect���� TopLeft�� ������ġ
	// TopLeft �������� �ݽð�������� 8������ �� Rgn�� ����

	//	*	*	*
	//	*		*
	//	*	*	*
	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint);

	// TopLeft ����
	rgn[0].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.TopLeft().y - tempThick, rect.TopLeft().x + tempThick, rect.TopLeft().y + tempThick);

	// TopLeft���� Rect ������ �ݸ�ŭ ������ ����
	rgn[1].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.CenterPoint().y - tempThick, 
		rect.TopLeft().x + tempThick, rect.CenterPoint().y + tempThick);

	// TopLeft���� Rect ���̸�ŭ ������ ����
	rgn[2].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.TopLeft().x + tempThick, rect.BottomRight().y + tempThick);

	// BottomRight���� Rect �ʺ��� �ݸ�ŭ �������� �̵��� ����
	rgn[3].CreateEllipticRgn(rect.CenterPoint().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.CenterPoint().x + tempThick, rect.BottomRight().y + tempThick);

	// BottomRight ����
	rgn[4].CreateEllipticRgn(rect.BottomRight().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.BottomRight().x + tempThick, rect.BottomRight().y + tempThick);

	// BottomRight �������� Rect ������ �ݸ�ŭ ���� �ö� ����
	rgn[5].CreateEllipticRgn(rect.BottomRight().x - tempThick, rect.CenterPoint().y- tempThick, 
		rect.BottomRight().x + tempThick, rect.CenterPoint().y + tempThick);

	// BottomRight �������� Rect ���̸�ŭ ���� �ö� ����
	rgn[6].CreateEllipticRgn(rect.right - tempThick, rect.top - tempThick, rect.right + tempThick, rect.top + tempThick);

	// TopLeft �������� Rect �ʺ��� �ݸ�ŭ �������� �̵��� ����
	rgn[7].CreateEllipticRgn(rect.CenterPoint().x - tempThick, rect.TopLeft().y - tempThick, 
		rect.CenterPoint().x + tempThick, rect.TopLeft().y + tempThick);

	// ã�����ϴ� ���� ��ġ�� ã�� ����
	for(i = 0; i < 8; i++) {
		if(rgn[i].PtInRegion(pt) == TRUE) {
			pos = i;
			i = 8;
		}
	}
	return pos;
}
//--------------------------------------------------------------------------
//���� Ÿ�� �����Լ�
//--------------------------------------------------------------------------
void GEllipse::SetFaceType(int faceType)
{
	m_esFaceType = (GFaceType)faceType;
}

//--------------------------------------------------------------------------
//���귯�� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetBrushStyle(int brushStyle)
{
	this->m_esBrushStyle = (GBrushType)brushStyle;
}

//--------------------------------------------------------------------------
//���귯�� �� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetBrushColor(COLORREF brushColor) {
	this->m_pBrushColor = brushColor;
}

//--------------------------------------------------------------------------
//���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetPenStyle(int penStyle)
{
	m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//����ġ ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetHatchStyle(int hatch)
{
	this->m_nsHatchStyle = hatch;
}

//--------------------------------------------------------------------------
//������ ���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetLineJoinStyle(int lineJoin)
{
	this->m_nsLineJoin = lineJoin;
}

//--------------------------------------------------------------------------
//���׶���Ʈ ��� ���� �Լ�
//  : Horizontal, Vertical, ForwardDiagonal, BackwardDiagonal
//--------------------------------------------------------------------------
void GEllipse::SetGradientMode(int grdMode)
{
	this->m_npGrdMode = grdMode;
}

//--------------------------------------------------------------------------
//���׶��̼� �� �� ���� �Լ�
//--------------------------------------------------------------------------
void GEllipse::SetGrdEndColor(COLORREF grdEndColor)
{
	this->m_sGrdEndColor = grdEndColor;
}

//--------------------------------------------------------------------------
//���׸��� �Լ�
//--------------------------------------------------------------------------
void GEllipse::Draw(CDC* cdc) {

	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// �� �÷� �� ���İ� ����
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Color foreColor = GObject::COLORREFtoColor(this->m_pBrushColor);
	Color gradientColor = GObject::COLORREFtoColor(this->m_sGrdEndColor);
	Pen pen(penColor, REAL(m_nsThickness));
	
	pen.SetLineJoin((LineJoin)this->m_nsLineJoin);	//���� ����(�ܰ��� ����) ����
	GObject::SetDashStyle(pen, this->m_nsPenStyle); //���� ���� ����
	Rect tempRect(GObject::m_sStartPoint.x, GObject::m_sStartPoint.y,
		GObject::m_sEndPoint.x - GObject::m_sStartPoint.x, GObject::m_sEndPoint.y - GObject::m_sStartPoint.y);

	//�귯�� ����
	SolidBrush pSolidBrush(foreColor);	//�ܻ� �귯��
	HatchBrush pHatchBrush((HatchStyle)m_nsHatchStyle, foreColor, Color::Transparent);	//���ڹ��� �귯��
	LinearGradientBrush sGradientBrush(tempRect,	foreColor, gradientColor, (LinearGradientMode)m_npGrdMode);	//�׶���Ʈ �귯��
	
	//���� ä��
	switch(m_esBrushStyle)	//�귯�� ��Ÿ�Ͽ� ���� �б�
	{
	case SOLID:	//�ܻ� ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillEllipse(&pSolidBrush, tempRect);
		break;
	case HATCH: //���ڹ��� ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillEllipse(&pHatchBrush, tempRect);
		break;
	case GRADIENT: //�׶���Ʈ ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillEllipse(&sGradientBrush, tempRect);
		break;
	}

	//�ܰ����� �׸�
	if(m_esFaceType != FILLONLY)
		graphics.DrawEllipse(&pen, tempRect);

// 	if(this->m_npBrushStyle == BS_SOLID) {
// 		SolidBrush pSolidBrush(foreColor);
// 		graphics.DrawEllipse(&pen, tempRect);
// 		graphics.FillEllipse(&pSolidBrush, tempRect);
// 	}
// 	else {
// 		//HatchBrush pHatchBrush((HatchStyle)this->m_nsHatchStyle, foreColor, Color::Transparent);
// 		HatchBrush pHatchBrush((HatchStyle)HatchStyleHorizontal, foreColor, Color::Transparent);
// 		graphics.DrawEllipse(&pen, tempRect);
// 		graphics.FillEllipse(&pHatchBrush, tempRect);
// 	}
	
	/*
	CPen pen(m_nsPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);	// �� ����
	CPen *oldPen = cdc->SelectObject(&pen);									// �� ����
	CBrush brush;

	if(m_npBrushStyle == BS_SOLID) {			// BS_SOLID ��Ÿ���ϰ��
		brush.CreateSolidBrush(m_pBrushColor);	// SOLID �귯���� ����
	}
	else {
		brush.CreateHatchBrush(m_lpHatchStyle, m_pBrushColor); // HATCH �귯�� ����
	}

	CBrush *oldBrush = cdc->SelectObject(&brush); // �귯����ü ����

	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint); // Rect ����

	cdc->Ellipse(rect);	//Ÿ���� �׸�
	cdc->SelectObject(&oldPen);					// ���������� �ǵ���
	cdc->SelectObject(&oldBrush);				// ���� �귯���� �ǵ���*/
}

//--------------------------------------------------------------------------
//������ ���� ���� �׸��� �Լ� by ����ö
//--------------------------------------------------------------------------
void GEllipse::DrawSelectLine(CDC *pDC)
{
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));			// dot ��Ÿ���� ���� �� ����
	CPen* oldPen = pDC->SelectObject(&penDotted);		// dot ��Ÿ�� �� ����

	CRect rect;
	GetRegion()->GetRgnBox(rect); //�������� �簢���� ����
	pDC->Rectangle(rect); //�簢�� ������ �׸�

	//�׸��� �Ӽ� ����
	CBrush brush(RGB(51,94,168)); //�귯�� ����
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN); //NULL_PEN���� ����

	//ũ�� ������ ����� ���� ��ġ�� ����
	CPoint sPointArray[8];
	sPointArray[0] = rect.TopLeft();
	sPointArray[1] = CPoint(rect.left, rect.CenterPoint().y);
	sPointArray[2] = CPoint(rect.left, rect.bottom);
	sPointArray[3] = CPoint(rect.CenterPoint().x, rect.bottom);
	sPointArray[4] = CPoint(rect.BottomRight());
	sPointArray[5] = CPoint(rect.right, rect.CenterPoint().y);
	sPointArray[6] = CPoint(rect.right, rect.top);
	sPointArray[7] = CPoint(rect.CenterPoint().x, rect.top);

	//ũ�� ������ ���� �� �׸���
	for(int i=0;i<8;i++)
		pDC->Ellipse(sPointArray[i].x-5, sPointArray[i].y-5, sPointArray[i].x+5, sPointArray[i].y+5);

	//���� ��� �귯�� �Ӽ����� �ǵ���
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}