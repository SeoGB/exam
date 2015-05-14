/* [ DrawPropertyValue 클래스 by 신희철 ]
** 해당 클래스에서는 그리기 속성에 대한 정보만 가지고 있습니다.
** 그리기 속성 다이얼로그의 각 컨트롤에 대한 value 변수들을 따로 만들어 두어,
** 그리기 속성 다이얼로그를 include 하지 않고도 이 클래스만 include하면,
** 그리기 속성 다이얼로그에서 변경한 정보를 가져다 쓸 수 있습니다.
** (개발 시 유연성 확대를 위함)
*/

#pragma once
class CDrawPropertyValue
{
public:
	static LOGFONT m_sLogFont;		//폰트 정보
	static int m_nsFontSize;		//폰트 크기
	
	static COLORREF m_sLineColor;	//라인 컬러
	static int m_nsThickness;		//선 두께
	static int m_nsPenStyle;		//펜 스타일
	static int m_nsStartCap;		//선 시작 모양
	static int m_nsEndCap;			//선 끝 모양

	static COLORREF m_sBrushColor;	//브러쉬 컬러(채우기 색)
	static int m_nsBrushStyle;		//브러쉬 스타일(솔리드, 해치, 그라디언트)
	static int m_nsHatchStyle;		//해치 스타일
	static int m_nsFaceType;		//면 타입
	static int m_nsGradientType;	//그라디언트 타입
	static COLORREF m_sGradientEndColor; //그라디언트 끝 컬러
	static int m_nsLineJoin;		//외곽선 모양

	static int m_nsAlpha;			//불투명도

	static void InitializeLogFont(void); //로그 폰트 초기화
};

