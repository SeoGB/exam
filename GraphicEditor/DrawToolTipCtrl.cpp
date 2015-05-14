/* [ CDrawToolTipCtrl Ŭ���� by ����ö ]
** CDrawToolTipCtrl Ŭ������ ������ �Դϴ�.
*/

// DrawToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "DrawToolTipCtrl.h"


// CDrawToolTipCtrl

IMPLEMENT_DYNAMIC(CDrawToolTipCtrl, CMFCToolTipCtrl)

//--------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------
CDrawToolTipCtrl::CDrawToolTipCtrl()
{
	m_nsCurrID = 0;
}

//--------------------------------------------------------------------------
//���޽����� ����
//--------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawToolTipCtrl, CMFCToolTipCtrl)
	ON_NOTIFY_REFLECT(TTN_SHOW, &CDrawToolTipCtrl::OnTtnTooltipShow)
END_MESSAGE_MAP()



// CDrawToolTipCtrl message handlers
//--------------------------------------------------------------------------
//�������� ������ ���
//--------------------------------------------------------------------------
void CDrawToolTipCtrl::OnTtnTooltipShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nsCurrID = CWnd::FromHandle ((HWND)pNMHDR->idFrom)->GetDlgCtrlID ();
	//�� ��ư ID���� Description�� �޶���
	switch (m_nsCurrID)
	{
	case IDC_BUTTON_FONT:
		SetDescription (L"��Ʈ ũ��� �۲��� �����մϴ�.");
		break;

	case IDC_BUTTON_LINECOLOR:
		SetDescription (L"���λ� �Ǵ� �ؽ�Ʈ���� �����մϴ�.");
		break;

	case IDC_BUTTON_FILLCOLOR:
		SetDescription (L"ä������ �����մϴ�.");
		break;

	case IDC_BUTTON_FILLCOLOR2:
		SetDescription(L"�׶���Ʈ ä������ �����մϴ�.");
		break;

	case IDC_FACE_OUTLINED:
	case IDC_OUTLINEONLY :
		SetDescription(L"������ ä���� �ʰ� �ܰ����� �׸��ϴ�.");
		break;

	case IDC_FACE_FILL:
	case IDC_FILLOUTLINE :
		SetDescription(L"�ܰ����� �׸��� ���� ���θ� ĥ�մϴ�.");
		break;

	case IDC_FACE_ONLYFILL:
	case IDC_FillOnly :
		SetDescription(L"�ܰ����� �׸��� �ʰ� ���� ���θ� ĥ�մϴ�.");
		break;

	case IDC_FILLTYPE_SOLID:
		SetDescription(L"�ܻ����� ������ ä��ϴ�");
		break;

	case IDC_FILLTYPE_HATCH:
		SetDescription(L"�������� ���ڹ��̷� ������ ä��ϴ�.");
		break;

	case IDC_FILLTYPE_GRADIENT:
		SetDescription(L"�׶���Ʈ ȿ���� �־� ������ ä��ϴ�.");
		break;

	case IDC_COMBO_STARTCAP:
	case IDC_STATIC_TEXT7:
		SetDescription(L"���ΰ� ���������� ���۸���� �ٲߴϴ�.");
		break;

	case IDC_COMBO_ENDCAP:
	case IDC_STATIC_TEXT6:
		SetDescription(L"���ΰ� ���������� �� ����� �ٲߴϴ�.");
		break;

	case IDC_EDIT_LINEWEIGHT:
	case IDC_STATIC_TEXT2:
	case IDC_SPIN1:
	case IDC_EDIT_WIDTH :
		SetDescription(L"���� ���⸦ �����մϴ�.");
		break;

	case IDC_LINEPATTERN:
	case IDC_STATIC_TEXT3:
	case IDC_COMBO_DASHSTYLE :
		SetDescription(L"���� ������ �����մϴ�.");
		break;

	case IDC_COMBO_LINEJOIN:
	case IDC_STATIC_TEXT9:
		SetDescription(L"�������� ���� ������ �κ��� ����� �����մϴ�.");
		break;

	case IDC_FILLPATTERN:
	case IDC_STATIC_TEXT4:
		SetDescription(L"������ ä�� ���ڹ����� ������ �����մϴ�.");
		break;

	case IDC_EDIT_ALPHA:
	case IDC_SPIN2:
	case IDC_STATIC_TEXT5:
	case IDC_EDIT_FILLALPHAEDIT :
		SetDescription(L"�׷����� ��ü���� �������� �����մϴ�.");
		break;

	case IDC_COMBO_GRADIENT:
	case IDC_STATIC_TEXT8:
	case IDC_COMBO_GRADIENTPATTERN :
		SetDescription(L"������ ä�� �׶���Ʈ ������ �����մϴ�.");
		break;
	default:
		SetDescription (_T (""));
	}

	CMFCToolTipCtrl::OnShow (pNMHDR, pResult);
}
