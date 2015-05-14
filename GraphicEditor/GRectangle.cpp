/* [ GRectangle Ŭ���� by ������ ]
** GRectangle Ŭ������ �����η�, �簢���� ���� ������ �Ӽ��� ���� Ŭ�����Դϴ�.
*/

#include "StdAfx.h"
#include "GRectangle.h"

enum POSITION_LIST {TOPLEFT, HALFTOPLEFT, BOTTOMLEFT, HALFBOTTOMLEFT, BOTTOMRIGHT,
	HALFBOTTOMRIGHT, TOPRIGHT, HALFTOPRIGHT};

IMPLEMENT_SERIAL(GRectangle, GObject, 0)
//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������
GRectangle::GRectangle(void){
	GObject::m_sType = RECTANGLE;
}
//���� ������
GRectangle::GRectangle(const GRectangle& pGRectangle)
{
	this->GObject::m_sType = pGRectangle.GObject::m_sType;
	this->GObject::m_sStartPoint = pGRectangle.GObject::m_sStartPoint;
	this->GObject::m_sEndPoint = pGRectangle.GObject::m_sEndPoint;
	this->GObject::m_sLineColor = pGRectangle.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pGRectangle.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pGRectangle.GObject::m_bsGrouped;

	this->m_npPenStyle = pGRectangle.m_npPenStyle;
	this->m_pBrushColor = pGRectangle.m_pBrushColor;
}

// �Ҹ���
GRectangle::~GRectangle(void){
}

//--------------------------------------------------------------------------
//������ȭ by ����ö
//--------------------------------------------------------------------------
void GRectangle::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //����
	{
		ar << (WORD)m_npPenStyle;
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
		ar >> wTemp; m_npPenStyle = wTemp;
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
void GRectangle::Move(int dX, int dY){

	GObject::m_sStartPoint.Offset(dX, dY);
	GObject::m_sEndPoint.Offset(dX, dY);
}

//--------------------------------------------------------------------------
//������ ���� �Լ�
//--------------------------------------------------------------------------
CRgn* GRectangle::GetRegion() {

	// �簢�� ������� ������ ����
	CRect rect(GObject::m_sStartPoint, this->GObject::m_sEndPoint);

	// �� �β��� �ʹ� ������ ������ ����Ƿ� �⺻ ũ��� 5
	int dx = (int)(m_nsThickness > 4 ? m_nsThickness : 5);
	int dy = (int)(m_nsThickness > 4 ? m_nsThickness : 5);

	// ���� ��������
	GObject::m_sRgn.DeleteObject();

	// ���ο� ��������
	GObject::m_sRgn.CreateRectRgn(rect.TopLeft().x - m_nsThickness/2-1, rect.TopLeft().y - m_nsThickness/2-1, 
		rect.BottomRight().x + m_nsThickness/2+1, rect.BottomRight().y + m_nsThickness/2+1);

	return &m_sRgn;
}

//--------------------------------------------------------------------------
//������ ũ�� �缳�� �Լ�
//  : ���ڷ� �Ѿ�� ����Ʈ�� �������� ��� ��ġ���� �ľ�
//    8�������� �Ǿ��ִ� ������ �簢���� �����ϸ� �ȴ�
//	  *	  *	  *
//	  *	  *   *
//	  *	  *	  *
//	  �ݽð� �������� TOPLEFT -> HALFTOPLEFT -> BOTTOMLEFT
//	  HALFBOTTOMLEFT -> BOTTOMRIGHT -> HALFBOTTOMRIGHT
//	  TOPRIGHT -> HALFTOPRIGHT
//--------------------------------------------------------------------------
void GRectangle::Resize(int index, int dX, int dY) {

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
//--------------------------------------------------------------------------
//������Ʈ ��ġ ã�� �Լ�
//--------------------------------------------------------------------------
int GRectangle::FindSelectPoint(CPoint pt) {

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
	rgn[2].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.TopLeft().y + rect.Height() - tempThick, 
		rect.TopLeft().x + tempThick, rect.TopLeft().y + rect.Height() + tempThick);

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

	// ��Ȯ�� ����Ʈ�� ã��
	for(i = 0; i < 8; i++) {
		if(rgn[i].PtInRegion(pt) == TRUE) {
			pos = i;
			i = 8;
		}
	}
	return pos;
}

//--------------------------------------------------------------------------
//���׸��� �Լ�
//--------------------------------------------------------------------------
void GRectangle::Draw(CDC* cdc) {
	
	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// �� �÷� �� ���İ� ����
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Color foreColor = GObject::COLORREFtoColor(this->m_pBrushColor);
	Color gradientColor = GObject::COLORREFtoColor(this->m_sGrdEndColor);
	Pen pen(penColor, REAL(m_nsThickness));

	pen.SetLineJoin((LineJoin)this->m_nsLineJoin);
	GObject::SetDashStyle(pen, this->m_npPenStyle);

	INT originX, originY;

	// Rect ������ ���� ��ǥ����
	originX = (GObject::m_sStartPoint.x < GObject::m_sEndPoint.x) ? GObject::m_sStartPoint.x : GObject::m_sEndPoint.x;
	originY = (GObject::m_sStartPoint.y < GObject::m_sEndPoint.y) ? GObject::m_sStartPoint.y : GObject::m_sEndPoint.y;

	// Rect�� ������ ��
	// UpperLeft�� X, Y�� Width�� Height�� ������ ����
	// ���� abs ���밪 �Լ��� �̿�
	Rect tempRect(originX, originY,
		abs(GObject::m_sEndPoint.x - GObject::m_sStartPoint.x), abs(GObject::m_sEndPoint.y - GObject::m_sStartPoint.y));

	//�귯�� ����
	SolidBrush pSolidBrush(foreColor);	//�ܻ� �귯��
	HatchBrush pHatchBrush((HatchStyle)m_nsHatchStyle, foreColor, Color::Transparent);	//���ڹ��� �귯��
	LinearGradientBrush sGradientBrush(tempRect,	foreColor, gradientColor, (LinearGradientMode)m_npGrdMode);	//�׶���Ʈ �귯��

	//���� ä��
	switch(m_esBrushStyle)	//�귯�� ��Ÿ�Ͽ� ���� �б�
	{
	case SOLID:	//�ܻ� ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillRectangle(&pSolidBrush, tempRect);
		break;
	case HATCH: //���ڹ��� ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillRectangle(&pHatchBrush, tempRect);
		break;
	case GRADIENT: //�׶���Ʈ ��Ÿ��
		if(m_esFaceType != OUTLINE) //�ܰ����� �׸��Ⱑ �ƴ� �� ���� ä��
			graphics.FillRectangle(&sGradientBrush, tempRect);
		break;
	}

	//�ܰ����� �׸�
	if(m_esFaceType != FILLONLY)
		graphics.DrawRectangle(&pen, tempRect);


// 	if(this->m_npBrushStyle == BS_SOLID) {
// 		SolidBrush pSolidBrush(foreColor);
// 				graphics.FillRectangle(&pSolidBrush, tempRect);
// 		graphics.DrawRectangle(&pen, tempRect);
// 	}
// 	else {
// 		//HatchBrush pHatchBrush((HatchStyle)this->m_nsHatchStyle, foreColor, Color::Transparent);
// 		HatchBrush pHatchBrush((HatchStyle)HatchStyleHorizontal, foreColor, Color::Transparent);
// 		graphics.DrawRectangle(&pen, tempRect);
// 		graphics.FillRectangle(&pHatchBrush, tempRect);
// 	}

/*
	// ������ �� ��, �β�, ��Ÿ���� �̿��� �� ����
	CPen pen(this->m_npPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);
	CPen *oldPen = cdc->SelectObject(&pen);					// �� ����
	CBrush brush;
	
	if(m_npBrushStyle == BS_SOLID) {
		brush.CreateSolidBrush(m_pBrushColor);
	}
	else {
		brush.CreateHatchBrush(m_lpHatchStyle, m_pBrushColor);
	}

	CBrush *oldBrush = cdc->SelectObject(&brush);
	
	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint);
	
	cdc->Rectangle(rect);
	cdc->SelectObject(&oldPen);					// ���� ������ �ǵ���
	cdc->SelectObject(&oldBrush);				// ���� �귯���� �ǵ���*/
}
//--------------------------------------------------------------------------
//���� Ÿ�� �����Լ�
//--------------------------------------------------------------------------
void GRectangle::SetFaceType(int faceType)
{
	m_esFaceType = (GFaceType)faceType;
}


//--------------------------------------------------------------------------
//���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetPenStyle(int penStyle)
{
	this->m_npPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//���귯�� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetBrushStyle(int brushStyle)
{
	this->m_esBrushStyle = (GBrushType)brushStyle;
}

//--------------------------------------------------------------------------
//���귯�� �� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetBrushColor(COLORREF brushColor)
{
	this->m_pBrushColor = brushColor;
}

//--------------------------------------------------------------------------
//����ġ ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetHatchStyle(int hatch)
{
	this->m_nsHatchStyle = hatch;
}

//--------------------------------------------------------------------------
//������ ���� ��Ÿ�� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetLineJoinStyle(int lineJoin)
{
	this->m_nsLineJoin = lineJoin;
}

//--------------------------------------------------------------------------
//���׶���Ʈ ��� ���� �Լ�
//  : Horizontal, Vertical, ForwardDiagonal, BackwardDiagonal
//--------------------------------------------------------------------------
void GRectangle::SetGradientMode(int grdMode)
{
	this->m_npGrdMode = grdMode;
}

//--------------------------------------------------------------------------
//���׶��̼� �� �� ���� �Լ�
//--------------------------------------------------------------------------
void GRectangle::SetGrdEndColor(COLORREF grdEndColor)
{
	this->m_sGrdEndColor = grdEndColor;
}

//--------------------------------------------------------------------------
//������ ���� �׸��� �Լ� by ����ö
//--------------------------------------------------------------------------
void GRectangle::DrawSelectLine(CDC *pDC)
{
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //�������� �簢���� ����
	rect.bottom += 1;
	rect.right +=1;
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