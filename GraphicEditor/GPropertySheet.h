#pragma once
#include "PropertyPage1.h"

// GPropertySheet

class GPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(GPropertySheet)

public:
	GPropertySheet(CMFCPropertySheet::PropSheetLook look,
		UINT uiIconsResID = 0, int cxIcon = 0);
	virtual ~GPropertySheet();

public : 
	PropertyPage1 m_Page1;
	PropertyPage2 m_Page2;
	PropertyPage3 m_Page3;

// Operations
public:

// Overrides
	virtual void OnDrawPageHeader (CDC* pDC, int nPage, CRect rectHeader);

protected:
	DECLARE_MESSAGE_MAP()
};