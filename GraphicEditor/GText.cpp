/* [ GText 클래스 by 신희철, 박현덕 ]
** GText 클래스의 구현부입니다.
*/

#include "StdAfx.h"
#include "GText.h"


IMPLEMENT_SERIAL(GText, GObject, 0)
//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GText::GText(void)
{
	m_sType = TEXT; //그래피컬 개체 타입은 TEXT
	::ZeroMemory(&m_sLogFont, sizeof(LOGFONT)); //로그폰트 초기화
}

//복사 생성자
GText::GText(const GText& pGText)
{
	this->GObject::m_sType = pGText.GObject::m_sType;
	this->GObject::m_sStartPoint = pGText.GObject::m_sStartPoint;
	this->GObject::m_sLineColor = pGText.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pGText.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pGText.GObject::m_bsGrouped;

	this->m_sBgColor = pGText.m_sBgColor;
	this->m_sEndPoint = pGText.m_sEndPoint;
	this->m_sLogFont = pGText.m_sLogFont;
	for(int i=0;this-m_sArrayString.GetSize();i++)
	{
		this->m_sArrayString.Add(m_sArrayString.GetAt(i));
	}
}

GText::~GText(void)
{}

//--------------------------------------------------------------------------
//　직렬화
//--------------------------------------------------------------------------
void GText::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //저장 시
	{
		ar << (WORD)m_bsTransparent;
		m_sArrayString.Serialize(ar);
		ar << m_sBgColor;
		ar.Write(&m_sLogFont, sizeof(LOGFONT));
	}
	else //로드 시
	{
		WORD wTemp;
		ar >> wTemp; m_bsTransparent = wTemp;
		m_sArrayString.Serialize(ar);
		ar >> m_sBgColor;
		ar.Read(&m_sLogFont, sizeof(LOGFONT));
	}
}

//--------------------------------------------------------------------------
//　글자의 영역을 구하는 함수
//--------------------------------------------------------------------------
CRgn* GText::GetRegion()
{
	m_sRgn.DeleteObject();
	m_sRgn.CreateRectRgn(m_sStartPoint.x-5, m_sStartPoint.y-5, m_sEndPoint.x+5, m_sEndPoint.y+5);
	return &m_sRgn;
}

//--------------------------------------------------------------------------
//　이동 함수
//--------------------------------------------------------------------------
void GText::Move(int dX, int dY)
{
	m_sStartPoint.x += dX;
	m_sStartPoint.y += dY;
	m_sEndPoint.x += dX;
	m_sEndPoint.y += dY;
}

//--------------------------------------------------------------------------
//　글꼴(폰트, 크기) 설정 함수
//--------------------------------------------------------------------------
void GText::SetLogFont(LOGFONT lf)
{
	m_sLogFont = lf; //글꼴(폰트, 크기) 지정(CFontDialog에서 가져올 예정)
}

//--------------------------------------------------------------------------
//　배경색 설정 함수
//--------------------------------------------------------------------------
void GText::SetBgColor(COLORREF color)
{
	m_sBgColor = color; //배경색 설정
}

//--------------------------------------------------------------------------
//　투명 배경 여부 설정 함수
//--------------------------------------------------------------------------
void GText::SetBgTransparent(bool bTransparent)
{
	m_bsTransparent = bTransparent; //투명배경 여부 지정
}
//--------------------------------------------------------------------------
//　글자 추가 함수
//--------------------------------------------------------------------------
void GText::AddChar(UINT nChar)
{
	m_sArrayString.Add(nChar); //글자 추가
}

//--------------------------------------------------------------------------
//　글자 지우기 함수
//--------------------------------------------------------------------------
void GText::DelChar(void)
{
	if(m_sArrayString.GetSize() > 0) //글자가 입력되어있는 경우
		m_sArrayString.RemoveAt(m_sArrayString.GetSize() - 1); //마지막 글자 제거
}

//--------------------------------------------------------------------------
//　텍스트 내용 반환 함수 by 박현덕
//--------------------------------------------------------------------------
TCHAR *GText::GetString() {
	return this->m_sArrayString.GetData();
}
//--------------------------------------------------------------------------
//　로그 폰트 반환 함수 by 박현덕
//--------------------------------------------------------------------------
LOGFONT GText::GetLogfont() {
	return this->m_sLogFont;
}
//--------------------------------------------------------------------------
//　글자 수 반환 함수 by 박현덕
//--------------------------------------------------------------------------
int GText::GetStringCount() {
	return this->m_sArrayString.GetSize();
}
//--------------------------------------------------------------------------
// 그리기 함수
//--------------------------------------------------------------------------
void GText::Draw(CDC* pDC)
{
	CFont font;
	font.CreateFontIndirect(&m_sLogFont); //폰트 생성
	CFont *oldFont = pDC->SelectObject(&font); //폰트 지정
	
	COLORREF oldTextColor, oldBkColor;
	oldTextColor = pDC->SetTextColor(m_sLineColor); //글자 색 지정
	oldBkColor = pDC->SetBkColor(m_sBgColor); //배경 색 지정

	int oldBkMode;
	if(m_bsTransparent == TRUE) //배경 투명일 경우
		oldBkMode = pDC->SetBkMode(TRANSPARENT); //투명 배경
	else //투명이 아닐 경우
		oldBkMode = pDC->SetBkMode(OPAQUE); //지정한 배경색의 배경

	CSize textSize = pDC->GetTextExtent(m_sArrayString.GetData(), m_sArrayString.GetSize()); //텍스트가 그려질 영역의 크기를 구함
	
	//글자 끝 위치 지정
	m_sEndPoint.x = m_sStartPoint.x + textSize.cx;
	m_sEndPoint.y = m_sStartPoint.y + textSize.cy;

	CRect textRect(m_sStartPoint.x, m_sStartPoint.y, m_sEndPoint.x, m_sEndPoint.y); //텍스트가 그려질 rect
	pDC->DrawText(m_sArrayString.GetData(), m_sArrayString.GetSize(), &textRect, DT_LEFT | DT_SINGLELINE); //글자 쓰기
	
	//이전 설정으로 되돌리기
	pDC->SetTextColor(oldTextColor); //이전 텍스트 컬러로 되돌리기
	pDC->SetBkColor(oldBkColor); //이전 백그라운드 컬러로 되돌리기
	pDC->SetBkMode(oldBkMode); //이전 백그라운드 모드로 되돌리기
	pDC->SelectObject(oldFont); //옛날 폰트로 되돌리기
}

//--------------------------------------------------------------------------
//　선택 영역 점선 그리기 함수
//--------------------------------------------------------------------------
void GText::DrawSelectLine(CDC *pDC)
{
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //리전으로 사각형을 만듬
	pDC->Rectangle(rect); //사각형 점선을 그림

	//이전 펜과 브러쉬 속성으로 되돌림
	pDC->SelectObject(oldPen);
}

