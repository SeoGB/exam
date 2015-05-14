/* [ GEllipse 클래스 by 박현덕 ]
** GEllipse 클래스는 원에 대한 정보와 속성을 가진 클래스입니다.
*/

#include "StdAfx.h"
#include "GEllipse.h"

enum POSITION_LIST {TOPLEFT, HALFTOPLEFT, BOTTOMLEFT, HALFBOTTOMLEFT, BOTTOMRIGHT,
	  HALFBOTTOMRIGHT, TOPRIGHT, HALFTOPRIGHT};

IMPLEMENT_SERIAL(GEllipse, GObject, 0)

//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자



//hhh
GEllipse::GEllipse(void) {
	GObject::m_sType = ELLIPSE;
}

//복사 생성자
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

// 소멸자
GEllipse::~GEllipse(void) {
}

//--------------------------------------------------------------------------
//　직렬화 by 신희철
//--------------------------------------------------------------------------
void GEllipse::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //저장
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
	else //로드
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
//　양 끝 점 설정 함수(이동)
//--------------------------------------------------------------------------
void GEllipse::Move(int dX, int dY){
	
	GObject::m_sStartPoint.Offset(dX, dY);
	GObject::m_sEndPoint.Offset(dX, dY);
}

//--------------------------------------------------------------------------
//　리전 생성 함수
//--------------------------------------------------------------------------
CRgn* GEllipse::GetRegion() {

	// 사각형 모양으로 리전을 만듬
	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint);

	// 선 두께가 너무 얇으면 선택이 힘드므로 기본 크기는 5
	int dx = (int)(m_nsThickness > 4 ? m_nsThickness : 5);
	int dy = (int)(m_nsThickness > 4 ? m_nsThickness : 5);

	m_sRgn.DeleteObject();				// 이미 만들어진 영역이 있다면 삭제

	// 영역을 재구성
	GObject::m_sRgn.CreateRectRgn(rect.TopLeft().x - m_nsThickness/2-1, rect.TopLeft().y - m_nsThickness/2-1, 
		rect.BottomRight().x + m_nsThickness/2+1, rect.BottomRight().y + m_nsThickness/2+1);

	return &m_sRgn;
}

//-------------------------------------------------------------
//　도형 크기 재설정 함수
//  : 인자로 넘어온 포인트를 기준으로 어느 위치인지 파악
//    8방향으로 되어있는 임의의 사각형을 생각하면 된다
//	  *	 *	*
//	  *	 *	*
//	  *	 *	*
//	  반시계 방향으로 TOPLEFT -> HALFTOPLEFT -> BOTTOMLEFT
//	  HALFBOTTOMLEFT -> BOTTOMRIGHT -> HALFBOTTOMRIGHT
//	  TOPRIGHT -> HALFTOPRIGHT
//-------------------------------------------------------------
void GEllipse::Resize(int index, int dX, int dY) {

	if(GObject::m_bsGrouped == FALSE) //그룹화 되어있지 않은 경우
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
	else		// 그룹화 되어있는 경우
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
//　포인트 위치 찾는 함수
//--------------------------------------------------------------------------
int GEllipse::FindSelectPoint(CPoint pt) {

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
	rgn[2].CreateEllipticRgn(rect.TopLeft().x - tempThick, rect.BottomRight().y - tempThick, 
		rect.TopLeft().x + tempThick, rect.BottomRight().y + tempThick);

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

	// 찾고자하는 점의 위치를 찾는 과정
	for(i = 0; i < 8; i++) {
		if(rgn[i].PtInRegion(pt) == TRUE) {
			pos = i;
			i = 8;
		}
	}
	return pos;
}
//--------------------------------------------------------------------------
//　면 타입 설정함수
//--------------------------------------------------------------------------
void GEllipse::SetFaceType(int faceType)
{
	m_esFaceType = (GFaceType)faceType;
}

//--------------------------------------------------------------------------
//　브러쉬 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetBrushStyle(int brushStyle)
{
	this->m_esBrushStyle = (GBrushType)brushStyle;
}

//--------------------------------------------------------------------------
//　브러쉬 색 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetBrushColor(COLORREF brushColor) {
	this->m_pBrushColor = brushColor;
}

//--------------------------------------------------------------------------
//　펜 스타일 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetPenStyle(int penStyle)
{
	m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//　해치 스타일 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetHatchStyle(int hatch)
{
	this->m_nsHatchStyle = hatch;
}

//--------------------------------------------------------------------------
//　라인 꺾임 스타일 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetLineJoinStyle(int lineJoin)
{
	this->m_nsLineJoin = lineJoin;
}

//--------------------------------------------------------------------------
//　그라디언트 모드 설정 함수
//  : Horizontal, Vertical, ForwardDiagonal, BackwardDiagonal
//--------------------------------------------------------------------------
void GEllipse::SetGradientMode(int grdMode)
{
	this->m_npGrdMode = grdMode;
}

//--------------------------------------------------------------------------
//　그라데이션 끝 색 설정 함수
//--------------------------------------------------------------------------
void GEllipse::SetGrdEndColor(COLORREF grdEndColor)
{
	this->m_sGrdEndColor = grdEndColor;
}

//--------------------------------------------------------------------------
//　그리기 함수
//--------------------------------------------------------------------------
void GEllipse::Draw(CDC* cdc) {

	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// 펜 컬러 및 알파값 지정
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Color foreColor = GObject::COLORREFtoColor(this->m_pBrushColor);
	Color gradientColor = GObject::COLORREFtoColor(this->m_sGrdEndColor);
	Pen pen(penColor, REAL(m_nsThickness));
	
	pen.SetLineJoin((LineJoin)this->m_nsLineJoin);	//라인 조인(외곽선 형태) 설정
	GObject::SetDashStyle(pen, this->m_nsPenStyle); //라인 패턴 설정
	Rect tempRect(GObject::m_sStartPoint.x, GObject::m_sStartPoint.y,
		GObject::m_sEndPoint.x - GObject::m_sStartPoint.x, GObject::m_sEndPoint.y - GObject::m_sStartPoint.y);

	//브러쉬 선언
	SolidBrush pSolidBrush(foreColor);	//단색 브러쉬
	HatchBrush pHatchBrush((HatchStyle)m_nsHatchStyle, foreColor, Color::Transparent);	//격자무늬 브러쉬
	LinearGradientBrush sGradientBrush(tempRect,	foreColor, gradientColor, (LinearGradientMode)m_npGrdMode);	//그라디언트 브러쉬
	
	//면을 채움
	switch(m_esBrushStyle)	//브러쉬 스타일에 따라 분기
	{
	case SOLID:	//단색 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillEllipse(&pSolidBrush, tempRect);
		break;
	case HATCH: //격자무늬 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillEllipse(&pHatchBrush, tempRect);
		break;
	case GRADIENT: //그라디언트 스타일
		if(m_esFaceType != OUTLINE) //외곽선만 그리기가 아닐 때 면을 채움
			graphics.FillEllipse(&sGradientBrush, tempRect);
		break;
	}

	//외곽선을 그림
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
	CPen pen(m_nsPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);	// 펜 생성
	CPen *oldPen = cdc->SelectObject(&pen);									// 펜 선택
	CBrush brush;

	if(m_npBrushStyle == BS_SOLID) {			// BS_SOLID 스타일일경우
		brush.CreateSolidBrush(m_pBrushColor);	// SOLID 브러쉬를 만듬
	}
	else {
		brush.CreateHatchBrush(m_lpHatchStyle, m_pBrushColor); // HATCH 브러쉬 생성
	}

	CBrush *oldBrush = cdc->SelectObject(&brush); // 브러쉬객체 선택

	CRect rect(GObject::m_sStartPoint, GObject::m_sEndPoint); // Rect 생성

	cdc->Ellipse(rect);	//타원을 그림
	cdc->SelectObject(&oldPen);					// 이전펜으로 되돌림
	cdc->SelectObject(&oldBrush);				// 이전 브러쉬로 되돌림*/
}

//--------------------------------------------------------------------------
//　선택 영역 점선 그리기 함수 by 신희철
//--------------------------------------------------------------------------
void GEllipse::DrawSelectLine(CDC *pDC)
{
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));			// dot 스타일의 점선 펜 생성
	CPen* oldPen = pDC->SelectObject(&penDotted);		// dot 스타일 펜 선택

	CRect rect;
	GetRegion()->GetRgnBox(rect); //리전으로 사각형을 만듬
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