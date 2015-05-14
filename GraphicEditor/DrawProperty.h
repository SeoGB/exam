/* [ CDrawProperty Ŭ���� by ����ö ]
** ���� �����ӿ� �׻� �پ��ִ� �׸��� �Ӽ��� ���� ���̾�α� Ŭ�����Դϴ�.
*/

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DrawComboBox.h"
#include "DrawToolTipCtrl.h"

// CDrawProperty dialog
class CMainFrame;
class CGraphicEditorDoc;
class CGraphicEditorView;
class CDrawProperty : public CDialog
{
	DECLARE_DYNAMIC(CDrawProperty)

public:
	CDrawProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDrawProperty();

// Dialog Data
	enum { IDD = IDD_DRAW_PROPERTY };
	CButton m_sFontBtn;					//��Ʈ ��ư ��Ʈ�� ����

	CButton m_sFillColorBtn;			//ä��� �� ��ư ��Ʈ�� ����
	CButton m_sLineColorBtn;			//���� �� ��Ʈ�� ����
	CButton m_sGradientEndColorBtn;		//�׶���Ʈ �� �� ��Ʈ�� ����

	CSpinButtonCtrl m_sLineWeightSpin;	//���� ���� ���� ��Ʈ�� ����
	CEdit m_sThicknessEdit;				//���� ���� ����Ʈ ��Ʈ�� ����

	CDrawComboBox m_sLinePtnCombo;		//���� ���� ��Ʈ�� ����
	CDrawComboBox m_sFillPtnCombo;		//ä��� �� ��ư ��Ʈ�� ����

	CButton m_sFaceTypeOutline;			//�� Ÿ�� �ܰ��� ������ư ��Ʈ�� ����
	CButton m_sFaceTypeOutlinedFill;	//�� Ÿ�� �ܰ���+ä��� ������ư ��Ʈ�� ����
	CButton m_sFaceTypeFillOnly;		//�� Ÿ�� only ä��� ������ư ��Ʈ�� ����

	CButton m_sFillTypeSolid;			//ä��� Ÿ�� �ָ��� ������ư ��Ʈ�� ����
	CButton m_sFillTypeHatch;			//ä��� Ÿ�� ��ġ ������ư ��Ʈ�� ����
	CButton m_sFillTypeGradient;		//ä��� Ÿ�� �׶���Ʈ ������ư ��Ʈ�� ����

	CSpinButtonCtrl m_sAlphaSpin;		//������ ���� ��Ʈ�� ����
	CEdit m_sAlphaEdit;					//������

	CComboBox m_sStartCapCombo;			//�� ���� ��� ��Ʈ�� ����
	CComboBox m_sEndCapCombo;			//�� �� ��� ��Ʈ�� ����
	
	CDrawComboBox m_sGradientCombo;		//�׶���Ʈ ä��� Ÿ�� ��Ʈ�Ѻ���
	CComboBox m_sLineJoinCombo;			//�ܰ��� ��� ��Ʈ�� ����

	//���� ��Ʈ�� ����
	CStatic m_sStcFont;			//��Ʈ	
	CStatic m_sStcThickness;	//���� ����
	CStatic m_sStcLinePtn;		//���� ����
	CStatic m_sStcFillPtn;		//ä��� ����
	CStatic m_sStcOpacity;		//������
	CStatic m_sStcEndCap;		//���� �� ���
	CStatic m_sStcStartCap;		//���� ���� ���
	CStatic m_sStcGradientMode;	//�׶���Ʈ ���
	CStatic m_sStcLineJoin;		//�ܰ��� ���

	//��Ʈ�ѿ� ���� value ����
	int m_nsFaceType;					//�� Ÿ���� ���ð�
	int m_nsBrushType;					//ä��� Ÿ���� ���ð�

	//����
	CDrawToolTipCtrl m_sCtrlToolTip;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFont();
	afx_msg void OnBnClickedButtonFgcolor();
	afx_msg void OnBnClickedButtonFillcolor();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnEnChangeEditLineweight();
	void SetLineWeightSpin(void);
	afx_msg void OnSelchangeLinepattern();
	afx_msg void OnSelchangeFillpattern();
	afx_msg void OnClickedFaceType();
	afx_msg void OnClickedBrushType();
	afx_msg void OnFaceFill();
	afx_msg void OnFaceOnlyfill();
	afx_msg void OnFilltypeHatch();
	afx_msg void OnFilltypeGradient();
	afx_msg void OnChangeEditAlpha();

	void SetAlphaSpin(void);			// ������ �� ����
	void SetLinePattern(void);			// ���� ��Ÿ�� ����
	void SetStartCap(int startCap, BOOL isPropValue = FALSE);	// ������ ���� ��� ����
	void SetEndCap(int endCap, BOOL isPropValue = FALSE);		// ������ �� ��� ����
	void SetLineJoin(int lineJoin);			// �ܰ��� ó�� ����
	void SetHatchPattern();				// ��ġ ��Ÿ�� ����
	void SetDrawStyle(int style);			// �׸��� ���� ����
	void SetFillStyle(int fillStyle);		// ä��� ���� ����
	void SetGradientPattern(int grdPattern);	// �׶���Ʈ ���� ����

	afx_msg void OnCbnSelchangeComboStartcap();
	afx_msg void OnCbnSelchangeComboEndcap();
	afx_msg void OnCbnSelchangeComboGradient();
	afx_msg void OnBnClickedButtonFillcolor2();
	afx_msg void OnCbnSelchangeComboLinejoin();


	void AdjustControl(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
