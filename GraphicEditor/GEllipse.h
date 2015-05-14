/* [ GEllipse 클래스 by 박현덕 ]
** 그래피컬 개체의 제일 최상위 클래스 GObject를 상속받아 정의한
** GEllipse 클래스입니다.
*/
#pragma once
#include "GObject.h"

// 열거형 상수 정의 TEXTURE는 빠질 수도 있음
// 열혈책 보고 4가지 스타일 열거 
// enum BrushType { SOLID, HATCH, GRADATION, TEXTURE};

class GEllipse : public GObject {
protected:
	DECLARE_SERIAL(GEllipse);
public:
	// 생성자 및 소멸자
	GEllipse(void);							//생성자
	GEllipse(const GEllipse& pGEllipse);	//복사 생성자
	~GEllipse(void);						//소멸자

	virtual void Serialize(CArchive& ar);	//직렬화

	// Accessor 함수
	CRgn* GetRegion();										// 리전을 반환
	int GetPenStyle() const {return m_nsPenStyle; }	//펜 스타일 반환
	int GetBrushStyle(void) const { return m_esBrushStyle; } //채우기 타입 반환
	int GetFaceType(void) const { return m_esFaceType; } //면 타입 반환
	COLORREF GetFillColor(void) const { return m_pBrushColor; } //채우기색 반환
	COLORREF GetGradientColor(void) const { return m_sGrdEndColor; } //그라디언트 끝 색 반환
	int GetLineJoin(void) const { return m_nsLineJoin; }	//외곽선 처리 반환
	int GetHatchStyle(void) const {return m_nsHatchStyle; } //해치 스타일 반환
	int GetGradientMode(void) const { return m_npGrdMode; } //그라디언트 모드 반환

	// Mutator 함수
	void Move(int dX, int dY);								// 좌표 이동

	void SetPenStyle(int penStyle);							// 펜 스타일 설정
	void SetBrushColor(COLORREF brushColor);				// 브러쉬 칼라 설정

	void SetFaceType(int faceType);							// 면 타입 설정
	void SetBrushStyle(int brushStyle);						// 브러쉬 스타일 설정
	void SetHatchStyle(int hatch);							// 해치 스타일 설정
	void SetLineJoinStyle(int lineJoin);					// 라인 꺾임 처리 설정
	void SetGradientMode(int grdMode);						// 그라디언트 모드 설정
	void SetTransparent(BOOL transparent);					// 투명 채우기 여부 설정
	void SetGrdEndColor(COLORREF grdEndColor);				// 그라데이션의 끝 색 설정

	// 크기 바꾸는 함수
	void Resize(int index, int dX, int dY);
	int FindSelectPoint(CPoint pt);

	// 그리는 함수
	void Draw(CDC* cdc);									//그리기 함수
	virtual void DrawSelectLine(CDC *pDC);					//선택 시 점선으로 영역을 그림

private :
	int m_nsPenStyle;						// 펜 스타일
	COLORREF m_pBrushColor;					// 브러쉬 칼라

	//for GDI+, 120528 추가 by 신희철
	GFaceType m_esFaceType;		//면 타입
	GBrushType m_esBrushStyle;	//브러쉬 타입
	int m_nsHatchStyle;			//해치 스타일=>타입을 int로 변경
	COLORREF m_sGrdEndColor;	//그라디언트 끝 컬러(컬러버튼)
	int m_nsLineJoin;			//라인 모양(콤보박스-드롭다운)

	int m_npGrdMode;			// 그라디언트 모드 by 박현덕
};

