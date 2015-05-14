/* [ GRectangle 클래스 by 박현덕 ]
** GRectangle 클래스의 구현부로, 사각형에 대한 정보와 속성을 가진 클래스입니다.
*/

#include "StdAfx.h"
#include "GRectangle.h"

enum POSITION_LIST {TOPLEFT, HALFTOPLEFT, BOTTOMLEFT, HALFBOTTOMLEFT, BOTTOMRIGHT,
	HALFBOTTOMRIGHT, TOPRIGHT, HALFTOPRIGHT};

IMPLEMENT_SERIAL(GRectangle, GObject, 0)
//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GRectangle::GRectangle(void){
	GObject::m_sType = RECTANGLE;
}
//복사 생성자
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

// 소멸자
GRectangle::~GRectangle(void){
}

//--------------------------------------------------------------------------
//　직렬화 by 신희철
//--------------------------------------------------------------------------
void GRectangle::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //저장
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
	else //로드
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
//　양 끝 점 설정 함수(이동)
//--------------------------------------------------------------------------
void GRectangle::Move(int dX, int dY){

	GObject::m_sStartPoint.Offset(dX, dY);
	GObject::m_sEndPoint.Offset(dX, dY);
}

//--------------------------------------------------------------------------
//　리전 생성 함수
//--------------------------------------------------------------------------
CRgn* GRectangle::GetRegion() {

	// 사각형 모양으로 리전을 만듬
	CRect rect(GObject::m_sStartPoint, this->GObject::m_sEndPoint);

	// 선 두께가 너무 얇으면 선택이 힘드므로 기본 크기는 5
	int dx = (int)(m_nsThickness > 4 ? m_nsThickness : 5);
	int dy = (int)(m_nsThickness > 4 ? m_nsThickness : 5);

	// 이전 영역삭제
	GObject::m_sRgn.DeleteObject();

	// 새로운 영역생성
	GObject::m_sRgn.CreateRectRgn(rect.TopLeft().x - m_nsThickness/2-1, rect.TopLeft().y - m_nsThickness/2-1, 
		rect.BottomRight().x + m_nsThickness/2+1, rect.BottomRight().y + m_nsThickness/2+1);

	return &m_sRgn;
}

//--------------------------------------------------------------------------
//　도형 크기 재설정 함수
//  : 인자로 넘어온 포인트를 기준으로 어느 위치인지 파악
//    8방향으로 되어있는 임의의 사각형을 생각하면 된다
//	  *	  *	  *
//	  *	  *   *
//	  *	  *	  *
//	  반시계 방향으로 TOPLEFT -> HALFTOPLEFT -> BOTTOMLEFT
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
//　포인트 위치 찾는 함수
//--------------------------------------------------------------------------
int GRectangle::FindSelectPoint(CPoint pt) {

	int pos = -1;
	int i;
	CRgn rgn[8];
	int tempThick = (GObject::m_nsThickness > 4 ? GObject::m_nsThickness : 5); // 기본 크기는 5  

	// 시작점은 Rect에서 TopLeft와 같은위치
	// TopLeft 지점부터 반시계방향으로 8방향의 각 Rgn을 만듬

	//	*	*	*
	//	*		*
	//	*	*	*
	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint);

	// TopLeft 지점
	rgn[0].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.TopLeft().y - tempThick, rect.TopLeft().x + tempThick, rect.TopLeft().y + tempThick);

	// TopLeft에서 Rect 높이의 반만큼 내려온 지점
	rgn[1].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.CenterPoint().y - tempThick, 
		rect.TopLeft().x + tempThick, rect.CenterPoint().y + tempThick);

	// TopLeft에서 Rect 높이만큼 내려온 지점
	rgn[2].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.TopLeft().y + rect.Height() - tempThick, 
		rect.TopLeft().x + tempThick, rect.TopLeft().y + rect.Height() + tempThick);

	// BottomRight에서 Rect 너비의 반만큼 왼쪽으로 이동한 지점
	rgn[3].CreateEllipticRgn(rect.CenterPoint().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.CenterPoint().x + tempThick, rect.BottomRight().y + tempThick);

	// BottomRight 지점
	rgn[4].CreateEllipticRgn(rect.BottomRight().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.BottomRight().x + tempThick, rect.BottomRight().y + tempThick);

	// BottomRight 지점에서 Rect 높이의 반만큼 위로 올라간 지점
	rgn[5].CreateEllipticRgn(rect.BottomRight().x - tempThick, rect.CenterPoint().y- tempThick, 
		rect.BottomRight().x + tempThick, rect.CenterPoint().y + tempThick);

	// BottomRight 지점에서 Rect 높이만큼 위로 올라간 지점
	rgn[6].CreateEllipticRgn(rect.right - tempThick, rect.top - tempThick, rect.right + tempThick, rect.top + tempThick);

	// TopLeft 지점에서 Rect 너비의 반만큼 우측으로 이동한 지점
	rgn[7].CreateEllipticRgn(rect.CenterPoint().x - tempThick, rect.TopLeft().y - tempThick, 
		rect.CenterPoint().x + tempThick, rect.TopLeft().y + tempThick);

	// 정확한 포인트를 찾음
	for(i = 0; i < 8; i++) {
		if(rgn[i].PtInRegion(pt) == TRUE) {
			pos = i;
			i = 8;
		}
	}
	return pos;
}

//--------------------------------------------------------------------------
//　그리기 함수
//--------------------------------------------------------------------------
void GRectangle::Draw(CDC* cdc) {
	
	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// 펜 컬러 및 알파값 지정
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Color foreColor = GObject::COLORREFtoColor(this->m_pBrushColor);
	Color gradientColor = GObject::COLORREFtoColor(this->m_sGrdEndColor);
	Pen pen(penColor, REAL(m_nsThickness));

	pen.SetLineJoin((LineJoin)this->m_nsLineJoin);
	GObject::SetDashStyle(pen, this->m_npPenStyle);

	INT originX, originY;

	// Rect 생성을 위한 좌표보정
	originX = (GObject::m_sStartPoint.x < GObject::m_sEndPoint.x) ? GObject::m_sStartPoint.x : GObject::m_sEndPoint.x;
	originY = (GObject::m_sStartPoint.y < GObject::m_sEndPoint.y) ? GObject::m_sStartPoint.y : GObject::m_sEndPoint.y;

	// Rect의 생성자 중
	// UpperLeft의 X, Y와 Width와 Height를 가지고 생성
	// 따라서 abs 절대값 함수를 이용
	Rect tempRect(originX, originY,
		abs(GObject::m_sEndPoint.x - GObject::m_sStartPoint.x), abs(GObject::m_sEndPoint.y - GObject::m_sStartPoint.y));

	//브러쉬 선언
	SolidBrush pSolidBrush(foreColor);	//단색 브러쉬
	HatchBrush pHatchBrush((HatchStyle)m_nsHatchStyle, foreColor, Color::Transparent);	//격자무늬 브러쉬
	LinearGradientBrush sGradientBrush(tempRect,	foreColor, gradientColor, (LinearGradientMode)m_npGrdMode);	//그라디언트 브러쉬

	//면을 채움
	switch(m_esBrushStyle)	//브러쉬 스타일에 따라 분기
	{
	case SOLID:	//단색 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillRectangle(&pSolidBrush, tempRect);
		break;
	case HATCH: //격자무늬 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillRectangle(&pHatchBrush, tempRect);
		break;
	case GRADIENT: //그라디언트 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillRectangle(&sGradientBrush, tempRect);
		break;
	}

	//외곽선을 그림
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
	// 설정된 펜 색, 두께, 스타일을 이용해 펜 생성
	CPen pen(this->m_npPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);
	CPen *oldPen = cdc->SelectObject(&pen);					// 펜 선택
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
	cdc->SelectObject(&oldPen);					// 이전 펜으로 되돌림
	cdc->SelectObject(&oldBrush);				// 이전 브러쉬로 되돌림*/
}
//--------------------------------------------------------------------------
//　면 타입 설정함수
//--------------------------------------------------------------------------
void GRectangle::SetFaceType(int faceType)
{
	m_esFaceType = (GFaceType)faceType;
}


//--------------------------------------------------------------------------
//　펜 스타일 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetPenStyle(int penStyle)
{
	this->m_npPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//　브러쉬 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetBrushStyle(int brushStyle)
{
	this->m_esBrushStyle = (GBrushType)brushStyle;
}

//--------------------------------------------------------------------------
//　브러쉬 색 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetBrushColor(COLORREF brushColor)
{
	this->m_pBrushColor = brushColor;
}

//--------------------------------------------------------------------------
//　해치 스타일 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetHatchStyle(int hatch)
{
	this->m_nsHatchStyle = hatch;
}

//--------------------------------------------------------------------------
//　라인 꺾임 스타일 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetLineJoinStyle(int lineJoin)
{
	this->m_nsLineJoin = lineJoin;
}

//--------------------------------------------------------------------------
//　그라디언트 모드 설정 함수
//  : Horizontal, Vertical, ForwardDiagonal, BackwardDiagonal
//--------------------------------------------------------------------------
void GRectangle::SetGradientMode(int grdMode)
{
	this->m_npGrdMode = grdMode;
}

//--------------------------------------------------------------------------
//　그라데이션 끝 색 설정 함수
//--------------------------------------------------------------------------
void GRectangle::SetGrdEndColor(COLORREF grdEndColor)
{
	this->m_sGrdEndColor = grdEndColor;
}

//--------------------------------------------------------------------------
//　점선 영역 그리기 함수 by 신희철
//--------------------------------------------------------------------------
void GRectangle::DrawSelectLine(CDC *pDC)
{
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //리전으로 사각형을 만듬
	rect.bottom += 1;
	rect.right +=1;
	pDC->Rectangle(rect); //사각형 점선을 그림

	//그리기 속성 지정
	CBrush brush(RGB(51,94,168)); //브러쉬 지정
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN); //NULL_PEN으로 지정

	//크기 조정시 사용할 점의 위치를 구함
	CPoint sPointArray[8];
	sPointArray[0] = rect.TopLeft();
	sPointArray[1] = CPoint(rect.left, rect.CenterPoint().y);
	sPointArray[2] = CPoint(rect.left, rect.bottom);
	sPointArray[3] = CPoint(rect.CenterPoint().x, rect.bottom);
	sPointArray[4] = CPoint(rect.BottomRight());
	sPointArray[5] = CPoint(rect.right, rect.CenterPoint().y);
	sPointArray[6] = CPoint(rect.right, rect.top);
	sPointArray[7] = CPoint(rect.CenterPoint().x, rect.top);

	//크기 조정을 위한 점 그리기
	for(int i=0;i<8;i++)
		pDC->Ellipse(sPointArray[i].x-5, sPointArray[i].y-5, sPointArray[i].x+5, sPointArray[i].y+5);
	
	//이전 펜과 브러쉬 속성으로 되돌림
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}