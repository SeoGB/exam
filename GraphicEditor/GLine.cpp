/* [ GLine 클래스 by 박현덕 ]
** GLine 클래스의 구현부입니다.
*/

#include "StdAfx.h"
#include "GLine.h"

IMPLEMENT_SERIAL(GLine, GObject, 0)
//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GLine::GLine() {
	GObject::m_sType = LINE;
}

//복사 생성자
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

//소멸자
GLine::~GLine() {}
//--------------------------------------------------------------------------
//　직렬화 by 신희철
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
//　선 객체에 대한 영역을 설정 by 신희철
//--------------------------------------------------------------------------
CRgn* GLine::GetRegion() {

	//라인 모양으로 리전을 만듬
	int x = m_sStartPoint.x;
	int y = m_sStartPoint.y;
	int w = GObject::m_sEndPoint.x - m_sStartPoint.x; //폭
	int h = GObject::m_sEndPoint.y - m_sStartPoint.y; //높이

	//두 점 사이의 기울기를 구함
	double theta = w ? atan2((double)w, (double)h) : (~h+1) * M_PI / 2;
	if(theta < 0)
		theta = theta + 2*M_PI;

	//기울기로 각도를 구함
	double angle = theta + M_PI/2;

	//선 두께가 너무 얇으면 선택이 힘드므로 기본 크기는 5
	int dx = (int)((m_nsThickness > 4 ? m_nsThickness+1 : 5) * sin(angle));
	int dy = (int)((m_nsThickness > 4 ? m_nsThickness+1 : 5) * cos(angle));

	//리전을 만들기 위해 4개의 꼭지점 지정
	CPoint pt [4];
	pt[0] = CPoint(x+dx, y+dy); //좌측 하단
	pt[1] = CPoint(x-dx, y-dy); //좌측 상단
	pt[2] = CPoint(x+w-dx, y+h-dy); //우측 상단
	pt[3] = CPoint(x+w+dx, y+h+dy); //우측 하단

	// 각 리전의 꼭지점을 배열에 저장
	for(int i = 0; i < 4; i++) {
		this->m_pRgnPoint[i] = pt[i];
	}

	//리전 생성
	GObject::m_sRgn.DeleteObject();
	GObject::m_sRgn.CreatePolygonRgn(pt, 4, WINDING);
	//리전 반환
	return &m_sRgn;
}

//--------------------------------------------------------------------------
//　리전 설정 시 4개의 좌표를 CPoint 배열로 관리
//--------------------------------------------------------------------------
CPoint* GLine::GetRgnPoint(void) {
	return this->m_pRgnPoint;
}

//--------------------------------------------------------------------------
//　이동 함수(모든 좌표)
//--------------------------------------------------------------------------
void GLine::Move(int dX, int dY) {

	GObject::m_sStartPoint.Offset(dX, dY);	// 시작점을 dX, dY만큼 이동
	this->GObject::m_sEndPoint.Offset(dX, dY);		// 종료점을 dX, dY만큼 이동
}

//--------------------------------------------------------------------------
//　이동 함수(선택된 하나의 좌표)
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
//　선택 지점의 좌표 포인트 반환
//--------------------------------------------------------------------------
int GLine::FindSelectPoint(CPoint pt) {
	
	int tempThick = (GObject::m_nsThickness > 4 ? GObject::m_nsThickness : 5); // 기본 크기는 5 
	
	CRgn rgn;
	CPoint tempPt = GObject::m_sStartPoint;
	rgn.CreateEllipticRgn(tempPt.x - tempThick, tempPt.y - tempThick, tempPt.x + tempThick, tempPt.y + tempThick); // 리전 생성

	// 시작점이 Rgn 안에 있으며 시작점이 선택
	if(rgn.PtInRegion(pt) == TRUE) {
		return 0;
	}

	tempPt = GObject::m_sEndPoint;
	rgn.DeleteObject();
	rgn.CreateEllipticRgn(tempPt.x - tempThick, tempPt.y - tempThick, tempPt.x + tempThick, tempPt.y + tempThick); // 리전 생성

	// 끝점이 선택된 경우
	if(rgn.PtInRegion(pt) == TRUE) {
		return 4;
	}
	return -1;
}

//--------------------------------------------------------------------------
//　펜 스타일 설정 함수
//--------------------------------------------------------------------------
void GLine::SetPenStyle(int penStyle)
{
	this->m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//　선 시작 모양 설정 함수
//--------------------------------------------------------------------------
void GLine::SetStartCap(int startCap)
{
	m_nsStartCap = startCap;
}

//--------------------------------------------------------------------------
//　선 끝 모양 설정 함수
//--------------------------------------------------------------------------
void GLine::SetEndCap(int endCap)
{
	m_nsEndCap = endCap;
}

//--------------------------------------------------------------------------
// 그리기 함수
//--------------------------------------------------------------------------
void GLine::Draw(CDC* pDC) {

	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);		// Antialiasing

	// 펜 컬러 및 알파값 지정
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	// 라인의 시작과 끝부분 모양지정
	pen.SetStartCap((LineCap)this->m_nsStartCap);
	pen.SetEndCap((LineCap)this->m_nsEndCap);

	// 펜 스타일 지정
	GObject::SetDashStyle(pen, this->m_nsPenStyle);

	graphics.DrawLine(&pen, Point(GObject::m_sStartPoint.x, GObject::m_sStartPoint.y), 
		Point(GObject::m_sEndPoint.x, GObject::m_sEndPoint.y));

// 	CPen pen(m_nsPenStyle, GObject::m_nsThickness, GObject::m_sLineColor);	// 펜 생성
// 	CPen *oldPen = pDC->SelectObject(&pen);	// 펜 선택
// 
// 	pDC->MoveTo(GObject::m_sStartPoint);// 선그리기
// 	pDC->LineTo(GObject::m_sEndPoint);	// 선그리기
// 
// 	pDC->SelectObject(&oldPen); // 이전펜으로 되돌림
}

//--------------------------------------------------------------------------
// 선택 영역 점선 그리기 함수 by 신희철
//--------------------------------------------------------------------------
void GLine::DrawSelectLine(CDC *pDC)
{
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	//점선으로 라인 모양대로 테두리를 그림
	this->GetRegion();
	pDC->Polygon(this->GetRgnPoint(), 4);

	//그리기 속성 설정
	CBrush brush(RGB(51,94,168)); //브러쉬 생성
	CBrush* oldBrush = pDC->SelectObject(&brush); //브러쉬 지정
	pDC->SelectStockObject(NULL_PEN);

	//각 시작점과 끝점에 점을 그림
	pDC->Ellipse(GObject::m_sStartPoint.x-5, GObject::m_sStartPoint.y-5, GObject::m_sStartPoint.x+5, GObject::m_sStartPoint.y+5); //시작점 그리기
	pDC->Ellipse(GObject::m_sEndPoint.x-5, GObject::m_sEndPoint.y-5, GObject::m_sEndPoint.x+5, GObject::m_sEndPoint.y+5); //끝점 그리기
	pDC->SelectObject(&oldBrush);

	//이전 펜과 브러쉬 속성으로 되돌림
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}
