// GPropertySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "GPropertySheet.h"


// GPropertySheet

IMPLEMENT_DYNAMIC(GPropertySheet, CPropertySheet)

GPropertySheet::GPropertySheet(CMFCPropertySheet::PropSheetLook look, UINT uiIconsResID, int cxIcon)
//	: CMFCPropertySheet(IDS_PROPSHT_CAPTION, NULL)
{
	SetLook (look);

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

GPropertySheet::~GPropertySheet()
{
}


BEGIN_MESSAGE_MAP(GPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// GPropertySheet 메시지 처리기입니다.
void GPropertySheet::OnDrawPageHeader (CDC* pDC, int nPage, CRect rectHeader)
{
	rectHeader.top += 2;
	rectHeader.right -= 2;
	rectHeader.bottom -= 2;

	pDC->FillRect (rectHeader, &afxGlobalData.brBtnFace);
	pDC->Draw3dRect (rectHeader, afxGlobalData.clrBtnShadow, afxGlobalData.clrBtnShadow);

	CDrawingManager dm (*pDC);
	dm.DrawShadow (rectHeader, 2);	

	CString strText;
	strText.Format (_T("Page %d description..."), nPage + 1);

	CRect rectText = rectHeader;
	rectText.DeflateRect (10, 0);

	CFont* pOldFont = pDC->SelectObject (&afxGlobalData.fontBold);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (afxGlobalData.clrBtnText);

	pDC->DrawText (strText, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
}