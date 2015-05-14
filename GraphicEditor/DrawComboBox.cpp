/* [ CDrawComboBox 클래스 by 신희철 ]
** CDrawComboBox 클래스의 구현부 입니다.
*/

// DrawComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "DrawComboBox.h"


// CDrawComboBox

IMPLEMENT_DYNAMIC(CDrawComboBox, CComboBox)
//--------------------------------------------------------------------------
//　CDrawComboBox 생성자 및 소멸자
//--------------------------------------------------------------------------
CDrawComboBox::CDrawComboBox()
{
	m_sHighlight = RGB(94,129,188); //선택시 색
}

CDrawComboBox::~CDrawComboBox()
{
}

//--------------------------------------------------------------------------
//　메시지맵
//--------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawComboBox, CComboBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


//--------------------------------------------------------------------------
//　아이템 추가 함수
//--------------------------------------------------------------------------
void CDrawComboBox::AddItem(int nIndex, ComboBoxDrawType esType)
{
	m_psDrawItems = new DrawItems; //새로운 DrawItem 객체 생성

	m_psDrawItems->m_nsIndex = nIndex;
	m_psDrawItems->m_esType = esType;
	int index = AddString(L"");
	SetItemDataPtr(index, m_psDrawItems); //콤보박스에 아이템 추가
}

// CDrawComboBox message handlers
//--------------------------------------------------------------------------
//　콤보박스 파괴 시 호출
//--------------------------------------------------------------------------
void CDrawComboBox::OnDestroy()
{
	CComboBox::OnDestroy();

	ResetContent(); //콤보박스 내의 내용을 리셋
}

//--------------------------------------------------------------------------
//　아이템 그리기 함수
//--------------------------------------------------------------------------
void CDrawComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->itemID == -1) 
		return;

	//디바이스 컨텍스트 생성
	HDC hdc = lpDrawItemStruct->hDC;
	Graphics graphics(hdc);

	//그려질 부분의 정보를 받아옴
	CRect rect = lpDrawItemStruct->rcItem;
	int itemState = lpDrawItemStruct->itemState;

	//컬러 지정
	Color bgColor(Color::White); //일반 출력 시 컬러
	if(itemState & ODS_SELECTED) //선택 시 컬러
		bgColor.SetFromCOLORREF(m_sHighlight);
	else if(itemState & ODS_DISABLED) //비활성화 시 컬러
		bgColor.SetFromCOLORREF(::GetSysColor(COLOR_BTNFACE));

	//콤보박스 엔트리에 사각형을 그림
	SolidBrush brush(bgColor);
	graphics.FillRectangle(&brush, rect.left, rect.top, rect.Width(), rect.Height());
	
	//콤보박스에 들어갈 내용의 정보를 가져옴
	DrawItems* pDrawItem = (DrawItems*)GetItemDataPtr(lpDrawItemStruct->itemID);

	//그리기 위한 GDI+ 객체들 선언
	Pen pen(Color::Black, 2.0f);	//펜
	Pen pen1(Color::Black, 1.0f);	//그라디언트 패턴 외곽선 용
	HatchBrush hatchBrush((HatchStyle)pDrawItem->m_nsIndex, Color::Black, Color::Transparent);	//해치 브러쉬
	LinearGradientBrush gradientBrush(Rect(rect.left+2, rect.top+2,rect.Width()-5, rect.Height()-5), //그라디언트 브러쉬
		Color::Black, Color::White, (LinearGradientMode)pDrawItem->m_nsIndex);
	
	//선택 상태일 경우 펜 색을 반전
	if(itemState & ODS_SELECTED)
		pen.SetColor(Color::White);

	//콤보박스에 그려질 내용의 타입에 따라 그려지는 내용이 달라짐
	switch(pDrawItem->m_esType)
	{
	case LINEPATTERN:	//라인 패턴
		pen.SetDashStyle((DashStyle)pDrawItem->m_nsIndex);
		graphics.DrawLine(&pen, Point(rect.left+5,rect.CenterPoint().y), Point(rect.left+rect.Width()-5, rect.CenterPoint().y));
		break;
	case HATCHPATTERN:	//격자무늬 패턴
		if(itemState & ODS_SELECTED)
		{
			HatchBrush hatchBrush((HatchStyle)pDrawItem->m_nsIndex, Color::White, Color::Transparent);
			graphics.FillRectangle(&hatchBrush, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		}
		else
			graphics.FillRectangle(&hatchBrush, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		break;
	case GRADIENTPATTERN: //그라디언트 패턴
		graphics.FillRectangle(&gradientBrush, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		graphics.DrawRectangle(&pen1, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		break;
	}
}

void CDrawComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  if(lpMeasureItemStruct->itemID == -1) 
	  return;
}
