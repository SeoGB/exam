/* [ GPolyline 클래스 by 신희철 ]
** GPolyline 클래스는 폴리라인에 관한 정보와 속성을 가진 클래스입니다.
*/

#pragma once
#include "GObject.h"
#include "QuickSort.h"

class GPolyline : public GObject
{
protected:
	DECLARE_SERIAL(GPolyline);
public:
	GPolyline(void);						//생성자
	GPolyline(const GPolyline& pPolyline);	//복사 생성자

	virtual void Serialize(CArchive& ar);	//직렬화

	//Accessor 함수
	virtual CRgn* GetRegion(void);								//리전을 구함
	int GetPenStyle() const {return m_nsPenStyle; }	//펜 스타일 반환
	virtual CPoint GetEndPoint(void) const;						//끝 점 반환
	CArray<CPoint, POINT>* GetArrayPoint(void);					//좌표 시퀀스 반환
	int FindSelectPoint(CPoint pt);								//선택 지점의 좌표 시퀀스 인덱스 반환
	CArray<CPoint, POINT>* GetRgnPoint(void);					//리전 포인트 반환
	void FindStartEndPt(void);									//그룹화 시 사용할 시작,끝점을 찾음
	void MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX); //x,y좌표의 배열을 만듬
	int FindFixedPoint(int nCoord, BOOL bIsX);
	int GetStartCap(void) const {return m_nsStartCap; } //라인 시작 모양 반환
	int GetEndCap(void) const {return m_nsEndCap; } //라인 끝 모양 반환
	
	//Mutator 함수
	void AddOnePt(CPoint pt);									//점을 그릴 때 점 추가
	void SetPolyStartPoint(CPoint pt);							//시작점 변경
	void SetPolyEndPoint(CPoint pt);							//끝점 변경
	virtual void Move(int dX, int dY);							//전체 좌표 이동
	void Resize(int index, int dX, int dY);						//한 점 이동
	BOOL DelOnePt(int index);									//점 지우기

	void SetPenStyle(int penStyle);								//펜 스타일 설정
	void SetStartCap(int startCap);								//선 시작 모양 설정
	void SetEndCap(int endCap);									//선 끝 모양 설정

	virtual void Draw(CDC* cdc);								//이 함수에서 그래피컬 개체 그림
	virtual void DrawSelectLine(CDC *pDC);						// 선택 시 점선으로 영역을 그림
	void DrawingLine(CDC* pDC, CPoint point);					//그리기 도중에 지정된 속성으로 커서를 따라 라인을 그림

	//폴리라인의 그룹화 리사이즈 시 사용
	void SavePolylinePoints();
	void ApplyPolylinePoints(double px, double py, int sx, int sy);

protected:
	CArray<CPoint, POINT> m_sArrayPoint;	//좌표 시퀀스
	CArray<CPoint, POINT> m_sPrevArrayPoint;	//이전 좌표 시퀀스
	int m_nsPenStyle;						//펜 스타일 
	CArray<CPoint, POINT> m_sArrayRgnPoint;	//리전 좌표

	//for GDI+, 120528 추가 by 신희철
	int m_nsStartCap;	//라인 시작 모양
	int m_nsEndCap;		//라인 끝 모양
public:

	void DrawSelectedPoint(CDC* pDC, int index);
};

