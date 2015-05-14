/* [ BrushPattern Ŭ���� by ������ ]
** BrushPatternŬ������ �������Դϴ�.
*/
// BrushPattern.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "BrushPattern.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"
#include "MainFrm.h"
#include "GraphicEditorView.h"

// BrushPattern ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(BrushPattern, CDialog)

// ������
BrushPattern::BrushPattern(CWnd* pParent /*=NULL*/)
	: CDialog(BrushPattern::IDD, pParent)
{

}

// �Ҹ���
BrushPattern::~BrushPattern()
{
}

void BrushPattern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINEJOIN, m_pLineJoinCombo);
	DDX_Control(pDX, IDC_FILLTYPE_SOLID, m_pFillTypeSolid);
	DDX_Control(pDX, IDC_FILLTYPE_HATCH, m_pFillTypeHatch);
	DDX_Control(pDX, IDC_FILLTYPE_GRADIENT, m_pFillTypeGradient);
	DDX_Control(pDX, IDC_BUTTON_FILLCOLOR, m_pFillColorBtn);
	DDX_Control(pDX, IDC_COMBO_HATCHPATTERN, m_pHatchPatternCombo);
	DDX_Control(pDX, IDC_BUTTON_GRADIENTCOLOR, m_pGrdColorBtn);
	DDX_Control(pDX, IDC_COMBO_GRADIENTPATTERN, m_pGrdPtnCombo);
	DDX_Control(pDX, IDC_EDIT_FILLALPHAEDIT, m_pFillAlphaEdit);
	DDX_Control(pDX, IDC_SPIN_FILLALPHASPIN, m_pFillAlphaSpin);
	DDX_Control(pDX, IDC_OUTLINEONLY, m_pOutlineOnly);
	DDX_Control(pDX, IDC_FILLOUTLINE, m_pFillOutline);
	DDX_Control(pDX, IDC_FillOnly, m_pFillOnly);
}


BEGIN_MESSAGE_MAP(BrushPattern, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILLCOLOR, &BrushPattern::OnBnClickedButtonFillcolor)
	ON_BN_CLICKED(IDC_FILLTYPE_HATCH, &BrushPattern::OnBnClickedFilltypeHatch)
	ON_BN_CLICKED(IDC_FILLTYPE_GRADIENT, &BrushPattern::OnBnClickedFilltypeGradient)
	ON_CBN_SELCHANGE(IDC_COMBO_HATCHPATTERN, &BrushPattern::OnCbnSelchangeComboHatchpattern)
	ON_BN_CLICKED(IDC_BUTTON_GRADIENTCOLOR, &BrushPattern::OnBnClickedButtonGradientcolor)
	ON_BN_CLICKED(IDC_OUTLINEONLY, &BrushPattern::OnBnClickedOutlineonly)
	ON_BN_CLICKED(IDC_FILLTYPE_SOLID, &BrushPattern::OnBnClickedFilltypeSolid)
	ON_BN_CLICKED(IDC_FILLOUTLINE, &BrushPattern::OnBnClickedFilloutline)
	ON_BN_CLICKED(IDC_FillOnly, &BrushPattern::OnBnClickedFillonly)
	ON_EN_CHANGE(IDC_EDIT_FILLALPHAEDIT, &BrushPattern::OnEnChangeEditFillalphaedit)
	ON_CBN_SELCHANGE(IDC_COMBO_LINEJOIN, &BrushPattern::OnCbnSelchangeComboLinejoin)
	ON_CBN_SELCHANGE(IDC_COMBO_GRADIENTPATTERN, &BrushPattern::OnCbnSelchangeComboGradientpattern)
ON_BN_CLICKED(IDOK, &BrushPattern::OnBnClickedOk)
ON_WM_CLOSE()
END_MESSAGE_MAP()

//--------------------------------------------------
//����ȭ���� �ε��� ���������� ���� ����
//--------------------------------------------------
BOOL BrushPattern::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	// �ܰ��� ��� �޺� �ڽ� �ʱ�ȭ
	this->m_pLineJoinCombo.SetCurSel(CDrawPropertyValue::m_nsLineJoin);

	//////////////////////////////////////////////////////////////////////////
	// �귯�� ���� �޺� �ڽ� �ʱ�ȭ
	for(int i = 0; i < 52; i++)
		this->m_pHatchPatternCombo.AddItem(i, HATCHPATTERN);
	this->m_pHatchPatternCombo.SetCurSel(CDrawPropertyValue::m_nsHatchStyle);

	//////////////////////////////////////////////////////////////////////////
	// �׶���Ʈ �޺� �ڽ� �ʱ�ȭ
	for(int i=0; i<4; i++)
		this->m_pGrdPtnCombo.AddItem(i, GRADIENTPATTERN);
	this->m_pGrdPtnCombo.SetCurSel(CDrawPropertyValue::m_nsGradientType);

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ��Ʈ�� �ʱ�ȭ
	this->m_pFillAlphaSpin.SetRange(1, 100);	//�ּ� ������ 1% ~ �ִ� ������ 100%
	this->m_pFillAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);		//�⺻�� 100

	//////////////////////////////////////////////////////////////////////////
	// ���� ��Ʈ�� �ʱ�ȭ
	CMFCToolTipInfo params;
	params.m_bBoldLabel = TRUE;
	params.m_bDrawDescription = TRUE;
	params.m_bDrawIcon = TRUE;
	params.m_bRoundedCorners = TRUE;
	params.m_bDrawSeparator = FALSE;
	params.m_clrFill = RGB (255, 255, 255);
	params.m_clrFillGradient = RGB (228, 228, 240);
	params.m_clrText = RGB (61, 83, 80);
	params.m_clrBorder = RGB (144, 149, 168);
	this->m_pCtrlToolTip.SetParams(&params);

	this->m_pCtrlToolTip.Create(this);
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_FILLALPHAEDIT), L"����");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_LINEJOIN), L"�ܰ��� ó��");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_OUTLINEONLY), L"�ܰ����� �׸���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLOUTLINE), L"�ܰ���+ä���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FillOnly), L"���θ� ä���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_SOLID), L"�ܻ����� ä���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_HATCH), L"���ڹ��� ä���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_GRADIENT), L"�׶���Ʈ ä���");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_HATCHPATTERN), L"���� ����");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_GRADIENTPATTERN), L"�׶���Ʈ ����");
	
	//////////////////////////////////////////////////////////////////////////
	// �ʱ� ��������
	// : �׸����� �ϴ� ������ ��Ÿ�Ͽ� �°� ���� ����

	// �ܰ�����, �ܰ��� �� ��ĥ, ��ĥ��
	switch(CDrawPropertyValue::m_nsFaceType) {
	case 0 : //�ܰ���
		//�ܰ����� �׸��� ���� ��ư ON, ������ ������ư OFF
		this->m_pOutlineOnly.SetCheck(1);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow(FALSE); //ä��� ��ư ��Ȱ��ȭ
		break;

	case 1 : //�ܰ���+��ĥ
		//�ܰ���+��ĥ ���� ��ư ON, ������ ������ư OFF
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(1);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow(); //ä��� ��ư Ȱ��ȭ
		break;

	case 2 : //��ĥ�� �ϱ�
		//��ĥ�� �ϴ� ���� ��ư ON, ������ ������ư OFF
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(1);
		this->m_pFillColorBtn.EnableWindow(); //ä��� ��ư Ȱ��ȭ
	
		break;
	}
	
	// �ָ���, ��ġ, �׶���Ʈ
	switch(CDrawPropertyValue::m_nsBrushStyle) {
	case 0 : //SOLID
		//SOLID ���� ��ư ON, ������ ���� ��ư OFF
		this->m_pFillTypeSolid.SetCheck(1);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pHatchPatternCombo.EnableWindow(FALSE); //Hatch ���� �޺��ڽ� ��Ȱ��ȭ
		this->m_pGrdColorBtn.EnableWindow(FALSE); //�׶���Ʈ �÷� ��ư ��Ȱ��ȭ
		this->m_pGrdPtnCombo.EnableWindow(FALSE); //�׶���Ʈ �޺� �ڽ� ��Ȱ��ȭ
		break;

	case 1 : //HATCH
		//HATCH ���� ��ư ON, ������ ���� ��ư OFF
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(1);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pHatchPatternCombo.EnableWindow(); //Hatch ���� �޺��ڽ� Ȱ��ȭ
		this->m_pGrdColorBtn.EnableWindow(FALSE); //�׶���Ʈ �÷� ��ư ��Ȱ��ȭ
		this->m_pGrdPtnCombo.EnableWindow(FALSE); //�׶���Ʈ �޺� �ڽ� ��Ȱ��ȭ
		break;

	case 2 : //GRADIENT
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(1);
		this->m_pHatchPatternCombo.EnableWindow(FALSE); //Hatch ���� �޺��ڽ� ��Ȱ��ȭ
		this->m_pGrdColorBtn.EnableWindow(); //�׶���Ʈ �÷� ��ư Ȱ��ȭ
		this->m_pGrdPtnCombo.EnableWindow(); //�׶���Ʈ �޺� �ڽ� Ȱ��ȭ
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

//--------------------------------------------------
//��ä��� �� ��ư Ŭ�� �� ȣ��
//��: ä��� ���� �����ϰ�, ���������ӿ� �پ��ִ� �Ӽ�â�� ���� ����
//--------------------------------------------------
void BrushPattern::OnBnClickedButtonFillcolor()
{
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK) {
		CDrawPropertyValue::m_sBrushColor = dlg.GetColor();
		Invalidate(TRUE);		// �ٲ� ������ ��ư �� ����
	}
	
	//�� ������ �ܺθ������ �˸���, �귯�� �÷�(ä����)�� �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGEFILLCOLOR);
	pMainFrm->m_dlgDrawProperty.Invalidate(); //���������ӿ� �پ��ִ� �Ӽ����ٸ� �ٽñ׸�
}

//--------------------------------------------------
//��Solid ä��� Ÿ�� ���� �� ȣ��
//��: Solid ��Ÿ���� �����ϰ�, 
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeSolid()
{
	// Solid ��Ÿ�� ������
	// ������ �������� Disable
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = 0;
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	this->m_pHatchPatternCombo.EnableWindow(FALSE);
	this->m_pGrdColorBtn.EnableWindow(FALSE);
	this->m_pGrdPtnCombo.EnableWindow(FALSE);

	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	pMainFrm->m_dlgDrawProperty.SetFillStyle(CDrawPropertyValue::m_nsBrushStyle);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
}
//--------------------------------------------------
//��Hatch ä��� Ÿ�� ���� �� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeHatch()
{
	// Hatch ��Ÿ�� ������
	// Hatch ���� �޺��ڽ��� Enable ������
	// Gradient �������� Disable
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = 1;
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	this->m_pHatchPatternCombo.EnableWindow(TRUE);
	this->m_pGrdColorBtn.EnableWindow(FALSE);
	this->m_pGrdPtnCombo.EnableWindow(FALSE);

	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();

	pMainFrm->m_dlgDrawProperty.SetFillStyle(CDrawPropertyValue::m_nsBrushStyle);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
}

//--------------------------------------------------
//��Gradient ä��� Ÿ�� ���� �� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeGradient()
{
	// Gradient ��Ÿ�� ������
	// Gradient �� ���� ��ư�� ���� ��ư�� Enable
	// Hatch �޺��ڽ��� Disable
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = 2;
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	
	this->m_pGrdColorBtn.EnableWindow(TRUE);
	this->m_pGrdPtnCombo.EnableWindow(TRUE);
	this->m_pHatchPatternCombo.EnableWindow(FALSE);

	CGraphicEditorView *pView = (CGraphicEditorView *)pMainFrm->GetActiveView();

	pMainFrm->m_dlgDrawProperty.SetFillStyle(CDrawPropertyValue::m_nsBrushStyle);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
}

//--------------------------------------------------
//��Hatch ���� ���� �� ȣ��
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboHatchpattern()
{
	CDrawPropertyValue::m_nsHatchStyle = this->m_pHatchPatternCombo.GetCurSel();

	//�� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	pMainFrm->m_dlgDrawProperty.SetHatchPattern();
}

//--------------------------------------------------
//���׶���Ʈ �÷� ���� �� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedButtonGradientcolor()
{
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK) {
		CDrawPropertyValue::m_sGradientEndColor = dlg.GetColor();
		Invalidate();
	}
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView *pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	pMainFrm->m_dlgDrawProperty.Invalidate();
}

//--------------------------------------------------
//���ܰ����� �׸��� ���� �������� ��� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedOutlineonly()
{
	//�Ӽ� �� ����
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = 0;

	this->m_pFillColorBtn.EnableWindow(FALSE);
	this->m_pFillTypeSolid.EnableWindow(FALSE);
	this->m_pFillTypeHatch.EnableWindow(FALSE);
	this->m_pFillTypeGradient.EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//�����������ӿ� �پ��ִ� �Ӽ� ���̾�α��� ���� ���� �� ��Ʈ�� Ȱ��ȭ/��Ȱ��ȭ ����
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow(FALSE);
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow(FALSE);
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow(FALSE);
}

//--------------------------------------------------
//���ܰ���+ä��⸦ �������� ��� ��� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedFilloutline()
{
	//�Ӽ� �� ����
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = 1;

	this->m_pFillColorBtn.EnableWindow();
	this->m_pFillTypeSolid.EnableWindow();
	this->m_pFillTypeHatch.EnableWindow();
	this->m_pFillTypeGradient.EnableWindow();

	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//�����������ӿ� �پ��ִ� �Ӽ� ���̾�α��� ���� ���� �� ��Ʈ�� Ȱ��ȭ/��Ȱ��ȭ ����
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow();
}
//--------------------------------------------------
//��ä��⸸ �ϱ⸦ �������� ��� ȣ��
//--------------------------------------------------
void BrushPattern::OnBnClickedFillonly()
{
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = 2;

	this->m_pFillColorBtn.EnableWindow();
	this->m_pFillTypeSolid.EnableWindow();
	this->m_pFillTypeHatch.EnableWindow();
	this->m_pFillTypeGradient.EnableWindow();

	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//�����������ӿ� �پ��ִ� �Ӽ� ���̾�α��� ���� ���� �� ��Ʈ�� Ȱ��ȭ/��Ȱ��ȭ ����
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow();
}

//--------------------------------------------------
//�������� ������ ���� ��� ȣ��
//--------------------------------------------------
void BrushPattern::OnEnChangeEditFillalphaedit()
{
	if(this->m_pFillAlphaSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsAlpha = this->m_pFillAlphaSpin.GetPos();
	}

	if(this->m_pFillAlphaSpin.GetSafeOwner()) {
		//�� ������ �������� �ٲٴ� �޽����� ����
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGEALPHA);
		// ������ ������ ������ �β��� ����
		pMainFrm->m_dlgDrawProperty.SetAlphaSpin();
	}
}

//--------------------------------------------------
//���ܰ��� ó��(LineJoin) ������ ���� ��� ȣ��
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboLinejoin()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CDrawPropertyValue::m_nsLineJoin = this->m_pLineJoinCombo.GetCurSel();

	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//�����������ӿ� �پ��ִ� �Ӽ� ���̾�α��� ���� ����
	pMainFrm->m_dlgDrawProperty.SetLineJoin(CDrawPropertyValue::m_nsLineJoin);
}
//--------------------------------------------------
//���׶���Ʈ ���� ���� �� ȣ��
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboGradientpattern()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CDrawPropertyValue::m_nsGradientType = this->m_pGrdPtnCombo.GetCurSel();
	

	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ܺθ������ �˸���, �귯�� ������ �ٲٴ� �Լ��� ȣ��
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//�����������ӿ� �پ��ִ� �Ӽ� ���̾�α��� ���� ����
	pMainFrm->m_dlgDrawProperty.SetGradientPattern(CDrawPropertyValue::m_nsGradientType);
}

//--------------------------------------------------------------------------
//���޽����� ����ä ó��
//--------------------------------------------------------------------------
BOOL BrushPattern::PreTranslateMessage(MSG* pMsg)
{
	//Enter/Esc Ű �Է� �� ���̾�α׸� ����
	if(pMsg->message == WM_KEYDOWN) {
		switch(pMsg->wParam) {
		case VK_ESCAPE :
		case VK_RETURN :
			DestroyWindow();
			return TRUE;

		default:
			break;
		}
	}
	//��Ĩ ���
	if(this->m_pCtrlToolTip && this->m_pCtrlToolTip.m_hWnd)
	{ 
		this->m_pCtrlToolTip.RelayEvent(pMsg);  // ������ ������
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------
//�����̾�α� ����� ���� �۾�
//--------------------------------------------------------------------------
void BrushPattern::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_dlgBrushPtn = NULL;
	delete this;
}
//--------------------------------------------------------------------------
//��OK ��ư �� �۾�
//--------------------------------------------------------------------------
void BrushPattern::OnBnClickedOk()
{
	CDialog::OnOK();
	DestroyWindow();
}
//--------------------------------------------------------------------------
//�����̾�αװ� ������ ��� �۾�
//--------------------------------------------------------------------------
void BrushPattern::OnClose()
{
	CDialog::OnClose();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//�������� ���� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetAlphaSpin(void)
{
	if(m_pFillAlphaSpin.GetSafeHwnd())
		m_pFillAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);
}

//--------------------------------------------------------------------------
//���ܰ��� ���� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetLineJoin(void)
{
	if(this->m_pLineJoinCombo.GetSafeHwnd())
		this->m_pLineJoinCombo.SetCurSel(CDrawPropertyValue::m_nsLineJoin);
}

//--------------------------------------------------------------------------
//���׸��� ��� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetDrawStyle(void)
{
	switch(CDrawPropertyValue::m_nsFaceType) 
	{
	case 0 :
		this->m_pOutlineOnly.SetCheck(1);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow(FALSE);
		this->m_pFillTypeSolid.EnableWindow(FALSE);
		this->m_pFillTypeHatch.EnableWindow(FALSE);
		this->m_pFillTypeGradient.EnableWindow(FALSE);
		break;

	case 1 :
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(1);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow();
		this->m_pFillTypeSolid.EnableWindow();
		this->m_pFillTypeHatch.EnableWindow();
		this->m_pFillTypeGradient.EnableWindow();
		break;

	case 2 :
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(1);
		this->m_pFillColorBtn.EnableWindow();
		this->m_pFillTypeSolid.EnableWindow();
		this->m_pFillTypeHatch.EnableWindow();
		this->m_pFillTypeGradient.EnableWindow();
		break;
	}
}

//--------------------------------------------------------------------------
//��ä��� ��Ÿ�� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetFillStyle(void)
{
	switch(CDrawPropertyValue::m_nsBrushStyle) {
	case 0 :
		this->m_pFillTypeSolid.SetCheck(1);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pGrdColorBtn.EnableWindow(FALSE);
		m_pHatchPatternCombo.EnableWindow(FALSE);
		m_pGrdPtnCombo.EnableWindow(FALSE);
		break;

	case 1 :
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(1);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pGrdColorBtn.EnableWindow(FALSE);
		m_pHatchPatternCombo.EnableWindow();
		m_pGrdPtnCombo.EnableWindow(FALSE);
		break;

	case 2 :
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(1);
		this->m_pGrdColorBtn.EnableWindow(TRUE);
		m_pHatchPatternCombo.EnableWindow(FALSE);
		m_pGrdPtnCombo.EnableWindow();
		break;
	}
}

//--------------------------------------------------------------------------
//���׶���Ʈ ���� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetGradientPattern(void)
{
	if(this->m_pGrdPtnCombo.GetSafeHwnd())
		this->m_pGrdPtnCombo.SetCurSel(CDrawPropertyValue::m_nsGradientType);
}

//--------------------------------------------------------------------------
//����ġ ���� ��Ʈ�� ����
//--------------------------------------------------------------------------
void BrushPattern::SetHatchPattern(void)
{
	if(this->m_pHatchPatternCombo.GetSafeHwnd())
		this->m_pHatchPatternCombo.SetCurSel(CDrawPropertyValue::m_nsHatchStyle);
}