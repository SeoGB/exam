/* [ GLine 클래스 by 박현덕 ]
** 그래피컬 개체의 제일 최상위 클래스 GObject를 상속받아 정의한
** GLine 클래스입니다.
*/
#pragma  once
#include "GObject.h"

class GLine : public GObject {
protected:
	DECLARE_SERIAL(GLine);
public :		// 멤버함수
	GLine(void);				// 생성자
	GLine(const GLine& pGLine);	//복사 생성자
	~GLine(void);				// 소멸자

	virtual void Serialize(CArchive& ar);	// 직렬화

	// Accessor 함수
	CRgn* GetRegion();						// 리전을 반환
	int GetPenStyle() const {return m_nsPenStyle; }	//펜 스타일 반환
	CPoint* GetRgnPoint();					// 리전의 각 꼭지점을 반환
	int GetStartCap(void) const {return m_nsStartCap; } //라인 시작 모양 반환
	int GetEndCap(void) const {return m_nsEndCap; } //라인 끝 모양 반환

	// Mutator 함수
	void Move(int dX, int dY);				// 좌표 이동
	void Resize(int index, int dX, int dY);	// 한 점 이동
	int FindSelectPoint(CPoint pt);			// 선택 지점의 좌표 시퀀스 인덱스 반환

	void SetPenStyle(int penStyle);			// 펜 스타일 설정
	void SetStartCap(int startCap);								//선 시작 모양 설정
	void SetEndCap(int endCap);									//선 끝 모양 설정

	void Draw(CDC* cdc);					// 그리는 함수
	virtual void DrawSelectLine(CDC *pDC);	// 선택 시 점선으로 영역을 그림

private :		// 멤버 변수
	int m_nsPenStyle;						// 펜 스타일
	CPoint m_pRgnPoint[4];					// 선택시 리전을 구성하는 포인트배열

	//for GDI+, 120528 추가 by 신희철
	int m_nsStartCap;	//라인 시작 모양
	int m_nsEndCap;		//라인 끝 모양
public:
};