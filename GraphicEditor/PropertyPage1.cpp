// PropertyPage1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "PropertyPage1.h"
#include "afxdialogex.h"


// PropertyPage1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(PropertyPage1, CMFCPropertyPage)
IMPLEMENT_DYNAMIC(PropertyPage2, CMFCPropertyPage)
IMPLEMENT_DYNAMIC(PropertyPage3, CMFCPropertyPage)

PropertyPage1::PropertyPage1()
	: CMFCPropertyPage(PropertyPage1::IDD)
{

}

PropertyPage1::~PropertyPage1()
{
}

void PropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PropertyPage1, CMFCPropertyPage)
END_MESSAGE_MAP()


// PropertyPage1 �޽��� ó�����Դϴ�.

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 property page

PropertyPage2::PropertyPage2() : CMFCPropertyPage(PropertyPage2::IDD)
{
}

PropertyPage2::~PropertyPage2()
{
}

void PropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PropertyPage2, CMFCPropertyPage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage3 property page

PropertyPage3::PropertyPage3() : CMFCPropertyPage(PropertyPage3::IDD)
{
}

PropertyPage3::~PropertyPage3()
{
}

void PropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PropertyPage3, CMFCPropertyPage)
END_MESSAGE_MAP()