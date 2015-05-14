/* [ BrushPattern 클래스 by 박현덕 ]
** BrushPattern클래스의 구현부입니다.
*/
// BrushPattern.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "BrushPattern.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"
#include "MainFrm.h"
#include "GraphicEditorView.h"

// BrushPattern 대화 상자입니다.

IMPLEMENT_DYNAMIC(BrushPattern, CDialog)

// 생성자
BrushPattern::BrushPattern(CWnd* pParent /*=NULL*/)
	: CDialog(BrushPattern::IDD, pParent)
{

}

// 소멸자
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
//　대화상자 로딩시 라인패턴의 정보 설정
//--------------------------------------------------
BOOL BrushPattern::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	// 외곽선 모양 콤보 박스 초기화
	this->m_pLineJoinCombo.SetCurSel(CDrawPropertyValue::m_nsLineJoin);

	//////////////////////////////////////////////////////////////////////////
	// 브러쉬 패턴 콤보 박스 초기화
	for(int i = 0; i < 52; i++)
		this->m_pHatchPatternCombo.AddItem(i, HATCHPATTERN);
	this->m_pHatchPatternCombo.SetCurSel(CDrawPropertyValue::m_nsHatchStyle);

	//////////////////////////////////////////////////////////////////////////
	// 그라디언트 콤보 박스 초기화
	for(int i=0; i<4; i++)
		this->m_pGrdPtnCombo.AddItem(i, GRADIENTPATTERN);
	this->m_pGrdPtnCombo.SetCurSel(CDrawPropertyValue::m_nsGradientType);

	//////////////////////////////////////////////////////////////////////////
	// 투명도 스핀 컨트롤 초기화
	this->m_pFillAlphaSpin.SetRange(1, 100);	//최소 불투명도 1% ~ 최대 불투명도 100%
	this->m_pFillAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);		//기본값 100

	//////////////////////////////////////////////////////////////////////////
	// 툴팁 컨트롤 초기화
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
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_FILLALPHAEDIT), L"투명도");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_LINEJOIN), L"외곽선 처리");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_OUTLINEONLY), L"외곽선만 그리기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLOUTLINE), L"외곽선+채우기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FillOnly), L"내부만 채우기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_SOLID), L"단색으로 채우기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_HATCH), L"격자무늬 채우기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_GRADIENT), L"그라디언트 채우기");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_HATCHPATTERN), L"패턴 설정");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_GRADIENTPATTERN), L"그라디언트 패턴");
	
	//////////////////////////////////////////////////////////////////////////
	// 초기 정보세팅
	// : 그리고자 하는 도형의 스타일에 맞게 정보 설정

	// 외곽선만, 외곽선 및 색칠, 색칠만
	switch(CDrawPropertyValue::m_nsFaceType) {
	case 0 : //외곽선
		//외곽선만 그리는 라디오 버튼 ON, 나머지 라디오버튼 OFF
		this->m_pOutlineOnly.SetCheck(1);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow(FALSE); //채우기 버튼 비활성화
		break;

	case 1 : //외곽선+색칠
		//외곽선+색칠 라디오 버튼 ON, 나머지 라디오버튼 OFF
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(1);
		this->m_pFillOnly.SetCheck(0);
		this->m_pFillColorBtn.EnableWindow(); //채우기 버튼 활성화
		break;

	case 2 : //색칠만 하기
		//색칠만 하는 라디오 버튼 ON, 나머지 라디오버튼 OFF
		this->m_pOutlineOnly.SetCheck(0);
		this->m_pFillOutline.SetCheck(0);
		this->m_pFillOnly.SetCheck(1);
		this->m_pFillColorBtn.EnableWindow(); //채우기 버튼 활성화
	
		break;
	}
	
	// 솔리드, 해치, 그라디언트
	switch(CDrawPropertyValue::m_nsBrushStyle) {
	case 0 : //SOLID
		//SOLID 라디오 버튼 ON, 나머지 라디오 버튼 OFF
		this->m_pFillTypeSolid.SetCheck(1);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pHatchPatternCombo.EnableWindow(FALSE); //Hatch 패턴 콤보박스 비활성화
		this->m_pGrdColorBtn.EnableWindow(FALSE); //그라디언트 컬러 버튼 비활성화
		this->m_pGrdPtnCombo.EnableWindow(FALSE); //그라디언트 콤보 박스 비활성화
		break;

	case 1 : //HATCH
		//HATCH 라디오 버튼 ON, 나머지 라디오 버튼 OFF
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(1);
		this->m_pFillTypeGradient.SetCheck(0);
		this->m_pHatchPatternCombo.EnableWindow(); //Hatch 패턴 콤보박스 활성화
		this->m_pGrdColorBtn.EnableWindow(FALSE); //그라디언트 컬러 버튼 비활성화
		this->m_pGrdPtnCombo.EnableWindow(FALSE); //그라디언트 콤보 박스 비활성화
		break;

	case 2 : //GRADIENT
		this->m_pFillTypeSolid.SetCheck(0);
		this->m_pFillTypeHatch.SetCheck(0);
		this->m_pFillTypeGradient.SetCheck(1);
		this->m_pHatchPatternCombo.EnableWindow(FALSE); //Hatch 패턴 콤보박스 비활성화
		this->m_pGrdColorBtn.EnableWindow(); //그라디언트 컬러 버튼 활성화
		this->m_pGrdPtnCombo.EnableWindow(); //그라디언트 콤보 박스 활성화
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

//--------------------------------------------------
//　채우기 색 버튼 클릭 시 호출
//　: 채우기 색을 변경하고, 메인프레임에 붙어있는 속성창의 색도 변경
//--------------------------------------------------
void BrushPattern::OnBnClickedButtonFillcolor()
{
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK) {
		CDrawPropertyValue::m_sBrushColor = dlg.GetColor();
		Invalidate(TRUE);		// 바꾼 색으로 버튼 색 변경
	}
	
	//뷰 영역에 외부명령임을 알리고, 브러쉬 컬러(채우기색)를 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGEFILLCOLOR);
	pMainFrm->m_dlgDrawProperty.Invalidate(); //메인프레임에 붙어있는 속성툴바를 다시그림
}

//--------------------------------------------------
//　Solid 채우기 타입 선택 시 호출
//　: Solid 스타일을 지정하고, 
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeSolid()
{
	// Solid 스타일 지정시
	// 나머지 설정들은 Disable
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
//　Hatch 채우기 타입 선택 시 호출
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeHatch()
{
	// Hatch 스타일 지정시
	// Hatch 패턴 콤보박스를 Enable 시켜줌
	// Gradient 설정들은 Disable
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
//　Gradient 채우기 타입 선택 시 호출
//--------------------------------------------------
void BrushPattern::OnBnClickedFilltypeGradient()
{
	// Gradient 스타일 지정시
	// Gradient 색 지정 버튼과 패턴 버튼을 Enable
	// Hatch 콤보박스는 Disable
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
//　Hatch 패턴 변경 시 호출
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboHatchpattern()
{
	CDrawPropertyValue::m_nsHatchStyle = this->m_pHatchPatternCombo.GetCurSel();

	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	pMainFrm->m_dlgDrawProperty.SetHatchPattern();
}

//--------------------------------------------------
//　그라디언트 컬러 변경 시 호출
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
//　외곽선만 그리는 것을 선택했을 경우 호출
//--------------------------------------------------
void BrushPattern::OnBnClickedOutlineonly()
{
	//속성 값 지정
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = 0;

	this->m_pFillColorBtn.EnableWindow(FALSE);
	this->m_pFillTypeSolid.EnableWindow(FALSE);
	this->m_pFillTypeHatch.EnableWindow(FALSE);
	this->m_pFillTypeGradient.EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////
	// 뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//　메인프레임에 붙어있는 속성 다이얼로그의 내용 변경 및 컨트롤 활성화/비활성화 조정
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow(FALSE);
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow(FALSE);
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow(FALSE);
}

//--------------------------------------------------
//　외곽선+채우기를 선택했을 경우 경우 호출
//--------------------------------------------------
void BrushPattern::OnBnClickedFilloutline()
{
	//속성 값 지정
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = 1;

	this->m_pFillColorBtn.EnableWindow();
	this->m_pFillTypeSolid.EnableWindow();
	this->m_pFillTypeHatch.EnableWindow();
	this->m_pFillTypeGradient.EnableWindow();

	//////////////////////////////////////////////////////////////////////////
	// 뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//　메인프레임에 붙어있는 속성 다이얼로그의 내용 변경 및 컨트롤 활성화/비활성화 조정
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow();
}
//--------------------------------------------------
//　채우기만 하기를 선택했을 경우 호출
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
	// 뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//　메인프레임에 붙어있는 속성 다이얼로그의 내용 변경 및 컨트롤 활성화/비활성화 조정
	pMainFrm->m_dlgDrawProperty.SetDrawStyle(CDrawPropertyValue::m_nsFaceType);
	pMainFrm->m_dlgDrawProperty.AdjustControl();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeSolid.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeHatch.EnableWindow();
	pMainFrm->m_dlgDrawProperty.m_sFillTypeGradient.EnableWindow();
}

//--------------------------------------------------
//　불투명도 조절을 했을 경우 호출
//--------------------------------------------------
void BrushPattern::OnEnChangeEditFillalphaedit()
{
	if(this->m_pFillAlphaSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsAlpha = this->m_pFillAlphaSpin.GetPos();
	}

	if(this->m_pFillAlphaSpin.GetSafeOwner()) {
		//뷰 영역에 불투명도를 바꾸는 메시지를 날림
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGEALPHA);
		// 프레임 윈도우 툴바의 두께를 변경
		pMainFrm->m_dlgDrawProperty.SetAlphaSpin();
	}
}

//--------------------------------------------------
//　외곽선 처리(LineJoin) 변경을 했을 경우 호출
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboLinejoin()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CDrawPropertyValue::m_nsLineJoin = this->m_pLineJoinCombo.GetCurSel();

	//////////////////////////////////////////////////////////////////////////
	// 뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//　메인프레임에 붙어있는 속성 다이얼로그의 내용 변경
	pMainFrm->m_dlgDrawProperty.SetLineJoin(CDrawPropertyValue::m_nsLineJoin);
}
//--------------------------------------------------
//　그라디언트 패턴 변경 시 호출
//--------------------------------------------------
void BrushPattern::OnCbnSelchangeComboGradientpattern()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CDrawPropertyValue::m_nsGradientType = this->m_pGrdPtnCombo.GetCurSel();
	

	//////////////////////////////////////////////////////////////////////////
	// 뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();
	//////////////////////////////////////////////////////////////////////////
	//　메인프레임에 붙어있는 속성 다이얼로그의 내용 변경
	pMainFrm->m_dlgDrawProperty.SetGradientPattern(CDrawPropertyValue::m_nsGradientType);
}

//--------------------------------------------------------------------------
//　메시지를 가로채 처리
//--------------------------------------------------------------------------
BOOL BrushPattern::PreTranslateMessage(MSG* pMsg)
{
	//Enter/Esc 키 입력 시 다이얼로그를 닫음
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
	//툴칩 출력
	if(this->m_pCtrlToolTip && this->m_pCtrlToolTip.m_hWnd)
	{ 
		this->m_pCtrlToolTip.RelayEvent(pMsg);  // 툴팁을 보여줌
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------
//　다이얼로그 종료시 정리 작업
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
//　OK 버튼 시 작업
//--------------------------------------------------------------------------
void BrushPattern::OnBnClickedOk()
{
	CDialog::OnOK();
	DestroyWindow();
}
//--------------------------------------------------------------------------
//　다이얼로그가 닫혀질 경우 작업
//--------------------------------------------------------------------------
void BrushPattern::OnClose()
{
	CDialog::OnClose();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//　불투명도 스핀 컨트롤 조정
//--------------------------------------------------------------------------
void BrushPattern::SetAlphaSpin(void)
{
	if(m_pFillAlphaSpin.GetSafeHwnd())
		m_pFillAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);
}

//--------------------------------------------------------------------------
//　외곽선 형태 컨트롤 조정
//--------------------------------------------------------------------------
void BrushPattern::SetLineJoin(void)
{
	if(this->m_pLineJoinCombo.GetSafeHwnd())
		this->m_pLineJoinCombo.SetCurSel(CDrawPropertyValue::m_nsLineJoin);
}

//--------------------------------------------------------------------------
//　그리기 모양 컨트롤 조정
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
//　채우기 스타일 컨트롤 조정
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
//　그라디언트 패턴 컨트롤 조정
//--------------------------------------------------------------------------
void BrushPattern::SetGradientPattern(void)
{
	if(this->m_pGrdPtnCombo.GetSafeHwnd())
		this->m_pGrdPtnCombo.SetCurSel(CDrawPropertyValue::m_nsGradientType);
}

//--------------------------------------------------------------------------
//　해치 패턴 컨트롤 조정
//--------------------------------------------------------------------------
void BrushPattern::SetHatchPattern(void)
{
	if(this->m_pHatchPatternCombo.GetSafeHwnd())
		this->m_pHatchPatternCombo.SetCurSel(CDrawPropertyValue::m_nsHatchStyle);
}