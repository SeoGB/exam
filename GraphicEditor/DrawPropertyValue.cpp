#include "StdAfx.h"
#include "DrawPropertyValue.h"

//각 멤버 초기화
LOGFONT	CDrawPropertyValue::	m_sLogFont;							//폰트 정보
int CDrawPropertyValue::		m_nsFontSize = 10;					//폰트 크기

COLORREF CDrawPropertyValue::	m_sLineColor = RGB(0,0,0);			//라인 컬러
int CDrawPropertyValue::		m_nsThickness = 1;					//선 두께
int CDrawPropertyValue::		m_nsPenStyle = PS_SOLID;			//펜 스타일
int CDrawPropertyValue::		m_nsStartCap = 2;					//선 시작 모양
int CDrawPropertyValue::		m_nsEndCap = 2;						//선 끝 모양

COLORREF CDrawPropertyValue::	m_sBrushColor = RGB(255,255,255);	//브러쉬 컬러(채우기 색)
int CDrawPropertyValue::		m_nsBrushStyle = 0;					//브러쉬 스타일
int CDrawPropertyValue::		m_nsHatchStyle = 0;					//해치 스타일
int CDrawPropertyValue::		m_nsFaceType = 1;					//면 타입
int CDrawPropertyValue::		m_nsGradientType = 0;				//그라디언트 타입
COLORREF CDrawPropertyValue::	m_sGradientEndColor = RGB(0,0,0);	//그라디언트 끝컬러(채우기 색2)
int CDrawPropertyValue::		m_nsLineJoin = 0;

int CDrawPropertyValue::		m_nsAlpha = 100;					//불투명도

//로그 폰트 초기화 함수
void CDrawPropertyValue::InitializeLogFont(void)
{
	HDC hdc = ::GetDC(::GetDesktopWindow());
	::ZeroMemory(&m_sLogFont, sizeof(LOGFONT)); //로그폰트 초기화
	m_sLogFont.lfHeight = -MulDiv(10, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
	_tcscpy_s(m_sLogFont.lfFaceName, LF_FACESIZE, _T("굴림"));
	::ReleaseDC(::GetDesktopWindow(), hdc);
}
