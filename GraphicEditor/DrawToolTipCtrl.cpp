/* [ CDrawToolTipCtrl 클래스 by 신희철 ]
** CDrawToolTipCtrl 클래스의 구현부 입니다.
*/

// DrawToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "DrawToolTipCtrl.h"


// CDrawToolTipCtrl

IMPLEMENT_DYNAMIC(CDrawToolTipCtrl, CMFCToolTipCtrl)

//--------------------------------------------------------------------------
//　생성자
//--------------------------------------------------------------------------
CDrawToolTipCtrl::CDrawToolTipCtrl()
{
	m_nsCurrID = 0;
}

//--------------------------------------------------------------------------
//　메시지맵 선언
//--------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawToolTipCtrl, CMFCToolTipCtrl)
	ON_NOTIFY_REFLECT(TTN_SHOW, &CDrawToolTipCtrl::OnTtnTooltipShow)
END_MESSAGE_MAP()



// CDrawToolTipCtrl message handlers
//--------------------------------------------------------------------------
//　툴팁이 보여질 경우
//--------------------------------------------------------------------------
void CDrawToolTipCtrl::OnTtnTooltipShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nsCurrID = CWnd::FromHandle ((HWND)pNMHDR->idFrom)->GetDlgCtrlID ();
	//각 버튼 ID마다 Description이 달라짐
	switch (m_nsCurrID)
	{
	case IDC_BUTTON_FONT:
		SetDescription (L"폰트 크기와 글꼴을 설정합니다.");
		break;

	case IDC_BUTTON_LINECOLOR:
		SetDescription (L"라인색 또는 텍스트색을 설정합니다.");
		break;

	case IDC_BUTTON_FILLCOLOR:
		SetDescription (L"채우기색을 설정합니다.");
		break;

	case IDC_BUTTON_FILLCOLOR2:
		SetDescription(L"그라디언트 채우기색을 설정합니다.");
		break;

	case IDC_FACE_OUTLINED:
	case IDC_OUTLINEONLY :
		SetDescription(L"도형을 채우지 않고 외곽선만 그립니다.");
		break;

	case IDC_FACE_FILL:
	case IDC_FILLOUTLINE :
		SetDescription(L"외곽선을 그리고 도형 내부를 칠합니다.");
		break;

	case IDC_FACE_ONLYFILL:
	case IDC_FillOnly :
		SetDescription(L"외곽선을 그리지 않고 도형 내부를 칠합니다.");
		break;

	case IDC_FILLTYPE_SOLID:
		SetDescription(L"단색으로 도형을 채웁니다");
		break;

	case IDC_FILLTYPE_HATCH:
		SetDescription(L"여러가지 격자무늬로 도형을 채웁니다.");
		break;

	case IDC_FILLTYPE_GRADIENT:
		SetDescription(L"그라디언트 효과를 주어 도형을 채웁니다.");
		break;

	case IDC_COMBO_STARTCAP:
	case IDC_STATIC_TEXT7:
		SetDescription(L"라인과 폴리라인의 시작모양을 바꿉니다.");
		break;

	case IDC_COMBO_ENDCAP:
	case IDC_STATIC_TEXT6:
		SetDescription(L"라인과 폴리라인의 끝 모양을 바꿉니다.");
		break;

	case IDC_EDIT_LINEWEIGHT:
	case IDC_STATIC_TEXT2:
	case IDC_SPIN1:
	case IDC_EDIT_WIDTH :
		SetDescription(L"라인 굵기를 설정합니다.");
		break;

	case IDC_LINEPATTERN:
	case IDC_STATIC_TEXT3:
	case IDC_COMBO_DASHSTYLE :
		SetDescription(L"라인 패턴을 설정합니다.");
		break;

	case IDC_COMBO_LINEJOIN:
	case IDC_STATIC_TEXT9:
		SetDescription(L"도형에서 선이 만나는 부분의 모양을 설정합니다.");
		break;

	case IDC_FILLPATTERN:
	case IDC_STATIC_TEXT4:
		SetDescription(L"도형을 채울 격자무늬의 패턴을 설정합니다.");
		break;

	case IDC_EDIT_ALPHA:
	case IDC_SPIN2:
	case IDC_STATIC_TEXT5:
	case IDC_EDIT_FILLALPHAEDIT :
		SetDescription(L"그래피컬 개체들의 불투명도를 설정합니다.");
		break;

	case IDC_COMBO_GRADIENT:
	case IDC_STATIC_TEXT8:
	case IDC_COMBO_GRADIENTPATTERN :
		SetDescription(L"도형을 채울 그라디언트 패턴을 설정합니다.");
		break;
	default:
		SetDescription (_T (""));
	}

	CMFCToolTipCtrl::OnShow (pNMHDR, pResult);
}
