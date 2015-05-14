#pragma once

/////////////////////////////////////////////////////////////////////////////
// PropertyPage1 dialog

class PropertyPage1 : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage1)

public:
	PropertyPage1();
	virtual ~PropertyPage1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};