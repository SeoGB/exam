/* [ LineThickness Ŭ���� by ������ ]
** LineThickness�� �����ϱ� ���� ��ȭ���� ���
** LineThickness Ŭ�����Դϴ�.
*/
#pragma once

// LineThickness ��ȭ �����Դϴ�.

class LineThickness : public CDialog
{
	DECLARE_DYNAMIC(LineThickness)

public:
	LineThickness(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~LineThickness();

	int GetLineThickness();					// ���� �β��� ����

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LINETHICKNESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_npLineThickness;						// ���εβ�
public :
	afx_msg void OnBnClickedOk();				// OK ��ư �̺�Ʈó�� �Լ�
};