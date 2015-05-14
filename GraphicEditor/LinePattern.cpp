/* [ LinePattern 클래스 by 박현덕 ]
** LinePattern 클래스의 구현부입니다.
*/

// LinePattern.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "LinePattern.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"
#include "MainFrm.h"
#include "GraphicEditorView.h"
// LinePattern 대화 상자입니다.

IMPLEMENT_DYNAMIC(LinePattern, CDialog)

// 생성자
LinePattern::LinePattern(CWnd* pParent /*=NULL*/)
	: CDialog(LinePattern::IDD, pParent)
{

}

// 소멸자
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


// LinePattern 메시지 처리기입니다.

//--------------------------------------------------
//　대화상자 로딩시 라인패턴의 정보 설정
//--------------------------------------------------
BOOL LinePattern::OnInitDialog()
{
	CDialog::OnInitDialog();

	int startIndex;
	int endIndex;

	//////////////////////////////////////////////////////////////////////////
	// 라인 굵기 스핀 컨트롤 초기화
	this->m_pLineWidthSpin.SetRange(1,30);		// 최소 1 ~ 최대 30
	this->m_pLineWidthSpin.SetPos(CDrawPropertyValue::m_nsThickness);

	//////////////////////////////////////////////////////////////////////////
	// 투명도 스핀 컨트롤 초기화
	this->m_pAlphaSpin.SetRange(1, 100);		// 최소 1 ~ 최대 100
	this->m_pAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);

	//////////////////////////////////////////////////////////////////////////
	// 대쉬 패턴 콤보박스 초기화
	for(int i = 0; i < 5; i++) {
		this->m_pLinePtnCmb.AddItem(i, LINEPATTERN);
	}
	this->m_pLinePtnCmb.SetCurSel(CDrawPropertyValue::m_nsPenStyle);

	//////////////////////////////////////////////////////////////////////////
	// 선 시작-끝 모양 콤보박스 초기화
	this->ChangeIndex(&startIndex, &endIndex);
	this->m_pStartCapCombo.SetCurSel(startIndex);
	this->m_pEndCapCombo.SetCurSel(endIndex);

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
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_ALPHA), L"투명도");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_WIDTH), L"두께");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_DASHSTYLE), L"선 스타일");
 	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_LINECOLOR), L"선 색");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_STARTCAP), L"선 시작모양");
	this->m_pCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_ENDCAP), L"선 끝모양");

	return TRUE;  // return TRUE unless you set the focus to a control
}

//--------------------------------------------------------------------------
//　라인 색
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
	// 뷰 영역에 외부명령임을 알리고, 라인 컬러 바꾸는 함수 호출
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView *pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINECOLOR);
	pMainFrm->m_dlgDrawProperty.Invalidate();
}

//--------------------------------------------------------------------------
//라인 굵기 지정
//--------------------------------------------------------------------------
void LinePattern::OnEnChangeEditWidth()
{
	if(this->m_pLineWidthSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsThickness = this->m_pLineWidthSpin.GetPos();
	}

	if(this->m_pLineWidthSpin.GetSafeOwner()) {
		//뷰 영역에 라인 굵기를 바꾸는 메시지를 날림
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGELINEWEIGHT);
		pMainFrm->m_dlgDrawProperty.SetLineWeightSpin();
	}
}

//--------------------------------------------------------------------------
//　투명도 지정
//--------------------------------------------------------------------------
void LinePattern::OnEnChangeEditAlpha()
{
	if(this->m_pAlphaSpin.GetSafeHwnd()) {
		CDrawPropertyValue::m_nsAlpha = this->m_pAlphaSpin.GetPos();
	}

	// 함수 생성하면 연결만 하면 선 두께처럼 바로 변경상태 확인가능
	if(this->m_pAlphaSpin.GetSafeOwner()) {
		//뷰 영역에 라인 굵기를 바꾸는 메시지를 날림
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGEALPHA);
		// 프레임 윈도우 툴바에 두께를 변경
		pMainFrm->m_dlgDrawProperty.SetAlphaSpin();
	}
}

//--------------------------------------------------------------------------
// 선 스타일 설정
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboDashstyle()
{
	CDrawPropertyValue::m_nsPenStyle = this->m_pLinePtnCmb.GetCurSel();

	//뷰 영역에 외부명령임을 알리고, 라인 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();
	pMainFrm->m_dlgDrawProperty.SetLinePattern();
}

//--------------------------------------------------------------------------
//　선 시작 모양 설정
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboStartcap()
{
	// 평면;둥글게;뾰족하게;사각형;원;다이아몬드;화살표
	int index;
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	
	switch(this->m_pStartCapCombo.GetCurSel()) {
	case 0: //평면
		index = 0; //LineCap::LineCapFlat;
		pMainFrm->m_dlgDrawProperty.SetStartCap(0);
		break;
	case 1: //둥글게
		index = 2; //LineCap::LineCapRound
		pMainFrm->m_dlgDrawProperty.SetStartCap(1);
		break;
	case 2: //뾰족하게
		index = 3; //LineCap::LineCapTriangle
		pMainFrm->m_dlgDrawProperty.SetStartCap(2);
		break;
	case 3: //사각형
		index = 17; //LineCap::LineCapSquareAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(3);
		break;
	case 4: //원
		index = 18; //LineCap::LineCapRoundAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(4);
		break;
	case 5: //다이아몬드
		index = 19; //LineCap::LineCapDiamondAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(5);
		break;
	case 6: //화살표
		index = 20; //LineCap::LineCapArrowAnchor
		pMainFrm->m_dlgDrawProperty.SetStartCap(6);
		break;
	}
	CDrawPropertyValue::m_nsStartCap = index;
	
	//뷰 영역에 외부명령임을 알리고, 라인 패턴을 바꾸는 함수를 호출
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();

}

//--------------------------------------------------------------------------
//　선 끝 모양 설정
//--------------------------------------------------------------------------
void LinePattern::OnCbnSelchangeComboEndcap()
{
	// 평면;둥글게;뾰족하게;사각형;원;다이아몬드;화살표
	int index;
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	switch(this->m_pEndCapCombo.GetCurSel()) {
	case 0: //평면
		index = 0; //LineCap::LineCapFlat;
		pMainFrm->m_dlgDrawProperty.SetEndCap(0);
		break;
	case 1: //둥글게
		index = 2; //LineCap::LineCapRound
		pMainFrm->m_dlgDrawProperty.SetEndCap(1);
		break;
	case 2: //뾰족하게
		index = 3; //LineCap::LineCapTriangle
		pMainFrm->m_dlgDrawProperty.SetEndCap(2);
		break;
	case 3: //사각형
		index = 17; //LineCap::LineCapSquareAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(3);
		break;
	case 4: //원
		index = 18; //LineCap::LineCapRoundAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(4);
		break;
	case 5: //다이아몬드
		index = 19; //LineCap::LineCapDiamondAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(5);
		break;
	case 6: //화살표
		index = 20; //LineCap::LineCapArrowAnchor
		pMainFrm->m_dlgDrawProperty.SetEndCap(6);
		break;
	}
	CDrawPropertyValue::m_nsEndCap = index;

	//뷰 영역에 외부명령임을 알리고, 라인 패턴을 바꾸는 함수를 호출
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();
}

void LinePattern::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	case 0: //평면
		*startIndex = 0;
		break;
	case 2: //둥글게
		*startIndex = 1; //LineCap::LineCapRound
		break;
	case 3: //뾰족하게
		*startIndex = 2; //LineCap::LineCapTriangle
		break;
	case 17: //사각형
		*startIndex = 3; //LineCap::LineCapSquareAnchor
		break;
	case 18: //원
		*startIndex = 4; //LineCap::LineCapRoundAnchor
		break;
	case 19: //다이아몬드
		*startIndex = 5; //LineCap::LineCapDiamondAnchor
		break;
	case 20: //화살표
		*startIndex = 6; //LineCap::LineCapArrowAnchor
		break;
	}

	switch(CDrawPropertyValue::m_nsEndCap) {
	case 0: //평면
		*endIndex = 0;
		break;
	case 2: //둥글게
		*endIndex = 1; //LineCap::LineCapRound
		break;
	case 3: //뾰족하게
		*endIndex = 2; //LineCap::LineCapTriangle
		break;
	case 17: //사각형
		*endIndex = 3; //LineCap::LineCapSquareAnchor
		break;
	case 18: //원
		*endIndex = 4; //LineCap::LineCapRoundAnchor
		break;
	case 19: //다이아몬드
		*endIndex = 5; //LineCap::LineCapDiamondAnchor
		break;
	case 20: //화살표
		*endIndex = 6; //LineCap::LineCapArrowAnchor
		break;
	}
}

//--------------------------------------------------------------------------
//　메시지를 가로채 처리
//--------------------------------------------------------------------------
BOOL LinePattern::PreTranslateMessage(MSG* pMsg)
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

	if(this->m_pCtrlToolTip && this->m_pCtrlToolTip.m_hWnd) {

		// 툴팁 보여줌
		this->m_pCtrlToolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------
//　다이얼로그 종료시 정리 작업
//--------------------------------------------------------------------------
void LinePattern::PostNcDestroy()
{
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView *)pMainFrm->GetActiveView();
	pView->m_dlgLinePtn = NULL;
	delete this;
}

//--------------------------------------------------------------------------
//　OK 버튼 시 작업
//--------------------------------------------------------------------------
void LinePattern::OnBnClickedClose()
{
	CDialog::OnOK();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//　다이얼로그가 닫혀질 경우 작업
//--------------------------------------------------------------------------
void LinePattern::OnClose()
{
	CDialog::OnClose();
	DestroyWindow();
}

//--------------------------------------------------------------------------
//　알파스핀 컨트롤 조정
//--------------------------------------------------------------------------
void LinePattern::SetAlphaSpin(void)
{
	if(m_pAlphaSpin.GetSafeHwnd())
		m_pAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);
}

//--------------------------------------------------------------------------
//　라인 패턴 컨트롤 조정
//--------------------------------------------------------------------------
void LinePattern::SetLinePattern(void)
{
	if(m_pLinePtnCmb.GetSafeHwnd()) 
		this->m_pLinePtnCmb.SetCurSel(CDrawPropertyValue::m_nsPenStyle);
}

//--------------------------------------------------------------------------
//　선 시작 모양 컨트롤 조정
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
//　선 끝모양 컨트롤 조정
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
//　라인 굵기 스핀 조정
//--------------------------------------------------------------------------
void LinePattern::SetLineThickness(void)
{
	if(m_pLineWidthSpin.GetSafeHwnd())
		m_pLineWidthSpin.SetPos(CDrawPropertyValue::m_nsThickness);
}
