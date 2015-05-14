/* [ GObject 클래스 by 신희철 ]
** 그래피컬 개체의 제일 최상위 클래스입니다.
** Line, Polyline, Rectangle, Ellipse, Text, Group은 모두 이 클래스를 상속받아 정의됩니다.
*/

#pragma once
enum GraphicalObjectType
{
	SELECT,
	LINE,
	POLYLINE,
	ELLIPSE,
	RECTANGLE,
	TEXT,
	GGROUP
}; //타입 지정 시 사용

enum GBrushType
{
	SOLID,		//솔리드 브러쉬
	HATCH,		//해치 브러쉬
	GRADIENT	//그라디언트 브러쉬
}; //브러쉬 타입 지정 시 사용

enum GFaceType
{
	OUTLINE,		//외곽선만 그리기
	OUTLINEDFILL,	//외곽선+채우기해서 그리기
	FILLONLY		//채우기만 하기
}; //면의 그리기 시 사용

class GObject : public CObject
{
protected:
	DECLARE_SERIAL(GObject);
public:		//멤버 함수
	//생성자 & 소멸자
	GObject(void);
	~GObject(void);

	virtual void Serialize(CArchive& ar);						//직렬화 함수

	//Accessor 함수
	GraphicalObjectType GetType(void) const { return m_sType; }	//그래피컬 개체의 타입을 반환
	
	CPoint GetStartPoint(void) const { return m_sStartPoint; }	//시작점 반환
	CPoint GetEndPoint(void) const { return m_sEndPoint;}		//종료점 반환
	
	COLORREF GetLineColor(void) const { return m_sLineColor; }	//라인 색 반환
	int GetThickness(void) const { return m_nsThickness; }		//라인 굵기 반환
	
	int GetAplha(void) const { return m_nsAlpha; }				//불투명도 반환

	BOOL PtInGObj(CPoint pt);									//클릭 지점이 리전안에 있는지 여부 반환
	virtual CRgn* GetRegion() { return &m_sRgn; }				//그래피컬 개체의 리전을 구함

	BOOL IsGrouped(void) const { return m_bsGrouped; }			//그룹 여부 반환

	Color COLORREFtoColor(COLORREF color);						//COLORREF형을 GDI+에서 사용하는 Color형으로 변환
	Point CPointToPoint(CPoint point);							//CPoint형을 GDI+에서 사용하는 Point형으로 변환

	//Mutator 함수
	void SetStartPoint(CPoint pt);								//시작점 변경
	void SetEndPoint(CPoint endPoint);							//끝점 변경
	void SetBothPoint(CPoint startPt, CPoint endPt);			//양 끝점을 변경
	
	virtual void Move(int dX, int dY) {};						//좌표 이동
	virtual void Resize(int index, int dX, int dY) {}			//크기(또는 점 위치) 변경
	virtual int FindSelectPoint(CPoint pt) { return -1; }		//선택한 점을 찾는 함수

	void SetLineColor(COLORREF color);							//라인 색 변경
	void SetThickness(int thickness);							//라인 굵기 변경
	void SetGrouped(BOOL isGrouped);							//그룹 여부 설정

	void SetDashStyle(Pen& pen, int nPenStyle);					//펜 설정

	void SetAlpha(int alpha);									//불투명도 설정

	//그리는 함수
	virtual void Draw(CDC* cdc) {}								//이 함수에서 그래피컬 개체 그림
	virtual void DrawSelectLine(CDC *pDC){}						//선택 시 점선으로 영역을 그림

	// PHD
	void SavePoints();
	void ApplyPoints(double px, double py, int sx, int sy);

	CPoint m_pResizeStart, m_pResizeEnd;

protected:	//멤버 변수
	GraphicalObjectType m_sType;	//그래피컬 개체의 타입
	CPoint m_sStartPoint;			//시작점(left,top의 좌표)
	CPoint m_sEndPoint;				//종료점(right,bottom의 좌표)
	COLORREF m_sLineColor;			//선 색
	int m_nsThickness;				//선 굵기
	CRgn m_sRgn;					//그래피컬 개체의 리전
	BOOL m_bsGrouped;				//그룹화 여부

	//for GDI+, 120528 추가 by 신희철
	int m_nsAlpha;					//불투명도
};
