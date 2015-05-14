/* [ BrushPattern Ŭ���� by ������ ]
** BrushPattern�� �����ϱ� ���� ��ȭ���� ���
** BrushPattern Ŭ�����Դϴ�.
** Brush ����, �׸��� ����, �׶���Ʈ ���� �Ӽ����� ����
*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DrawComboBox.h"
#include "DrawToolTipCtrl.h"

// BrushPattern ��ȭ �����Դϴ�.

class BrushPattern : public CDialog
{
	DECLARE_DYNAMIC(BrushPattern)

public:
	BrushPattern(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BrushPattern();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BRUSHPATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public :
	CComboBox m_pLineJoinCombo;				// �ܰ��� ó�� ��Ʈ�� ����

	CButton m_pFillTypeSolid;				// ä��� Ÿ�� �ָ��� ��Ʈ�� ����
	CButton m_pFillTypeHatch;				// ä��� Ÿ�� ��ġ ��Ʈ�� ����
	CButton m_pFillTypeGradient;			// ä��� Ÿ�� �׶���Ʈ  ��Ʈ�� ����

	CButton m_pFillColorBtn;				// ä��� �� ��ư ��Ʈ�� ����
	CButton m_pGrdColorBtn;					// �׶���Ʈ �� ��ư ��Ʈ�� ����

	CDrawComboBox m_pHatchPatternCombo;		// ä��� ���� ��ġ ��Ʈ�� ����
	CDrawComboBox m_pGrdPtnCombo;			// ä��� ���� �׶���Ʈ ��Ʈ�� ����
	
	CEdit m_pFillAlphaEdit;					// ����
	CSpinButtonCtrl m_pFillAlphaSpin;		// ���� ���� ��Ʈ�� ����

	CButton m_pOutlineOnly;					// �� Ÿ�� �ܰ��� ������ư ��Ʈ�� ����
	CButton m_pFillOutline;					// �� Ÿ�� �ܰ��� + ä��������ư ��Ʈ�� ����
	CButton m_pFillOnly;					// �� Ÿ�� ä��� ������ư ��Ʈ�� ����

	//����
	CDrawToolTipCtrl m_pCtrlToolTip;		// ���� ��Ʈ��

public :
	afx_msg void OnBnClickedButtonFillcolor();
	afx_msg void OnBnClickedFilltypeHatch();
	afx_msg void OnBnClickedFilltypeGradient();
	afx_msg void OnCbnSelchangeComboHatchpattern();
	afx_msg void OnBnClickedButtonGradientcolor();

	afx_msg void OnBnClickedOutlineonly();
	afx_msg void OnBnClickedFilltypeSolid();
	afx_msg void OnBnClickedFilloutline();
	afx_msg void OnBnClickedFillonly();
	afx_msg void OnEnChangeEditFillalphaedit();
	afx_msg void OnCbnSelchangeComboLinejoin();
	afx_msg void OnStnClickedStaticLinejoin();
	afx_msg void OnCbnSelchangeComboGradientpattern();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	void SetAlphaSpin(void);
	void SetLineJoin(void);
	void SetDrawStyle(void);
	void SetFillStyle(void);
	void SetGradientPattern(void);
	void SetHatchPattern(void);
};