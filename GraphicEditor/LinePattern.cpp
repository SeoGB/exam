/* [ LinePattern Ŭ���� by ������ ]
** LinePattern Ŭ������ �������Դϴ�.
*/

// LinePattern.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "LinePattern.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"
#include "MainFrm.h"
#include "GraphicEditorView.h"
// LinePattern ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(LinePattern, CDialog)

// ������
LinePattern::LinePattern(CWnd* pParent /*=NULL*/)
	: CDialog(LinePattern::IDD, pParent)
{

}

// �Ҹ���
LinePattern::~LinePattern()
{
}

void LinePattern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_pLineWidthEdit);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_pLineWidthSpin);
	DDX_Control(pDX, IDC_COMBO_DASHSTYLE, m_pLinePtnCmb);
	DDX_Control(pDX, IDC_BUTTON_LINECOLOR, m_pLineColorBtn);
	DDX_Control(pDX, IDC_COMBO_STARTCAP, m_pStartCapCombo);
	DDX_Control(pDX, IDC_COMBO_ENDCAP, m_pEndCapCombo);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_pAlphaEdit);
	DDX_Control(pDX, IDC_SPIN_ALPHA, m_pAlphaSpin);
}


BEGIN_MESSAGE_MAP(LinePattern, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LINECOLOR, &LinePattern::OnBnClickedButtonLinecolor)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &LinePattern::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, &LinePattern::OnEnChangeEditAlpha)
	ON_CBN_SELCHANGE(IDC_COMBO_DASHSTYLE, &LinePattern::OnCbnSelchangeComboDashstyle)
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_STARTCAP, &LinePattern::OnCbnSelchangeComboStartcap)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAP, &LinePattern::OnCbnSelchangeComboEndcap)
	ON_BN_CLICKED(IDCLOSE, &LinePattern::OnBnClickedClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// LinePattern �޽��� ó�����Դϴ�.

//--------------------------------------------------
//����ȭ���� �ε��� ���������� ���� ����
//--------------------------------------------------
BOOL LinePattern::OnInitDialog()
{
	CDialog::OnInitDialog();

	int startIndex;
	int endIndex;

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� ��Ʈ�� �ʱ�ȭ
	this->m_pLineWidthSpin.SetRange(1,30);		// �ּ� 1 ~ �ִ� 30
	this->m_pLineWidthSpin.SetPos(CDrawPropertyValue::m_nsThickness);

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ��Ʈ�� �ʱ�ȭ
	this->m_pAlphaSpin.SetRange(1, 100);		// �ּ� 1 ~ �ִ� 100
	this->m_pAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);

	//////////////////////////////////////////////////////////////////////////
	// �뽬 ���� �޺��ڽ� �ʱ�ȭ
	for(int i = 0; i < 5; i++) {
		this->m_pLinePtnCmb.AddItem(i, LINEPATTERN);
	}
	this->m_pLinePtnCmb.SetCurSel(CDrawPropertyValue::m_nsPenStyle);

	//////////////////////////////////////////////////////////////////////////
	// �� ����-�� ��� �޺��ڽ� �ʱ�ȭ
	this->ChangeIndex(&startIndex, &endIndex);
	this->m_pStartCapCombo.SetCurSel(startIndex);
	this->m_pEndCapCombo.SetCurSel(endIndex);

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
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_ALPHA), L"����");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_WIDTH), L"�β�");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_DASHSTYLE), L"�� ��Ÿ��");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_LINECOLOR), L"�� ��");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_STARTCAP), L"�� ���۸��");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_ENDCAP), L"�� �����");

	return TRUE;  // return TRUE unless you set the focus to a control
}

//--------------------------------------------------------------------------
//������ ��
//--------------------------------------------------------------------------
void LinePattern::OnBnClickedButtonLinecolor()
{
	CColorDialog dlg;
	CDC dc;
	CRect rect;

	if(dlg.DoModal() == IDOK) {
		CDrawPropertyValue::m_sLineColor = dlg.GetColor();
		Invalidate();
	}
	// �� ������ �ܺθ������ �˸���, ���� �÷� �ٲٴ� �Լ� ȣ��
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView *pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINECOLOR);
	pMainFrm->m_dlgDrawProperty.Invalidate();
}

//--------------------------------------------------------------------------
//���� ���� ����
//--------------------------------------------------------------------------
void LinePattern::OnEnChangeEditWidth()
{
	if(this->m_pLineWidthSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsThickness = this->m_pLineWidthSpin.GetPos();
	}

	if(this->m_pLineWidthSpin.GetSafeOwner()) {
		//�� ������ ���� ���⸦ �ٲٴ� �޽����� ����
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGELINEWEIGHT);
		pMainFrm->m_dlgDrawProperty.SetLineWeightSpin();
	}
}

//--------------------------------------------------------------------------
//������ ����
//--------------------------------------------------------------------------
void LinePattern::OnEnChangeEditAlpha()
{
	if(this->m_pAlphaSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsAlpha = this->m_pAlphaSpin.GetPos();
	}

	// �Լ� �����ϸ� ���Ḹ �ϸ� �� �β�ó�� �ٷ� ������� Ȯ�ΰ���
	if(this->m_pAlphaSpin.GetSafeOwner()) {
		//�� ������ ���� ���⸦ �ٲٴ� �޽����� ����
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGEALPHA);
		// ������ ������ ���ٿ� �β��� ����
		pMainFrm->m_dlgDrawProperty.SetAlphaSpin();
	}
}

//--------------------------------------------------------------------------
// �� ��Ÿ�� ����
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboDashstyle()
{
	CDrawPropertyValue::m_nsPenStyle = this->m_pLinePtnCmb.GetCurSel();

	//�� ������ �ܺθ������ �˸���, ���� ������ �ٲٴ� �Լ��� ȣ��
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();
	pMainFrm->m_dlgDrawProperty.SetLinePattern();
}

//--------------------------------------------------------------------------
//���� ���� ��� ����
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboStartcap()
{
	// ���;�ձ۰�;�����ϰ�;�簢��;��;���̾Ƹ��;ȭ��ǥ
	int index;
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	
	switch(this->m_pStartCapCombo.GetCurSel()) {
	case 0: //���
		index = 0; //LineCap::LineCapFlat;
		pMainFrm->m_dlgDrawProperty.SetStartCap(0);
		break;
	case 1: //�ձ۰�
		index = 2; //LineCap::LineCapRound
		pMainFrm->m_dlgDrawProperty.SetStartCap(1);
		break;
	case 2: //�����ϰ�
		index = 3; //LineCap::LineCapTriangle
		pMainFrm->m_dlgDrawProperty.SetStartCap(2);
		break;
	case 3: //�簢��
		index = 17; //LineCap::LineCapSquareAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(3);
		break;
	case 4: //��
		index = 18; //LineCap::LineCapRoundAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(4);
		break;
	case 5: //���̾Ƹ��
		index = 19; //LineCap::LineCapDiamondAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(5);
		break;
	case 6: //ȭ��ǥ
		index = 20; //LineCap::LineCapArrowAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(6);
		break;
	}
	CDrawPropertyValue::m_nsStartCap = index;
	
	//�� ������ �ܺθ������ �˸���, ���� ������ �ٲٴ� �Լ��� ȣ��
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();

}

//--------------------------------------------------------------------------
//���� �� ��� ����
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboEndcap()
{
	// ���;�ձ۰�;�����ϰ�;�簢��;��;���̾Ƹ��;ȭ��ǥ
	int index;
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	switch(this->m_pEndCapCombo.GetCurSel()) {
	case 0: //���
		index = 0; //LineCap::LineCapFlat;
		pMainFrm->m_dlgDrawProperty.SetEndCap(0);
		break;
	case 1: //�ձ۰�
		index = 2; //LineCap::LineCapRound
		pMainFrm->m_dlgDrawProperty.SetEndCap(1);
		break;
	case 2: //�����ϰ�
		index = 3; //LineCap::LineCapTriangle
		pMainFrm->m_dlgDrawProperty.SetEndCap(2);
		break;
	case 3: //�簢��
		index = 17; //LineCap::LineCapSquareAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(3);
		break;
	case 4: //��
		index = 18; //LineCap::LineCapRoundAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(4);
		break;
	case 5: //���̾Ƹ��
		index = 19; //LineCap::LineCapDiamondAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(5);
		break;
	case 6: //ȭ��ǥ
		index = 20; //LineCap::LineCapArrowAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(6);
		break;
	}
	CDrawPropertyValue::m_nsEndCap = index;

	//�� ������ �ܺθ������ �˸���, ���� ������ �ٲٴ� �Լ��� ȣ��
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();
}

void LinePattern::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData();
	CDC dc;
	CRect rect;

	switch(nIDCtl) {
	case IDC_COMBO_DASHSTYLE :
		if(this->m_pLinePtnCmb.GetSafeHwnd()) {
			this->m_pLinePtnCmb.DrawItem(lpDrawItemStruct);
		}
		break;
	}
	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void LinePattern::ChangeIndex(int *startIndex, int *endIndex) {

	switch(CDrawPropertyValue::m_nsStartCap) {
	case 0: //���
		*startIndex = 0;
		break;
	case 2: //�ձ۰�
		*startIndex = 1; //LineCap::LineCapRound
		break;
	case 3: //�����ϰ�
		*startIndex = 2; //LineCap::LineCapTriangle
		break;
	case 17: //�簢��
		*startIndex = 3; //LineCap::LineCapSquareAnchor
		break;
	case 18: //��
		*startIndex = 4; //LineCap::LineCapRoundAnchor
		break;
	case 19: //���̾Ƹ��
		*startIndex = 5; //LineCap::LineCapDiamondAnchor
		break;
	case 20: //ȭ��ǥ
		*startIndex = 6; //LineCap::LineCapArrowAnchor
		break;
	}

	switch(CDrawPropertyValue::m_nsEndCap) {
	case 0: //���
		*endIndex = 0;
		break;
	case 2: //�ձ۰�
		*endIndex = 1; //LineCap::LineCapRound
		break;
	case 3: //�����ϰ�
		*endIndex = 2; //LineCap::LineCapTriangle
		break;
	case 17: //�簢��
		*endIndex = 3; //LineCap::LineCapSquareAnchor
		break;
	case 18: //��
		*endIndex = 4; //LineCap::LineCapRoundAnchor
		break;
	case 19: //���̾Ƹ��
		*endIndex = 5; //LineCap::LineCapDiamondAnchor
		break;
	case 20: //ȭ��ǥ
		*endIndex = 6; //LineCap::LineCapArrowAnchor
		break;
	}
}

//--------------------------------------------------------------------------
//���޽����� ����ä ó��
//--------------------------------------------------------------------------
BOOL LinePattern::PreTranslateMessage(MSG* pMsg)
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

	if(this->m_pCtrlToolTip && this->m_pCtrlToolTip.m_hWnd) {

		// ���� ������
		this->m_pCtrlToolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------
//�����̾�α� ����� ���� �۾�
//--------------------------------------------------------------------------
void LinePattern::PostNcDestroy()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_dlgLinePtn = NULL;
	delete this;
}

//--------------------------------------------------------------------------
//��OK ��ư �� �۾�
//--------------------------------------------------------------------------
void LinePattern::OnBnClickedClose()
{
	CDialog::OnOK();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//�����̾�αװ� ������ ��� �۾�
//--------------------------------------------------------------------------
void LinePattern::OnClose()
{
	CDialog::OnClose();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//�����Ľ��� ��Ʈ�� ����
//--------------------------------------------------------------------------
void LinePattern::SetAlphaSpin(void)
{
	if(m_pAlphaSpin.GetSafeHwnd())
		m_pAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);
}

//--------------------------------------------------------------------------
//������ ���� ��Ʈ�� ����
//--------------------------------------------------------------------------
void LinePattern::SetLinePattern(void)
{
	if(m_pLinePtnCmb.GetSafeHwnd()) 
		this->m_pLinePtnCmb.SetCurSel(CDrawPropertyValue::m_nsPenStyle);
}

//--------------------------------------------------------------------------
//���� ���� ��� ��Ʈ�� ����
//--------------------------------------------------------------------------
void LinePattern::SetStartCap(void)
{
	int index;
	switch(CDrawPropertyValue::m_nsStartCap)
	{
	case 0:
		index = 0;
		break;
	case 2:
		index = 1;
		break;
	case 3:
		index = 2;
		break;
	case 17:
		index = 3;
		break;
	case 18:
		index = 4;
		break;
	case 19:
		index = 5;
		break;
	case 20:
		index = 6;
		break;
	}
	if(m_pStartCapCombo.GetSafeHwnd())
		this->m_pStartCapCombo.SetCurSel(index);
}

//--------------------------------------------------------------------------
//���� ����� ��Ʈ�� ����
//--------------------------------------------------------------------------
void LinePattern::SetEndCap(void)
{
	int index;
	switch(CDrawPropertyValue::m_nsEndCap)
	{
	case 0:
		index = 0;
		break;
	case 2:
		index = 1;
		break;
	case 3:
		index = 2;
		break;
	case 17:
		index = 3;
		break;
	case 18:
		index = 4;
		break;
	case 19:
		index = 5;
		break;
	case 20:
		index = 6;
		break;
	}
	if(m_pEndCapCombo.GetSafeHwnd())
		this->m_pEndCapCombo.SetCurSel(index);
}

//--------------------------------------------------------------------------
//������ ���� ���� ����
//--------------------------------------------------------------------------
void LinePattern::SetLineThickness(void)
{
	if(m_pLineWidthSpin.GetSafeHwnd())
		m_pLineWidthSpin.SetPos(CDrawPropertyValue::m_nsThickness);
}
