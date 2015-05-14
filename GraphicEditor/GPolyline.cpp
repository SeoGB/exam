/* [ GPolyline 클래스 by 신희철 ]
** GPolyline 클래스의 구현부입니다.
*/

#include "StdAfx.h"
#include "GPolyline.h"


IMPLEMENT_SERIAL(GPolyline, GObject, 0)
//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GPolyline::GPolyline(void)
{
	m_sType = POLYLINE;
}
//복사 생성자
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
//　직렬화
//--------------------------------------------------------------------------
void GPolyline::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //저장 시
	{
		ar << (WORD)m_nsPenStyle;
		ar << (WORD)m_nsStartCap;
		ar << (WORD)m_nsEndCap;
		m_sArrayPoint.Serialize(ar);
	}
	else //로드 시
	{
		WORD wTemp;
		ar >> wTemp; m_nsPenStyle = wTemp;
		ar >> wTemp; m_nsStartCap = wTemp;
		ar >> wTemp; m_nsEndCap = wTemp;
		m_sArrayPoint.Serialize(ar);
	}
}
//--------------------------------------------------------------------------
//　Polyline의 영역을 구하는 함수
//--------------------------------------------------------------------------
CRgn* GPolyline::GetRegion(void)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //좌표 시퀀스의 크기를 구함
	if(nsArraySize != 0) //좌표 시퀀스가 비어있지 않을 경우
	{
		//좌표 시퀀스를 순회하며 polyline 모양으로 리전을 만듬
		for(int i=0;i< nsArraySize-1 ; i++)
		{
			CPoint &currPt = m_sArrayPoint.GetAt(i); //현재 좌표
			CPoint &nextPt = m_sArrayPoint.GetAt(i+1); //다음 좌표

			int x = currPt.x;
			int y = currPt.y;
			int w = nextPt.x - currPt.x; //폭
			int h = nextPt.y - currPt.y; //높이

			//두 점 사이의 기울기를 구함
			double theta = w ? atan2((double)w, (double)h) : (~h+1) * M_PI / 2;
			if(theta < 0)
				theta = theta + 2*M_PI;
			
			//기울기를 각도로 변환
			double angle = theta + M_PI/2.0;
			
			//선 두께가 너무 얇으면 선택이 힘드므로 기본 크기는 5
			int dx = (int)((m_nsThickness > 4 ? m_nsThickness : 5) * sin(angle));
			int dy = (int)((m_nsThickness > 4 ? m_nsThickness : 5) * cos(angle));
			
			//리전을 만들기 위해 4개의 꼭지점 지정
			CPoint pt[4];
			pt[0] = CPoint(x+dx, y+dy); //좌측 하단
			pt[1] = CPoint(x-dx, y-dy); //좌측 상단
			pt[2] = CPoint(x+w-dx, y+h-dy); //우측 상단
			pt[3] = CPoint(x+w+dx, y+h+dy); //우측 하단
			
			//리전 좌표 추가
			if(i==0)
				m_sArrayRgnPoint.RemoveAll();
			for(int j=0;j<4;j++)
				m_sArrayRgnPoint.Add(pt[j]);

			if(i==0)
			{	//첫 번째 점일 경우 리전 생성 
				m_sRgn.DeleteObject();
				m_sRgn.CreatePolygonRgn(pt, 4, WINDING);
			}
			else //그 외에는 리전을 합쳐서 만듬
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
//　종료점 반환 함수
//--------------------------------------------------------------------------
CPoint GPolyline::GetEndPoint(void) const
{
	if(m_bsGrouped == FALSE)
		return m_sArrayPoint.GetAt(m_sArrayPoint.GetSize()-1);
	else
		return m_sEndPoint;
}

//--------------------------------------------------------------------------
//　좌표 시퀀스 반환 함수
//--------------------------------------------------------------------------
CArray<CPoint, POINT>* GPolyline::GetArrayPoint(void)
{
	return &m_sArrayPoint; //좌표 시퀀스 반환
}

//--------------------------------------------------------------------------
//　리전 포인트를 얻어오는 함수
//--------------------------------------------------------------------------
CArray<CPoint, POINT>* GPolyline::GetRgnPoint(void)
{
	return &m_sArrayRgnPoint;
}

//--------------------------------------------------------------------------
//　선택 지점의 좌표 시퀀스 인덱스 반환
//--------------------------------------------------------------------------
int GPolyline::FindSelectPoint(CPoint pt)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //좌표 시퀀스의 크기를 구함
	if(nsArraySize != 0)
	{
		//좌표 시퀀스를 하나씩 검사하여 선택 좌표가 각 꼭지점 좌표±선두께 안에 있으면 해당 시퀀스의 인덱스 반환 
		for(int i=0;i<nsArraySize;i++)
		{
			CRgn rgn;
			int tempThick = (m_nsThickness > 4 ? m_nsThickness : 5); //선 두께가 너무 얇으면 선택이 힘드므로 기본 크기는 5 
			CPoint &tempPt = m_sArrayPoint.GetAt(i);
			rgn.CreateEllipticRgn(tempPt.x-tempThick, tempPt.y-tempThick, tempPt.x+tempThick, tempPt.y+tempThick); //리전 생성
			if(rgn.PtInRegion(pt) == TRUE) //리전안에 선택 좌표가 있을 경우
				return i; //해당 시퀀스의 인덱스 반환
		}
	}
	return -1; //없을 경우 -1
}
//--------------------------------------------------------------------------
//　최소 left,top 좌표와 최대 right,bottom 좌표를 찾음: 그룹화 시 필요
//--------------------------------------------------------------------------
void GPolyline::FindStartEndPt()
{
	CArray<int, int&>* arrayX = new CArray<int, int&>; //x좌표에 대한 배열
	CArray<int, int&>* arrayY = new CArray<int, int&>; //y좌표에 대한 배열
	MakeCoordinateArray(arrayX, TRUE);
	MakeCoordinateArray(arrayY, FALSE);

	CQuickSort* qSort = new CQuickSort();
	qSort->QuickSort(arrayX, 0, arrayX->GetSize()-1);
	qSort->QuickSort(arrayY, 0, arrayY->GetSize()-1);

	//시작점 지정(x,y값 배열의 가장 작은 값이 시작점)
	m_sStartPoint.x = arrayX->GetAt(0);
	m_sStartPoint.y = arrayY->GetAt(0);

	//끝점 지정(x,y값 배열의 가장 큰 값이 끝점)
	m_sEndPoint.x = arrayX->GetAt(arrayX->GetSize()-1);
	m_sEndPoint.y = arrayY->GetAt(arrayX->GetSize()-1);

	delete qSort;
	delete arrayX;
	delete arrayY;
}
//--------------------------------------------------------------------------
//　x좌표 또는 y좌표에 대한 배열을 만듬
//--------------------------------------------------------------------------
void GPolyline::MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX)
{
	if(bIsX == TRUE) //x값
	{
		//점 시퀀스의 x값을 전부 array에 추가
		for(int i=0;i<m_sArrayPoint.GetSize();i++)
		{
			int x = m_sArrayPoint.GetAt(i).x;
			array->Add(x);
		}
	}
	else //y값
	{
		//점 시퀀스의 y값을 전부 array에 추가
		for(int i=0;i<m_sArrayPoint.GetSize();i++)
		{
			int y = m_sArrayPoint.GetAt(i).y;
			array->Add(y);
		}
	}
}
//--------------------------------------------------------------------------
//　그룹화 시 사용
//--------------------------------------------------------------------------
int GPolyline::FindFixedPoint(int nCoord, BOOL bIsX)
{
	int nsArraySize = m_sArrayPoint.GetSize(); //좌표 시퀀스의 크기를 구함
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
//　점을 그릴 때 점 추가
//--------------------------------------------------------------------------
void GPolyline::AddOnePt(CPoint pt)
{
	m_sArrayPoint.Add(pt);
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//　시작점 설정 함수
//--------------------------------------------------------------------------
void GPolyline::SetPolyStartPoint(CPoint pt)
{
	m_sArrayPoint.SetAt(0, pt); //좌표 시퀀스의 시작점 지정
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//　종료점 설정 함수
//--------------------------------------------------------------------------
void GPolyline::SetPolyEndPoint(CPoint pt)
{
	m_sArrayPoint.SetAt(m_sArrayPoint.GetSize()-1, pt); //좌표 시퀀스의 종료점 지정
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//　이동 함수(모든 좌표)
//--------------------------------------------------------------------------
void GPolyline::Move(int dX, int dY)
{
	int nArraySize = m_sArrayPoint.GetSize(); //좌표 시퀀스의 크기를 구함
	if(nArraySize > 0) //좌표 시퀀스가 비어있지 않을 경우
	{
		for(int i=0;i<nArraySize;i++) //모든 좌표를 dX, dY만큼 이동
		{
			CPoint &pt = m_sArrayPoint.GetAt(i); 
			pt.x += dX;
			pt.y += dY;
			m_sArrayPoint.SetAt(i, pt); //dx, dy만큼 이동한 좌표를 다시 배열에 지정
		}
	}
	FindStartEndPt();
}

//--------------------------------------------------------------------------
//　이동 함수(선택된 하나의 좌표)
//--------------------------------------------------------------------------
void GPolyline::Resize(int index, int dX, int dY)
{
	if(m_bsGrouped == FALSE) //그룹화되어있지 않을경우
	{
		CPoint &tempPt = m_sArrayPoint.GetAt(index); //해당 인덱스의 좌표를 받아옴
		//해당 좌표를 dx,dy만큼 이동
		tempPt.x += dX;
		tempPt.y += dY;
		m_sArrayPoint.SetAt(index, tempPt); //dx,dy만큼 이동한 좌표를 다시 배열에 지정
		FindStartEndPt();
	}
}
//--------------------------------------------------------------------------
//　삭제(선택된 하나의 좌표)
//--------------------------------------------------------------------------
BOOL GPolyline::DelOnePt(int index)
{
	if(m_sArrayPoint.GetSize() > 2) //점이 2개 넘게 있을 경우 한 점을 지움
	{
		m_sArrayPoint.RemoveAt(index); //해당 인덱스의 좌표 삭제]
		FindStartEndPt();
		return TRUE;
	}
	else
	{
		delete this; //점이 2개밖에 남지 않았으면 해당 객체 제거
		return FALSE;
	}
}

//--------------------------------------------------------------------------
//　펜 스타일 설정 함수
//--------------------------------------------------------------------------
void GPolyline::SetPenStyle(int penStyle)
{
	m_nsPenStyle = penStyle;
}

//--------------------------------------------------------------------------
//　선 시작 모양 설정 함수
//--------------------------------------------------------------------------
void GPolyline::SetStartCap(int startCap)
{
	m_nsStartCap = startCap;
}

//--------------------------------------------------------------------------
//　선 끝 모양 설정 함수
//--------------------------------------------------------------------------
void GPolyline::SetEndCap(int endCap)
{
	m_nsEndCap = endCap;
}

//--------------------------------------------------------------------------
//　그리기 함수
//--------------------------------------------------------------------------
void GPolyline::Draw(CDC *pDC)
{
	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality); //안티앨리어싱

	GraphicsPath p;

	//펜 컬러 및 알파값 지정
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	//라인의 시작과 끝부분 캡지정
	pen.SetStartCap((LineCap)m_nsStartCap);
	pen.SetEndCap((LineCap)m_nsEndCap);

	//펜 스타일 지정
	GObject::SetDashStyle(pen, m_nsPenStyle);

	//CPoint형의 점 시퀀스를 Point형의 점 시퀀스로 변환
	int nArraySize = m_sArrayPoint.GetSize();
	Point *pts = new Point[nArraySize];
	for(int i=0;i<nArraySize;i++)
		pts[i] = GObject::CPointToPoint(m_sArrayPoint.GetAt(i));

	graphics.DrawLines(&pen, pts, nArraySize);

	//////////////////////////////////////////////////////////////////////////
	// 이전에 사용하던 그리기
//	CPen pen(m_nsPenStyle, m_nsThickness, m_sLineColor); //펜 그리기
//	CPen* oldPen = pDC->SelectObject(&pen); //펜 선택
//	
//	pDC->Polyline(m_sArrayPoint.GetData(), m_sArrayPoint.GetSize()); //polyline 그리기
//
//	pDC->SelectObject(&oldPen);				//이전 펜으로 되돌림
}

//--------------------------------------------------------------------------
//　선택 영역 점선 그리기 함수
//--------------------------------------------------------------------------
void GPolyline::DrawSelectLine(CDC *pDC)
{	
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	//점선으로 라인 모양대로 테두리를 그림
	this->GetRegion();
	for(int i=0;i<m_sArrayRgnPoint.GetSize();i+=4 /* 점 4개씩 사용하므로 */) //라인 하나마다 점선 테두리를 그림
	{
		CPoint sPt[4]; //라인하나 당 테두리를 그리려면 4개의 점이 필요
		for(int j=0;j<4;j++)
			sPt[j] = m_sArrayRgnPoint.GetAt(i+j); //점 하나씩 sPt에 지정
		pDC->Polygon(sPt, 4); //테두리 그리기
	}

	//그리기 속성 설정
	CBrush brush(RGB(51,94,168)); //브러쉬 생성
	CBrush* oldBrush = pDC->SelectObject(&brush); //브러쉬 지정
	pDC->SelectStockObject(NULL_PEN);

	//각 꼭지점마다 점을 그림
	for(int i=0;i<m_sArrayPoint.GetSize();i++) //점 시퀀스의 사이즈만큼 반복
	{
		CPoint sPt = m_sArrayPoint.GetAt(i);
		pDC->Ellipse(sPt.x-5, sPt.y-5, sPt.x+5, sPt.y+5); //각 꼭지점 그리기
	}

	//이전 펜과 브러쉬 속성으로 되돌림
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

//--------------------------------------------------------------------------
//　그리기 도중에 지정된 속성으로 커서를 따라 라인을 그림
//--------------------------------------------------------------------------
void GPolyline::DrawingLine(CDC* pDC, CPoint point)
{
	Graphics graphics(*pDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality); //안티앨리어싱

	//펜 컬러 및 알파값 지정
	Color penColor = GObject::COLORREFtoColor(GObject::m_sLineColor);
	Pen pen(penColor, REAL(m_nsThickness));

	//펜 스타일 지정
	GObject::SetDashStyle(pen, m_nsPenStyle);

	//라인의 시작과 끝부분 캡지정
	pen.SetStartCap((LineCap)m_nsStartCap);
	pen.SetEndCap((LineCap)m_nsEndCap);

	Point endPoint = GObject::CPointToPoint(m_sArrayPoint.GetAt(m_sArrayPoint.GetSize()-1));
	Point currPoint = GObject::CPointToPoint(point);

	graphics.DrawLine(&pen, endPoint, currPoint);
}


//--------------------------------------------------------------------------
//　선택된 점에 대해 그림
//--------------------------------------------------------------------------
void GPolyline::DrawSelectedPoint(CDC* pDC, int index)
{
	//그리기 속성 설정
	CBrush brush(RGB(255,0,0)); //브러쉬 생성
	CBrush* oldBrush = pDC->SelectObject(&brush); //브러쉬 지정

	CPoint pt = m_sArrayPoint[index];
	pDC->Ellipse(pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	//이전 펜으로 되돌림
	pDC->SelectObject(oldBrush);
}


//--------------------------------------------------------------------------
//　그룹 리사이징 시 포인트를 저장
//--------------------------------------------------------------------------
void GPolyline::SavePolylinePoints()
{
	int nArraySize = m_sArrayPoint.GetSize();
	if(nArraySize > 0) //좌표 시퀀스가 비어있지 않은 경우
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
//　그룹 리사이징 시 포인트를 재지정
//--------------------------------------------------------------------------
void GPolyline::ApplyPolylinePoints(double px, double py, int sx, int sy)
{
	int nArraySize = m_sArrayPoint.GetSize(); //좌표 시퀀스의 크기를 구함
	if(nArraySize > 0) //좌표 시퀀스가 비어있지 않을 경우
	{
		for(int i=0;i<nArraySize;i++) //모든 좌표 조정
		{
			CPoint &prevPt = m_sPrevArrayPoint.GetAt(i);
			CPoint &pt = m_sArrayPoint.GetAt(i); 
			pt.x = (int)(sx + (prevPt.x-sx) * px);
			pt.y = (int)(sy + (prevPt.y-sy) * py);
			m_sArrayPoint.SetAt(i, pt); //조정한 좌표를 다시 배열에 지정
		}
	}
	FindStartEndPt();
}