/* [ CDrawProperty 클래스 by 신희철 ]
** CDrawProperty 클래스의 구현부 입니다.
*/

// DrawProperty.cpp : implementation file
//

#include "stdafx.h"
#include "GraphicEditor.h"
#include "DrawProperty.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"
#include "GraphicEditorView.h"
#include "GraphicEditorDoc.h"
#include "MainFrm.h"

// CDrawProperty dialog

IMPLEMENT_DYNAMIC(CDrawProperty, CDialog)

//--------------------------------------------------------------------------
//　생성자 및 소멸자
//--------------------------------------------------------------------------
CDrawProperty::CDrawProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawProperty::IDD, pParent)
	, m_nsFaceType(0)
	, m_nsBrushType(1)
{
}

CDrawProperty::~CDrawProperty()
{
}

//--------------------------------------------------------------------------
//　DDX/DDV 지원
//--------------------------------------------------------------------------
void CDrawProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_sLineWeightSpin);
	DDX_Control(pDX, IDC_BUTTON_FILLCOLOR, m_sFillColorBtn);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_sFontBtn);
	DDX_Control(pDX, IDC_BUTTON_LINECOLOR, m_sLineColorBtn);
	DDX_Control(pDX, IDC_EDIT_LINEWEIGHT, m_sThicknessEdit);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_sStcFont);
	DDX_Control(pDX, IDC_STATIC_TEXT2, m_sStcThickness);
	DDX_Control(pDX, IDC_STATIC_TEXT3, m_sStcLinePtn);
	DDX_Control(pDX, IDC_STATIC_TEXT4, m_sStcFillPtn);
	DDX_Control(pDX, IDC_LINEPATTERN, m_sLinePtnCombo);
	DDX_Control(pDX, IDC_FILLPATTERN, m_sFillPtnCombo);
	DDX_Control(pDX, IDC_FILLTYPE_SOLID, m_sFillTypeSolid);
	DDX_Control(pDX, IDC_FILLTYPE_HATCH, m_sFillTypeHatch);
	DDX_Control(pDX, IDC_FILLTYPE_GRADIENT, m_sFillTypeGradient);
	DDX_Control(pDX, IDC_FACE_OUTLINED, m_sFaceTypeOutline);
	DDX_Control(pDX, IDC_FACE_FILL, m_sFaceTypeOutlinedFill);
	DDX_Control(pDX, IDC_FACE_ONLYFILL, m_sFaceTypeFillOnly);
	DDX_Control(pDX, IDC_SPIN2, m_sAlphaSpin);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_sAlphaEdit);

	DDX_Radio(pDX, IDC_FACE_OUTLINED, m_nsFaceType);
	DDX_Radio(pDX, IDC_FILLTYPE_SOLID, m_nsBrushType);
	DDX_Control(pDX, IDC_COMBO_ENDCAP, m_sEndCapCombo);
	DDX_Control(pDX, IDC_COMBO_GRADIENT, m_sGradientCombo);
	DDX_Control(pDX, IDC_COMBO_LINEJOIN, m_sLineJoinCombo);
	DDX_Control(pDX, IDC_COMBO_STARTCAP, m_sStartCapCombo);
	DDX_Control(pDX, IDC_BUTTON_FILLCOLOR2, m_sGradientEndColorBtn);
	DDX_Control(pDX, IDC_STATIC_TEXT5, m_sStcOpacity);
	DDX_Control(pDX, IDC_STATIC_TEXT6, m_sStcEndCap);
	DDX_Control(pDX, IDC_STATIC_TEXT7, m_sStcStartCap);
	DDX_Control(pDX, IDC_STATIC_TEXT8, m_sStcGradientMode);
	DDX_Control(pDX, IDC_STATIC_TEXT9, m_sStcLineJoin);
}

//--------------------------------------------------------------------------
//　메시지맵
//--------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawProperty, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CDrawProperty::OnBnClickedButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_LINECOLOR, &CDrawProperty::OnBnClickedButtonFgcolor)
	ON_BN_CLICKED(IDC_BUTTON_FILLCOLOR, &CDrawProperty::OnBnClickedButtonFillcolor)
	ON_WM_DRAWITEM()
	ON_EN_CHANGE(IDC_EDIT_LINEWEIGHT, &CDrawProperty::OnEnChangeEditLineweight)
	ON_CBN_SELCHANGE(IDC_LINEPATTERN, &CDrawProperty::OnSelchangeLinepattern)
	ON_CBN_SELCHANGE(IDC_FILLPATTERN, &CDrawProperty::OnSelchangeFillpattern)
	ON_BN_CLICKED(IDC_FACE_OUTLINED, &CDrawProperty::OnClickedFaceType)
	ON_BN_CLICKED(IDC_FILLTYPE_SOLID, &CDrawProperty::OnClickedBrushType)
	ON_COMMAND(IDC_FACE_FILL, &CDrawProperty::OnFaceFill)
	ON_COMMAND(IDC_FACE_ONLYFILL, &CDrawProperty::OnFaceOnlyfill)
	ON_COMMAND(IDC_FILLTYPE_HATCH, &CDrawProperty::OnFilltypeHatch)
	ON_COMMAND(IDC_FILLTYPE_GRADIENT, &CDrawProperty::OnFilltypeGradient)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, &CDrawProperty::OnChangeEditAlpha)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTCAP, &CDrawProperty::OnCbnSelchangeComboStartcap)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDCAP, &CDrawProperty::OnCbnSelchangeComboEndcap)
	ON_CBN_SELCHANGE(IDC_COMBO_GRADIENT, &CDrawProperty::OnCbnSelchangeComboGradient)
	ON_BN_CLICKED(IDC_BUTTON_FILLCOLOR2, &CDrawProperty::OnBnClickedButtonFillcolor2)
	ON_CBN_SELCHANGE(IDC_COMBO_LINEJOIN, &CDrawProperty::OnCbnSelchangeComboLinejoin)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//--------------------------------------------------------------------------
//　엔터나 esc가 입력되도 다이얼로그가 꺼지지 않도록함
//--------------------------------------------------------------------------
BOOL CDrawProperty::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}

	if(m_sCtrlToolTip && m_sCtrlToolTip.m_hWnd)
		m_sCtrlToolTip.RelayEvent(pMsg); //툴팁 출력

	return CDialog::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------
//　다이얼로그 초기화
//--------------------------------------------------------------------------
BOOL CDrawProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	//라인 굵기 스핀 컨트롤 초기화
	m_sLineWeightSpin.SetRange(1,30);	//최소 두께 1~최대 두께 30
	m_sLineWeightSpin.SetPos(1);		//기본 값 1
	
	//////////////////////////////////////////////////////////////////////////
	//알파값 스핀 컨트롤 초기화
	m_sAlphaSpin.SetRange(1, 100);	//최소 불투명도 1% ~ 최대 불투명도 100%
	m_sAlphaSpin.SetPos(100);		//기본값 100

	//////////////////////////////////////////////////////////////////////////
	//　라인 패턴 콤보 박스 초기화
	for(int i=0; i<5; i++)
		m_sLinePtnCombo.AddItem(i, LINEPATTERN);
	m_sLinePtnCombo.SetCurSel(0);

	//////////////////////////////////////////////////////////////////////////
	//브러쉬 패턴 콤보 박스 초기화
	for(int i=0; i<52; i++)
		m_sFillPtnCombo.AddItem(i, HATCHPATTERN);
	m_sFillPtnCombo.SetCurSel(0);
	
	//////////////////////////////////////////////////////////////////////////
	//선 시작-끝 모양 콤보박스 초기화
	m_sStartCapCombo.SetCurSel(1);
	m_sEndCapCombo.SetCurSel(1);

	//////////////////////////////////////////////////////////////////////////
	//외곽선 모양 콤보 박스 초기화
	m_sLineJoinCombo.SetCurSel(0);

	//////////////////////////////////////////////////////////////////////////
	// 그라디언트 콤보 박스 초기화
	for(int i=0; i<4; i++)
		m_sGradientCombo.AddItem(i, GRADIENTPATTERN);
	m_sGradientCombo.SetCurSel(0);

	//////////////////////////////////////////////////////////////////////////
	// 면 타입 라디오 버튼 초기화
	m_sFaceTypeOutlinedFill.SetCheck(1);
	m_sFaceTypeOutline.SetCheck(0);

	m_sFillTypeSolid.SetCheck(1);
	m_sFillTypeHatch.SetCheck(0);
	m_sFillTypeGradient.SetCheck(0);

	m_sGradientEndColorBtn.EnableWindow(FALSE);


	//////////////////////////////////////////////////////////////////////////
	//　윈도우 비스타 이상에서는 라디오버튼에 XP 테마 적용이 제대로 되지 않는 문제 해결을 위해 추가
	OSVERSIONINFO sInfo;
	sInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	// 윈도우 버전 정보를 구해 비스타 이상일 경우 아래 내용 적용
	if(::GetVersionEx(&sInfo) && (sInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && sInfo.dwMajorVersion >= 6 && sInfo.dwMinorVersion >= 0))
	{
		SetWindowTheme(GetDlgItem(IDC_FACE_FILL)->GetSafeHwnd(), L"", L"");
		SetWindowTheme(GetDlgItem(IDC_FACE_ONLYFILL)->GetSafeHwnd(), L"", L"");
		SetWindowTheme(GetDlgItem(IDC_FACE_OUTLINED)->GetSafeHwnd(), L"", L"");
		SetWindowTheme(GetDlgItem(IDC_FILLTYPE_GRADIENT)->GetSafeHwnd(), L"", L"");
		SetWindowTheme(GetDlgItem(IDC_FILLTYPE_HATCH)->GetSafeHwnd(), L"", L"");
		SetWindowTheme(GetDlgItem(IDC_FILLTYPE_SOLID)->GetSafeHwnd(), L"", L"");
	}

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
	m_sCtrlToolTip.SetParams(&params);

	m_sCtrlToolTip.Create(this);
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_FONT), L"폰트");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_LINECOLOR), L"라인색(텍스트색)");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_FILLCOLOR), L"채우기색");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_BUTTON_FILLCOLOR2), L"그라디언트 채우기색");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FACE_OUTLINED), L"외곽선만 그리기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FACE_FILL), L"외곽선+채우기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FACE_ONLYFILL), L"내부만 채우기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_SOLID), L"단색으로 채우기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_HATCH), L"격자무늬 채우기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FILLTYPE_GRADIENT), L"그라디언트 채우기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_STARTCAP), L"라인 시작 모양");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_ENDCAP),L"라인 끝 모양");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_LINEWEIGHT), L"라인 굵기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_SPIN1), L"라인 굵기");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_LINEPATTERN), L"라인 패턴");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_LINEJOIN), L"외곽선 처리");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_FILLPATTERN), L"채우기 패턴");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_EDIT_ALPHA), L"불투명도");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_SPIN2), L"불투명도");
	m_sCtrlToolTip.AddTool(GetDlgItem(IDC_COMBO_GRADIENT), L"그라디언트 패턴");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDrawProperty message handlers
//--------------------------------------------------------------------------
//　폰트 정보 지정
//--------------------------------------------------------------------------
void CDrawProperty::OnBnClickedButtonFont()
{
	CClientDC dc(this);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(10, dc.GetDeviceCaps(LOGPIXELSY), 72);
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("굴림"));

	//폰트 공용 다이얼로그에서 색지정과 효과를 제외
	CFontDialog dlg(&lf, CF_SCREENFONTS, NULL, NULL);
	dlg.m_cf.Flags &= ~CF_EFFECTS;

	if (dlg.DoModal() == IDOK) //폰트 다이얼로그를 띄워 확인 버튼을 눌렀을 경우
	{
		dlg.GetCurrentFont(&CDrawPropertyValue::m_sLogFont);	//폰트 지정
		CDrawPropertyValue::m_nsFontSize = dlg.GetSize() / 10;	//사이즈 지정
	}
	//뷰 영역에 외부명령임을 알리고, 폰트 정보를 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyFont();
}

//--------------------------------------------------------------------------
//　라인 색
//--------------------------------------------------------------------------
void CDrawProperty::OnBnClickedButtonFgcolor()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CDrawPropertyValue::m_sLineColor = dlg.GetColor(); //라인색 지정
		Invalidate();
	}
	//뷰 영역에 라인컬러를 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINECOLOR);
}

//--------------------------------------------------------------------------
//채우기 색
//--------------------------------------------------------------------------
void CDrawProperty::OnBnClickedButtonFillcolor()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK) 
	{
		CDrawPropertyValue::m_sBrushColor = dlg.GetColor(); //채우기 색 지정
		Invalidate(TRUE); //바꾼 색으로 버튼 색 변경
	}
	//뷰 영역에 브러쉬 컬러(채우기색)를 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGEFILLCOLOR);
}

//--------------------------------------------------------------------------
//　버튼 색을 동적으로 변하게하는 함수
//--------------------------------------------------------------------------
void CDrawProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UpdateData();
	CDC dc;
	CRect rect;
	UINT state;
	
	//////////////////////////////////////////////////////////////////////////
	// 버튼 그리기 작업 초기화
	if (nIDCtl == IDC_BUTTON_LINECOLOR || nIDCtl == IDC_BUTTON_FILLCOLOR || nIDCtl == IDC_BUTTON_FILLCOLOR2)
	{
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		state = lpDrawItemStruct->itemState;	
	}

	switch (nIDCtl)
	{
	//////////////////////////////////////////////////////////////////////////
	// 라인색 버튼의 색 지정
	case IDC_BUTTON_LINECOLOR:
		{
			dc.Draw3dRect(&rect, RGB(255,255,255), RGB(0,0,0));
			if(state & ODS_DISABLED)
				dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE)); //비활성화색
			else
				dc.FillSolidRect(&rect, CDrawPropertyValue::m_sLineColor); //버튼 색 지정
				

			if(state & ODS_SELECTED)
				dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
			else
				dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

			COLORREF invertColor = CDrawPropertyValue::m_sLineColor ^ 0x00FFFFFF; //색반전
			dc.SetTextColor(invertColor);
			
			CString str = L"Line\n(Text)";
			CRect textRect = rect;
			textRect.top += textRect.CenterPoint().y/2;
			dc.DrawText(str, textRect,  DT_CENTER | DT_VCENTER | BS_MULTILINE | DT_NOCLIP);
			break;
		}
	//////////////////////////////////////////////////////////////////////////
	// 채우기 색 버튼의 색 지정
	case IDC_BUTTON_FILLCOLOR:
		{
			dc.Draw3dRect(&rect, RGB(255,255,255), RGB(0,0,0));

			if(state & ODS_DISABLED)
				dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE)); //비활성화색				
			else
				dc.FillSolidRect(&rect, CDrawPropertyValue::m_sBrushColor); //버튼 색 지정


			if(state & ODS_SELECTED)
				dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
			else
				dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

			COLORREF invertColor = CDrawPropertyValue::m_sBrushColor ^ 0x00FFFFFF; //색반전
			if(state & ODS_DISABLED)
				invertColor = ::GetSysColor(COLOR_GRAYTEXT);
			dc.SetTextColor(invertColor);

			CString str = L"Fill";
			if(state & ODS_DISABLED)
				str.SetString(L"Not Use");
			dc.DrawText(str, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			break;
		}
	//////////////////////////////////////////////////////////////////////////
	// 그라디언트 끝 색 버튼의 색 지정
	case IDC_BUTTON_FILLCOLOR2:
		{
			dc.Draw3dRect(&rect, RGB(255,255,255), RGB(0,0,0));

			if(state & ODS_DISABLED)
				dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE)); //비활성화색
			else
				dc.FillSolidRect(&rect, CDrawPropertyValue::m_sGradientEndColor); //버튼 색 지정

			if(state & ODS_SELECTED)
				dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
			else
				dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

			COLORREF invertColor = CDrawPropertyValue::m_sGradientEndColor ^ 0x00FFFFFF; //색반전
			if(state & ODS_DISABLED)
				invertColor = ::GetSysColor(COLOR_GRAYTEXT);
			dc.SetTextColor(invertColor);

			CString str = L"Grd End";
			if(state & ODS_DISABLED)
				str.SetString(L"Not Use");
			dc.DrawText(str, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			break;
		}
	//////////////////////////////////////////////////////////////////////////
	// 라인 패턴 그리기
	case IDC_LINEPATTERN:
		if(m_sLinePtnCombo.GetSafeHwnd())
			m_sLinePtnCombo.DrawItem(lpDrawItemStruct);
		break;
	//////////////////////////////////////////////////////////////////////////
	// 채우기 패턴(HATCH STYLE) 그리기
	case IDC_FILLPATTERN:
		if(m_sFillPtnCombo.GetSafeHwnd())
			m_sFillPtnCombo.DrawItem(lpDrawItemStruct);
		break;
	//////////////////////////////////////////////////////////////////////////
	//　그라디언트 패턴 그리기
	case IDC_COMBO_GRADIENT:
		if(m_sGradientCombo.GetSafeHwnd())
			m_sGradientCombo.DrawItem(lpDrawItemStruct);
		break;
	}
	dc.Detach();
//	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//--------------------------------------------------------------------------
//라인 굵기 지정
//--------------------------------------------------------------------------
void CDrawProperty::OnEnChangeEditLineweight()
{
	if(m_sLineWeightSpin.GetSafeHwnd())
		CDrawPropertyValue::m_nsThickness = m_sLineWeightSpin.GetPos();

	if(m_sLineWeightSpin.GetSafeOwner())
	{
		//뷰 영역에 외부명령임을 알리고, 라인 굵기를 바꾸는 함수를 호출
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGELINEWEIGHT);

		//모달리스 다이얼로그들과 동기화 작업
		if(pView->m_dlgLinePtn != NULL)
			pView->m_dlgLinePtn->SetLineThickness();
	}
}
//--------------------------------------------------------------------------
//　불투명도 지정
//--------------------------------------------------------------------------
void CDrawProperty::OnChangeEditAlpha()
{
	if(m_sAlphaSpin.GetSafeHwnd())
		CDrawPropertyValue::m_nsAlpha = m_sAlphaSpin.GetPos();

	if(m_sAlphaSpin.GetSafeOwner())
	{
		//뷰 영역에 불투명도를 바꾸는 메시지를 날림
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
		pView->SendMessage(UM_CHANGEALPHA);

		//모달리스 다이얼로그들과 동기화 작업
		if(pView->m_dlgLinePtn != NULL)
			pView->m_dlgLinePtn->SetAlphaSpin();
		if(pView->m_dlgBrushPtn != NULL)
			pView->m_dlgBrushPtn->SetAlphaSpin();
	}
}

//--------------------------------------------------------------------------
//펜 스타일 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnSelchangeLinepattern()
{
	CDrawPropertyValue::m_nsPenStyle  = m_sLinePtnCombo.GetCurSel();
	//뷰 영역에 외부명령임을 알리고, 라인 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyLinepattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgLinePtn != NULL)
		pView->m_dlgLinePtn->SetLinePattern();
}

//--------------------------------------------------------------------------
//해치 스타일 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnSelchangeFillpattern()
{
	CDrawPropertyValue::m_nsHatchStyle = m_sFillPtnCombo.GetCurSel();

	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetHatchPattern();
}

//--------------------------------------------------------------------------
//　라인 굵기를 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetLineWeightSpin(void)
{
	if(m_sLineWeightSpin.GetSafeHwnd())
		m_sLineWeightSpin.SetPos(CDrawPropertyValue::m_nsThickness);
}

//--------------------------------------------------------------------------
//　투명도 지정
//--------------------------------------------------------------------------
void CDrawProperty::SetAlphaSpin(void)
{
	if(m_sAlphaSpin.GetSafeHwnd())
		m_sAlphaSpin.SetPos(CDrawPropertyValue::m_nsAlpha);

	//모달리스 다이얼로그들과 동기화 작업
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetAlphaSpin();
	if(pView->m_dlgLinePtn != NULL)
		pView->m_dlgLinePtn->SetAlphaSpin();
}

//--------------------------------------------------------------------------
//　라인 스타일을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetLinePattern(void) {
	if(this->m_sLinePtnCombo.GetSafeHwnd()) {
		this->m_sLinePtnCombo.SetCurSel(CDrawPropertyValue::m_nsPenStyle);
	}
}

//--------------------------------------------------------------------------
//　해치 스타일을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetHatchPattern(void) {
	if(this->m_sFillPtnCombo.GetSafeHwnd()) {
		this->m_sFillPtnCombo.SetCurSel(CDrawPropertyValue::m_nsHatchStyle);
	}
}

//--------------------------------------------------------------------------
//　라인 시작모양을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetStartCap(int startCap, BOOL isPropValue) {
	if(isPropValue == FALSE)
	{
		if(this->m_sStartCapCombo.GetSafeHwnd()) {
			this->m_sStartCapCombo.SetCurSel(startCap);
		}
	}
	else
	{
		int index;
		switch(startCap)
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
		if(this->m_sStartCapCombo.GetSafeHwnd()) {
			this->m_sStartCapCombo.SetCurSel(index);
		}
	}
}

//--------------------------------------------------------------------------
//　라인 끝모양을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetEndCap(int endCap, BOOL isPropValue) {
	if(isPropValue == FALSE)
	{
		if(this->m_sEndCapCombo.GetSafeHwnd()) {
			this->m_sEndCapCombo.SetCurSel(endCap);
		}
	}
	else
	{
		int index;
		switch(endCap)
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
		if(this->m_sEndCapCombo.GetSafeHwnd()) {
			this->m_sEndCapCombo.SetCurSel(index);
		}
	}
}

//--------------------------------------------------------------------------
//　외곽선 모양을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetLineJoin(int lineJoin) {
	if(this->m_sLineJoinCombo.GetSafeHwnd()) {
		this->m_sLineJoinCombo.SetCurSel(lineJoin);
	}
}

//--------------------------------------------------------------------------
//　그라디언트 끝색을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetGradientPattern(int grdPattern) {
	if(this->m_sGradientCombo.GetSafeHwnd()){
		this->m_sGradientCombo.SetCurSel(grdPattern);
	}

}
//--------------------------------------------------------------------------
//　그리기 모양을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetDrawStyle(int style) {

	switch(style) {
	case 0 :
		this->m_sFaceTypeOutline.SetCheck(1);
		this->m_sFaceTypeOutlinedFill.SetCheck(0);
		this->m_sFaceTypeFillOnly.SetCheck(0);
		m_sFillTypeGradient.EnableWindow(FALSE);
		m_sFillTypeHatch.EnableWindow(FALSE);
		m_sFillTypeSolid.EnableWindow(FALSE);
		break;

	case 1 :
		this->m_sFaceTypeOutline.SetCheck(0);
		this->m_sFaceTypeOutlinedFill.SetCheck(1);
		this->m_sFaceTypeFillOnly.SetCheck(0);
		m_sFillTypeGradient.EnableWindow();
		m_sFillTypeHatch.EnableWindow();
		m_sFillTypeSolid.EnableWindow();
		break;

	case 2 :
		this->m_sFaceTypeOutline.SetCheck(0);
		this->m_sFaceTypeOutlinedFill.SetCheck(0);
		this->m_sFaceTypeFillOnly.SetCheck(1);
		m_sFillTypeGradient.EnableWindow();
		m_sFillTypeHatch.EnableWindow();
		m_sFillTypeSolid.EnableWindow();
		break;
	}
}

//--------------------------------------------------------------------------
//　채우기 스타일을 읽어와 해당 컨트롤의 상태 변경
//--------------------------------------------------------------------------
void CDrawProperty::SetFillStyle(int fillStyle) {

	m_nsBrushType = fillStyle;

	switch(fillStyle) {
	case 0 :
		this->m_sFillTypeSolid.SetCheck(1);
		this->m_sFillTypeHatch.SetCheck(0);
		this->m_sFillTypeGradient.SetCheck(0);
		this->m_sGradientEndColorBtn.EnableWindow(FALSE);
		break;

	case 1 :
		this->m_sFillTypeSolid.SetCheck(0);
		this->m_sFillTypeHatch.SetCheck(1);
		this->m_sFillTypeGradient.SetCheck(0);
		this->m_sGradientEndColorBtn.EnableWindow(FALSE);
		this->OnFilltypeHatch();
		break;

	case 2 :
		this->m_sFillTypeSolid.SetCheck(0);
		this->m_sFillTypeHatch.SetCheck(0);
		this->m_sFillTypeGradient.SetCheck(1);
		this->m_sGradientEndColorBtn.EnableWindow(TRUE);
		this->OnFilltypeGradient();
		break;
	}
}

//--------------------------------------------------------------------------
//면 타입 설정
//--------------------------------------------------------------------------
//외곽선만 그리기
void CDrawProperty::OnClickedFaceType()
{
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = m_nsFaceType;

	//////////////////////////////////////////////////////////////////////////
	//외곽선만 그릴 경우 컨트롤들의 비활성화
	m_sFillPtnCombo.EnableWindow(FALSE); //채우기 패턴(해치 타입) 비활성화
	//브러쉬(채우기)타입 라디오버튼 비활성화
	m_sFillTypeSolid.EnableWindow(FALSE);
	m_sFillTypeHatch.EnableWindow(FALSE);
	m_sFillTypeGradient.EnableWindow(FALSE);
	m_sLineJoinCombo.EnableWindow();

	m_sFillPtnCombo.EnableWindow(FALSE);//Hatch 패턴 콤보 박스 비활성화
	m_sGradientCombo.EnableWindow(FALSE); //그라디언트 콤보 박스 비활성화
	m_sFillColorBtn.EnableWindow(FALSE); //채우기색 버튼 비활성화
	m_sGradientEndColorBtn.EnableWindow(FALSE); //그라디언트 끝 색 버튼 비활성화

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetDrawStyle();
}
//외곽선+채우기
void CDrawProperty::OnFaceFill()
{
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = m_nsFaceType;

	//////////////////////////////////////////////////////////////////////////
	//컨트롤 활성화/비활성화
	m_sFillPtnCombo.EnableWindow();
	m_sFillTypeSolid.EnableWindow();
	m_sFillTypeHatch.EnableWindow();
	m_sFillTypeGradient.EnableWindow();
	m_sLineJoinCombo.EnableWindow();

	AdjustControl();
	
	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetDrawStyle();
}
//외곽선은 그리지 않고 채우기만하기
void CDrawProperty::OnFaceOnlyfill()
{
	UpdateData();
	CDrawPropertyValue::m_nsFaceType = m_nsFaceType;

	//////////////////////////////////////////////////////////////////////////
	//컨트롤 활성화/비활성화
	m_sFillPtnCombo.EnableWindow();
	m_sFillTypeSolid.EnableWindow();
	m_sFillTypeHatch.EnableWindow();
	m_sFillTypeGradient.EnableWindow();
	m_sLineJoinCombo.EnableWindow(FALSE);

	AdjustControl();

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetDrawStyle();
}
//--------------------------------------------------------------------------
//브러쉬 스타일 설정
//--------------------------------------------------------------------------
//SOLID 브러쉬
void CDrawProperty::OnClickedBrushType()
{
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = m_nsBrushType;

	//컨트롤 비활성화
	m_sFillPtnCombo.EnableWindow(FALSE);
	m_sGradientCombo.EnableWindow(FALSE);
	m_sGradientEndColorBtn.EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetFillStyle();
}
//HATCH 브러쉬
void CDrawProperty::OnFilltypeHatch()
{
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = m_nsBrushType;

	//컨트롤 활성화/비활성화
	m_sFillPtnCombo.EnableWindow();
	m_sGradientCombo.EnableWindow(FALSE);
	m_sGradientEndColorBtn.EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetFillStyle();
}
//GRADIENT 브러쉬
void CDrawProperty::OnFilltypeGradient()
{
	UpdateData();
	CDrawPropertyValue::m_nsBrushStyle = m_nsBrushType;

	//컨트롤 활성화/비활성화
	m_sFillPtnCombo.EnableWindow(FALSE);
	m_sGradientCombo.EnableWindow();
	m_sGradientEndColorBtn.EnableWindow();

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetFillStyle();
}


//--------------------------------------------------------------------------
//　선 시작 모양 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnCbnSelchangeComboStartcap()
{
	//평면;둥글게;뾰족하게;사각형;원;다이아몬드;화살표
	int index;
	switch(m_sStartCapCombo.GetCurSel())
	{
	case 0: //평면
		index = 0; //LineCap::LineCapFlat;
		break;
	case 1: //둥글게
		index = 2; //LineCap::LineCapRound
		break;
	case 2: //뾰족하게
		index = 3; //LineCap::LineCapTriangle
		break;
	case 3: //사각형
		index = 17; //LineCap::LineCapSquareAnchor
		break;
	case 4: //원
		index = 18; //LineCap::LineCapRoundAnchor
		break;
	case 5: //다이아몬드
		index = 19; //LineCap::LineCapDiamondAnchor
		break;
	case 6: //화살표
		index = 20; //LineCap::LineCapArrowAnchor
		break;
	}
	CDrawPropertyValue::m_nsStartCap = index;

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 라인 시작모양을 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINECAPSTART);

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgLinePtn != NULL)
		pView->m_dlgLinePtn->SetStartCap();
}

//--------------------------------------------------------------------------
//　선 끝 모양 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnCbnSelchangeComboEndcap()
{
	//평면;둥글게;뾰족하게;사각형;원;다이아몬드;화살표
	int index;
	switch(m_sEndCapCombo.GetCurSel())
	{
	case 0: //평면
		index = 0; //LineCap::LineCapFlat;
		break;
	case 1: //둥글게
		index = 2; //LineCap::LineCapRound
		break;
	case 2: //뾰족하게
		index = 3; //LineCap::LineCapTriangle
		break;
	case 3: //사각형
		index = 17; //LineCap::LineCapSquareAnchor
		break;
	case 4: //원
		index = 18; //LineCap::LineCapRoundAnchor
		break;
	case 5: //다이아몬드
		index = 19; //LineCap::LineCapDiamondAnchor
		break;
	case 6: //화살표
		index = 20; //LineCap::LineCapArrowAnchor
		break;
	}
	CDrawPropertyValue::m_nsEndCap = index;

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 라인 시작모양을 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINECAPEND);

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgLinePtn != NULL)
		pView->m_dlgLinePtn->SetEndCap();
}

//--------------------------------------------------------------------------
//　그라디언트 모양 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnCbnSelchangeComboGradient()
{
	CDrawPropertyValue::m_nsGradientType = m_sGradientCombo.GetCurSel();

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외부명령임을 알리고, 브러쉬 패턴을 바꾸는 함수를 호출
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->m_bExternalCmd = TRUE;
	pView->OnDrawpropertyBrushpattern();

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetGradientPattern();
}

//--------------------------------------------------------------------------
//　그라디언트 끝 색 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnBnClickedButtonFillcolor2()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK) 
	{
		CDrawPropertyValue::m_sGradientEndColor = dlg.GetColor(); //채우기 색 지정
		Invalidate(TRUE); //바꾼 색으로 버튼 색 변경
	}
	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 그라디언트 컬러를 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGEGRDTCOLOR);
}

//--------------------------------------------------------------------------
//　라인조인 설정
//--------------------------------------------------------------------------
void CDrawProperty::OnCbnSelchangeComboLinejoin()
{
	CDrawPropertyValue::m_nsLineJoin = m_sLineJoinCombo.GetCurSel();

	//////////////////////////////////////////////////////////////////////////
	//뷰 영역에 외곽선 형태를 바꾸는 메시지를 날림
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CGraphicEditorView* pView = (CGraphicEditorView*)pMainFrm->GetActiveView();
	pView->SendMessage(UM_CHANGELINEJOIN);

	//모달리스 다이얼로그들과 동기화 작업
	if(pView->m_dlgBrushPtn != NULL)
		pView->m_dlgBrushPtn->SetLineJoin();
}

//--------------------------------------------------------------------------
//　컨트롤들의 활성화/비활성화 조정
//--------------------------------------------------------------------------
void CDrawProperty::AdjustControl(void)
{
	switch(CDrawPropertyValue::m_nsBrushStyle)
	{
	case 0:
		m_sStcFillPtn.EnableWindow(FALSE);
		m_sFillPtnCombo.EnableWindow(FALSE);
		m_sStcGradientMode.EnableWindow(FALSE);
		m_sGradientCombo.EnableWindow(FALSE);
		m_sGradientEndColorBtn.EnableWindow(FALSE);
		break;
	case 1:
		m_sStcFillPtn.EnableWindow();
		m_sFillPtnCombo.EnableWindow();
		m_sStcGradientMode.EnableWindow(FALSE);
		m_sGradientCombo.EnableWindow(FALSE);
		m_sGradientEndColorBtn.EnableWindow(FALSE);
		break;
	case 2:
		m_sStcFillPtn.EnableWindow(FALSE);
		m_sFillPtnCombo.EnableWindow(FALSE);
		m_sStcGradientMode.EnableWindow();
		m_sGradientCombo.EnableWindow();
		m_sGradientEndColorBtn.EnableWindow();
		break;
	}
	if(CDrawPropertyValue::m_nsFaceType == 0)
		m_sFillColorBtn.EnableWindow(FALSE);
	else
		m_sFillColorBtn.EnableWindow();
}

//--------------------------------------------------------------------------
//　다이얼로그의 배경을 VS2008 스타일로 그림
//--------------------------------------------------------------------------
BOOL CDrawProperty::OnEraseBkgnd(CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);
	CRect rect;
	GetClientRect(rect);
	Rect barRect(rect.left, rect.top, rect.Width(), rect.Height());

	OSVERSIONINFO sInfo;
	sInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(::GetVersionEx(&sInfo))
	{
		//비스타 이상 버전일 경우
		if(sInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && sInfo.dwMajorVersion >= 6 && sInfo.dwMinorVersion >= 0)
		{ 
			SolidBrush sSolidBrush(Color(211,218,237));
			graphics.FillRectangle(&sSolidBrush, barRect);
		}
		//비스타 하위 버전일 경우
		else
		{
			Color gradientColor(228,226,231); //그라디언트 끝 색
			LinearGradientBrush sGradientBrush(barRect, Color(248,248,248), gradientColor, LinearGradientModeHorizontal);	//그라디언트 브러쉬
			graphics.FillRectangle(&sGradientBrush, barRect);
		}
	}
	return TRUE;
}

//--------------------------------------------------------------------------
//　static 컨트롤 및 윈도우 비스타 이상에서는 라디오버튼들의 배경을 투명하게 설정
//--------------------------------------------------------------------------
HBRUSH CDrawProperty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush brush;
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC: //정적 컨트롤 배경 투명하게
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	//윈도우 비스타 이상일 경우 라디오버튼을 투명하게 만듦
	OSVERSIONINFO sInfo;
	sInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(::GetVersionEx(&sInfo) && (sInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && sInfo.dwMajorVersion >= 6 && sInfo.dwMinorVersion >= 0))
	{
		switch(GetDlgCtrlID())
		{
		case IDC_FACE_FILL:
		case IDC_FACE_ONLYFILL:
		case IDC_FACE_OUTLINED:
		case IDC_FILLTYPE_GRADIENT:
		case IDC_FILLTYPE_HATCH:
		case IDC_FILLTYPE_SOLID:
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}