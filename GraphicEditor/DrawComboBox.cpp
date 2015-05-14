/* [ CDrawComboBox Ŭ���� by ����ö ]
** CDrawComboBox Ŭ������ ������ �Դϴ�.
*/

// DrawComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "DrawComboBox.h"


// CDrawComboBox

IMPLEMENT_DYNAMIC(CDrawComboBox, CComboBox)
//--------------------------------------------------------------------------
//��CDrawComboBox ������ �� �Ҹ���
//--------------------------------------------------------------------------
CDrawComboBox::CDrawComboBox()
{
	m_sHighlight = RGB(94,129,188); //���ý� ��
}

CDrawComboBox::~CDrawComboBox()
{
}

//--------------------------------------------------------------------------
//���޽�����
//--------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawComboBox, CComboBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


//--------------------------------------------------------------------------
//�������� �߰� �Լ�
//--------------------------------------------------------------------------
void CDrawComboBox::AddItem(int nIndex, ComboBoxDrawType esType)
{
	m_psDrawItems = new DrawItems; //���ο� DrawItem ��ü ����

	m_psDrawItems->m_nsIndex = nIndex;
	m_psDrawItems->m_esType = esType;
	int index = AddString(L"");
	SetItemDataPtr(index, m_psDrawItems); //�޺��ڽ��� ������ �߰�
}

// CDrawComboBox message handlers
//--------------------------------------------------------------------------
//���޺��ڽ� �ı� �� ȣ��
//--------------------------------------------------------------------------
void CDrawComboBox::OnDestroy()
{
	CComboBox::OnDestroy();

	ResetContent(); //�޺��ڽ� ���� ������ ����
}

//--------------------------------------------------------------------------
//�������� �׸��� �Լ�
//--------------------------------------------------------------------------
void CDrawComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->itemID == -1) 
		return;

	//����̽� ���ؽ�Ʈ ����
	HDC hdc = lpDrawItemStruct->hDC;
	Graphics graphics(hdc);

	//�׷��� �κ��� ������ �޾ƿ�
	CRect rect = lpDrawItemStruct->rcItem;
	int itemState = lpDrawItemStruct->itemState;

	//�÷� ����
	Color bgColor(Color::White); //�Ϲ� ��� �� �÷�
	if(itemState & ODS_SELECTED) //���� �� �÷�
		bgColor.SetFromCOLORREF(m_sHighlight);
	else if(itemState & ODS_DISABLED) //��Ȱ��ȭ �� �÷�
		bgColor.SetFromCOLORREF(::GetSysColor(COLOR_BTNFACE));

	//�޺��ڽ� ��Ʈ���� �簢���� �׸�
	SolidBrush brush(bgColor);
	graphics.FillRectangle(&brush, rect.left, rect.top, rect.Width(), rect.Height());
	
	//�޺��ڽ��� �� ������ ������ ������
	DrawItems* pDrawItem = (DrawItems*)GetItemDataPtr(lpDrawItemStruct->itemID);

	//�׸��� ���� GDI+ ��ü�� ����
	Pen pen(Color::Black, 2.0f);	//��
	Pen pen1(Color::Black, 1.0f);	//�׶���Ʈ ���� �ܰ��� ��
	HatchBrush hatchBrush((HatchStyle)pDrawItem->m_nsIndex, Color::Black, Color::Transparent);	//��ġ �귯��
	LinearGradientBrush gradientBrush(Rect(rect.left+2, rect.top+2,rect.Width()-5, rect.Height()-5), //�׶���Ʈ �귯��
		Color::Black, Color::White, (LinearGradientMode)pDrawItem->m_nsIndex);
	
	//���� ������ ��� �� ���� ����
	if(itemState & ODS_SELECTED)
		pen.SetColor(Color::White);

	//�޺��ڽ��� �׷��� ������ Ÿ�Կ� ���� �׷����� ������ �޶���
	switch(pDrawItem->m_esType)
	{
	case LINEPATTERN:	//���� ����
		pen.SetDashStyle((DashStyle)pDrawItem->m_nsIndex);
		graphics.DrawLine(&pen, Point(rect.left+5,rect.CenterPoint().y), Point(rect.left+rect.Width()-5, rect.CenterPoint().y));
		break;
	case HATCHPATTERN:	//���ڹ��� ����
		if(itemState & ODS_SELECTED)
		{
			HatchBrush hatchBrush((HatchStyle)pDrawItem->m_nsIndex, Color::White, Color::Transparent);
			graphics.FillRectangle(&hatchBrush, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		}
		else
			graphics.FillRectangle(&hatchBrush, rect.left+2, rect.top+2, rect.Width()-5, rect.Height()-5);
		break;
	case GRADIENTPATTERN: //�׶���Ʈ ����
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
