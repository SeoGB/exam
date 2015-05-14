/* [ GText 클래스 by 신희철, 박현덕 ]
** GText 클래스는 텍스트 개체를 관리하는 클래스입니다.
*/

#pragma once
#include "gobject.h"
class GText : public GObject
{
protected:
	DECLARE_SERIAL(GText);
public:
	//생성자 및 소멸자
	GText(void);				//생성자
	GText(const GText& pGText);	//복사 생성자
	~GText(void);				//소멸자

	virtual void Serialize(CArchive& ar);

	//Accessor 함수
	void SetLogFont(LOGFONT lf);							//글꼴 설정(폰트, 크기)
	void SetBgColor(COLORREF color);						//배경색 설정
	void SetBgTransparent(bool bTransparent);				//투명 배경 여부 설정
	void AddChar(UINT nChar);								//글자 추가
	void DelChar(void);										//한 글자 제거

	virtual CRgn* GetRegion();								//리전을 구함
	virtual int FindSelectPoint(CPoint pt) { return -1; }	//선택한 점을 찾는 함수
	virtual void Resize(int index, int dX, int dY) { }		//크기 변경
	virtual void Move(int dX, int dY);						//좌표 이동

	virtual void Draw(CDC* cdc);							//이 함수에서 그래피컬 개체 그림
	virtual void DrawSelectLine(CDC *pDC);					//선택 시 점선으로 영역을 그림

	int GetStringCount();	//글자 수 반환 by 박현덕
	TCHAR *GetString();		//입력된 텍스트 반환 by 박현덕
	LOGFONT GetLogfont();	//로그 폰트 반환 by 박현덕

protected:
	CArray <TCHAR, TCHAR> m_sArrayString;	//글자 저장 배열
	COLORREF m_sBgColor;					//배경색
	LOGFONT m_sLogFont;						//글꼴(폰트, 크기)
	BOOL m_bsTransparent;					//투명 배경 여부
};