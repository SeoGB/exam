/* [ LinePattern Ŭ���� by ������ ]
** LinePattern�� �����ϱ� ���� ��ȭ���� ���
** LinePattern Ŭ�����Դϴ�.
*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DrawComboBox.h"
#include "DrawToolTipCtrl.h"

// LinePattern ��ȭ �����Դϴ�.

class LinePattern : public CDialog
{
	DECLARE_DYNAMIC(LinePattern)

public:
	LinePattern(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~LinePattern();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LINEPATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public :
	CEdit m_pLineWidthEdit;					// �β�
	CSpinButtonCtrl m_pLineWidthSpin;		// �β� ���� ��Ʈ�� ����

	CDrawComboBox m_pLinePtnCmb;			// ���� ���� ��Ʈ�� ����

	CButton m_pLineColorBtn;				// ���� �� ��ư ��Ʈ�� ����

	CDrawComboBox m_pStartCapCombo;			// ���� ���� ��� ��Ʈ�� ����
	CDrawComboBox m_pEndCapCombo;			// ���� �� ��� ��Ʈ�� ����

	CEdit m_pAlphaEdit;						// ����
	CSpinButtonCtrl m_pAlphaSpin;			// ���� ���� ��Ʈ�� ����

	// ����
	CDrawToolTipCtrl m_pCtrlToolTip;
public :
	afx_msg void OnBnClickedButtonLinecolor();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnCbnSelchangeComboDashstyle();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCbnSelchangeComboStartcap();
	afx_msg void OnCbnSelchangeComboEndcap();
	afx_msg void OnBnClickedClose();

	void ChangeIndex(int *startIndex, int *endIndex);		// ������ ���� ���� �� ��� ������
															// �ش�Ǵ� �ε����� �������� �ƴϱ⿡
															// �������־�� ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnClose();

	void SetAlphaSpin(void);
	void SetLinePattern(void);
	void SetStartCap(void);
	void SetEndCap(void);
	void SetLineThickness(void);
};