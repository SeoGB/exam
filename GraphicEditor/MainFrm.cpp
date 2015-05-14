
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GraphicEditor.h"

#include "GraphicEditorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_DRAWPROPERTY_LINECOLOR, &CMainFrame::OnDrawpropertyLinecolor)
	ON_UPDATE_COMMAND_UI(ID_DRAWPROPERTY_LINECOLOR, &CMainFrame::OnUpdateDrawpropertyLinecolor)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TASK, &CMainFrame::OnUpdateIndicatorTask)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINECOLOR, &CMainFrame::OnUpdateIndicatorColor)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILLCOLOR, &CMainFrame::OnUpdateIndicatorFillcolor)
	ON_COMMAND(ID_FILE_SAVE_IMAGE, &CMainFrame::OnFileSaveImage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_TASK,
	ID_INDICATOR_LINECOLOR,
	ID_INDICATOR_FILLCOLOR
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	//////////////////////////////////////////////////////////////////////////
	// Draw Property : by SHC

	if (!m_dlgDrawProperty.Create(IDD_DRAW_PROPERTY, this))
	{
		TRACE0("그리기 속성 모음을 만들지 못했습니다.\n");
		return -1;		//생성 실패
	}

	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndMenuBar, NULL, NULL, dwStyle) || //rebar에 메뉴 붙임
		!m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, dwStyle) || //rebar에 툴바 붙임
		!m_wndReBar.AddBar(&m_dlgDrawProperty, NULL, NULL, dwStyle)) //rebar에 그리기 속성창 붙임
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	//////////////////////////////////////////////////////////////////////////
	// DrawTool Side Bar : by SHC

	if (!m_wndDrawToolBar.Create(this,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC ) ||
		!m_wndDrawToolBar.LoadToolBar(IDR_DRAWTOOL,0,0,TRUE))
	{
		TRACE0("그리기 도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strDrawToolBarName;
	bNameValid = strDrawToolBarName.LoadString(IDS_TOOLBAR_DRAW);
	ASSERT(bNameValid);
	m_wndDrawToolBar.CleanUpLockedImages();
	m_wndDrawToolBar.LoadBitmap(IDB_DRAWTOOLHI,0,0,TRUE);
	m_wndDrawToolBar.SetWindowText(strDrawToolBarName);
	m_wndDrawToolBar.SetLockedSizes(CSize(32,32), CSize(32,32));
	//////////////////////////////////////////////////////////////////////////

	// 사용자 정의 도구 모음 작업을 허용합니다.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//라인색 상태
	m_wndStatusBar.SetPaneBackgroundColor(2, CDrawPropertyValue::m_sLineColor);
	COLORREF invertColor = CDrawPropertyValue::m_sLineColor ^ 0x00FFFFFF; //색반전
	m_wndStatusBar.SetPaneTextColor(2, invertColor);
	//채우기색 상태
	m_wndStatusBar.SetPaneBackgroundColor(3, CDrawPropertyValue::m_sBrushColor);
	invertColor = CDrawPropertyValue::m_sBrushColor ^ 0x00FFFFFF; //색반전
	m_wndStatusBar.SetPaneTextColor(3, invertColor);

	//m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_TASK, SBPS_NORMAL, 100);
	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndReBar.EnableDocking(CBRS_TOP);
//	m_wndDrawToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockPane(&m_wndMenuBar);
	DockPane(&m_wndReBar);
//	DockPane(&m_wndToolBar);
	DockPane(&m_wndDrawToolBar);

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 사용자 정의 도구 모음 이미지를 로드합니다.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 메뉴 개인 설정을 활성화합니다(가장 최근에 사용한 명령).
	// TODO: 사용자의 기본 명령을 정의하여 각 풀다운 메뉴에 하나 이상의 기본 명령을 포함시킵니다.
// 	CList<UINT, UINT> lstBasicCommands;
// 
// 	lstBasicCommands.AddTail(ID_FILE_NEW);
// 	lstBasicCommands.AddTail(ID_FILE_OPEN);
// 	lstBasicCommands.AddTail(ID_FILE_SAVE);
// 	lstBasicCommands.AddTail(ID_FILE_PRINT);
// 	lstBasicCommands.AddTail(ID_APP_EXIT);
// 	lstBasicCommands.AddTail(ID_EDIT_CUT);
// 	lstBasicCommands.AddTail(ID_EDIT_PASTE);
// 	lstBasicCommands.AddTail(ID_EDIT_UNDO);
// 	lstBasicCommands.AddTail(ID_APP_ABOUT);
// 	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
// 	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
// 
// 	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnDrawpropertyLinecolor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

}


void CMainFrame::OnUpdateDrawpropertyLinecolor(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}

void CMainFrame::OnUpdateIndicatorTask(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}


void CMainFrame::OnUpdateIndicatorColor(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}


void CMainFrame::OnUpdateIndicatorFillcolor(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable();
}

//-------------------------------------------------------------------------
//　그림파일로 저장
//-------------------------------------------------------------------------
void CMainFrame::OnFileSaveImage()
{
	//파일 다이얼로그 생성
	CFileDialog dlg(FALSE, L"bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					L"Bitmap File(*.bmp) | *.bmp |JPG File(*.jpg) | *.jpg |PNG File(*.png) | *.png ||", this);

	if(dlg.DoModal() == IDOK)
	{
		//////////////////////////////////////////////////////////////////////////
		//　View 내용을 비트맵으로 만듬
		CRect clientRect;
		CGraphicEditorView* psView = (CGraphicEditorView*)GetActiveView();
		psView->GetClientRect(clientRect);
		//선택 라인을 그리지 않도록 선택 모드 해제
		BOOL bsSingleSelect = FALSE, bsMultiSelect = FALSE;
		if (psView->m_bsSelectMode) 
		{
			bsSingleSelect = TRUE;
			psView->m_bsSelectMode = FALSE;

		}
		if(psView->m_bsMultiSelectMode)
		{
			bsMultiSelect = TRUE;
			psView->m_bsMultiSelectMode = FALSE;
		}
		psView->RedrawWindow();
		CDC dcMem;
		dcMem.CreateCompatibleDC(psView->GetDC());
		CBitmap cBitmap, *pOldBitmap;
		cBitmap.CreateCompatibleBitmap(psView->GetDC(), clientRect.Width(), clientRect.Height());
		pOldBitmap = (CBitmap*)dcMem.SelectObject(cBitmap);
		dcMem.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), psView->GetDC(), 0, 0, SRCCOPY);

		//////////////////////////////////////////////////////////////////////////
		//　파일 저장
		Bitmap loadBitmap(cBitmap, NULL); //위에서 생성한 CBitmap을 GDI+에서 사용하는 Bitmap 객체로 바꿈
		CLSID imgClsid; //이미지 코덱 정보
		switch(dlg.m_ofn.nFilterIndex)
		{
		case 1:	//BMP에 대한 이미지 코덱의 CLSID를 받아옴
			GetEncoderClsid(L"image/bmp", &imgClsid);
			break;
		case 2: //JPG에 대한 이미지 코덱의 CLSID를 받아옴
			GetEncoderClsid(L"image/jpeg", &imgClsid);
			break;
		case 3: //PNG에 대한 이미지 코덱의 CLSID를 받아옴
			GetEncoderClsid(L"image/png", &imgClsid);
			break;
		}
		//그림파일 저장 시 파라미터
		int nQuality = 100;
		EncoderParameters param;
		param.Count                       = 1; 
		param.Parameter[0].Guid           = EncoderQuality;
		param.Parameter[0].Type           = EncoderParameterValueTypeLong;
		param.Parameter[0].NumberOfValues = 1;
		param.Parameter[0].Value          = &nQuality;

		//그림 파일 저장
		CString fileName = dlg.GetPathName();
		loadBitmap.Save(fileName,&imgClsid, &param);

		//////////////////////////////////////////////////////////////////////////
		//선택 라인이 해제되었다면 다시 활성화
		if (bsSingleSelect) 
		{
			bsSingleSelect = FALSE;
			psView->m_bsSelectMode = TRUE;

		}
		if(bsMultiSelect)
		{
			bsMultiSelect = FALSE;
			psView->m_bsMultiSelectMode = TRUE;
		}
	}
}

//이미지 코덱의 CLSID를 가져오는 함수
BOOL CMainFrame::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;	// 이미지 엔코더 번호
	UINT  size = 0;	// 이미지 엔코더 배열의 사이즈
 
	GetImageEncodersSize(&num, &size); //이미지 엔코더의 사이즈를 구함
	if(size == 0) return FALSE;

	ImageCodecInfo* pImageCodecInfo = NULL;
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size)); //메모리 할당
	if(pImageCodecInfo == NULL) return FALSE;

	GetImageEncoders(num, size, pImageCodecInfo); //코덱정보를 구함
	for(UINT i=0; i < num; ++i)
	{
		//코덱 정보와 전달받은 포맷이 동일할 경우
		if(wcscmp(pImageCodecInfo[i].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[i].Clsid; //CLSID를 지정
			free(pImageCodecInfo); //메모리 해제
			return TRUE;
		}    
	}
	free(pImageCodecInfo);
	return FALSE;
}