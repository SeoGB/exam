/* [ GObject 클래스 by 신희철 ]
** GObject 클래스의 구현부입니다.
*/

#include "StdAfx.h"
#include "GObject.h"

IMPLEMENT_SERIAL(GObject, CObject, 0)

//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GObject::GObject(void)
{
	m_nsThickness = 1;
	m_sLineColor = RGB(0,0,0);
	m_bsGrouped = FALSE;
	m_nsAlpha = 100;
}
//소멸자
GObject::~GObject(void)
{}
//--------------------------------------------------------------------------
//　직렬화
//--------------------------------------------------------------------------
void GObject::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring()) //저장
	{
		ar << (WORD)m_sType;
		ar << m_sStartPoint;
		ar << m_sEndPoint;
		ar << m_sLineColor;
		ar << m_nsThickness;
		ar << (WORD)m_bsGrouped;
		ar << m_nsAlpha;
	}
	else //로드
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
//　클릭 지점이 리전안에 있는지 여부 반환
//--------------------------------------------------------------------------
BOOL GObject::PtInGObj(CPoint pt)
{
	CRgn *rgn = GetRegion(); //그래피컬 개체 모양으로 리전 생성
	return rgn->PtInRegion(pt); //클릭 지점이 리전 안에 있는지 반환
}

//--------------------------------------------------------------------------
//　COLORREF형을 GDI+에서 사용하는 Color형으로 변환
//--------------------------------------------------------------------------
Color GObject::COLORREFtoColor(COLORREF color)
{
	//불투명도를 구함
	int nAlpha = (int)((m_nsAlpha / 100.0) * 255);

	//R,G,B값을 얻어옴
	int nR = GetRValue(color);
	int nG = GetGValue(color);
	int nB = GetBValue(color);

	//컬러 반환
	return Color(nAlpha, nR, nG, nB);
}
//--------------------------------------------------------------------------
//　CPoint형을 GDI+에서 사용하는 Point형으로 변환
//--------------------------------------------------------------------------
Point GObject::CPointToPoint(CPoint point)
{
	return Point(point.x, point.y);
}

//--------------------------------------------------------------------------
//　시작점 설정
//--------------------------------------------------------------------------
void GObject::SetStartPoint(CPoint pt)
{
	m_sStartPoint = pt;
}

//--------------------------------------------------------------------------
//　종료점 설정
//--------------------------------------------------------------------------
void GObject::SetEndPoint(CPoint endPoint)
{
	m_sEndPoint = endPoint;
}

//--------------------------------------------------------------------------
//　시작점 및 종료점 설정
//--------------------------------------------------------------------------
void GObject::SetBothPoint(CPoint startPt, CPoint endPt)
{
	m_sStartPoint = startPt;
	m_sEndPoint = endPt;
}

//--------------------------------------------------------------------------
//　라인 컬러 설정
//--------------------------------------------------------------------------
void GObject::SetLineColor(COLORREF color)
{
	m_sLineColor = color;
}

//--------------------------------------------------------------------------
//　라인 굵기 설정
//--------------------------------------------------------------------------
void GObject::SetThickness(int thickness)
{
	m_nsThickness = thickness;
}
//--------------------------------------------------------------------------
//　그룹 여부 설정
//--------------------------------------------------------------------------
void GObject::SetGrouped(BOOL isGrouped) 
{ 
	m_bsGrouped = isGrouped;
}

//--------------------------------------------------------------------------
//　펜 설정
//--------------------------------------------------------------------------
void GObject::SetDashStyle(Pen& pen, int nPenStyle)
{
	//굵기가 1일 경우에만 변화가 미세하여 커스텀 스타일 적용
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
			arrayDash[1] = 6.0f; //간격
			
			break;
		case 2: //DashStyleDot
			nCount = 2;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 3.0f; //dot
			arrayDash[1] = 3.0f; //간격
			pen.SetDashPattern(arrayDash,3);
			break;
		case 3: //DashStyleDashDot
			nCount = 4;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 9.0f; //dash
			arrayDash[2] = 3.0f; //dot
			arrayDash[1] = arrayDash[3] = 6.0f; //간격
			pen.SetDashPattern(arrayDash,4);
			break;
		case 4: //DashStyleDashDotDot
			nCount = 6;
			arrayDash = new REAL[nCount];
			arrayDash[0] = 9.0f; //dash
			arrayDash[2] = arrayDash[4] = 3.0f; //dot
			arrayDash[1] = arrayDash[3] = arrayDash[5] = 3.0f; //간격
			pen.SetDashPattern(arrayDash,6);
			break;
		}
		pen.SetDashStyle(DashStyleCustom); //커스텀 스타일
		pen.SetDashPattern(arrayDash,nCount);
	}
	//굵기가 2 이상이거나 DashStyleSolid일 경우 dash style 그대로 적용
	else
		pen.SetDashStyle((DashStyle)nPenStyle);
}

//--------------------------------------------------------------------------
//　불투명도 설정
//--------------------------------------------------------------------------
void GObject::SetAlpha(int alpha)
{
	m_nsAlpha = alpha;
}

//--------------------------------------------------------------------------
//　그룹화 리사이징을 위함(by 박현덕)
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
