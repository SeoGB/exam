#pragma once

/////////////////////////////////////////////////////////////////////////////
// PropertyPage1 dialog

class PropertyPage1 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage1)

public:
	PropertyPage1();
	virtual ~PropertyPage1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROPPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// PropertyPage2 dialog

class PropertyPage2 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage2)

public:
	PropertyPage2();
	virtual ~PropertyPage2();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROPPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// PropertyPage3 dialog
class PropertyPage3 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage3)

public:
	PropertyPage3();
	virtual ~PropertyPage3();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROPPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};