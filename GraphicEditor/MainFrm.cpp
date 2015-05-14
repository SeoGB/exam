
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_TASK,
	ID_INDICATOR_LINECOLOR,
	ID_INDICATOR_FILLCOLOR
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// ��� ����� �������̽� ��Ҹ� �׸��� �� ����ϴ� ���־� �����ڸ� �����մϴ�.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
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
		TRACE0("�׸��� �Ӽ� ������ ������ ���߽��ϴ�.\n");
		return -1;		//���� ����
	}

	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndMenuBar, NULL, NULL, dwStyle) || //rebar�� �޴� ����
		!m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, dwStyle) || //rebar�� ���� ����
		!m_wndReBar.AddBar(&m_dlgDrawProperty, NULL, NULL, dwStyle)) //rebar�� �׸��� �Ӽ�â ����
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
		TRACE0("�׸��� ���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	CString strDrawToolBarName;
	bNameValid = strDrawToolBarName.LoadString(IDS_TOOLBAR_DRAW);
	ASSERT(bNameValid);
	m_wndDrawToolBar.CleanUpLockedImages();
	m_wndDrawToolBar.LoadBitmap(IDB_DRAWTOOLHI,0,0,TRUE);
	m_wndDrawToolBar.SetWindowText(strDrawToolBarName);
	m_wndDrawToolBar.SetLockedSizes(CSize(32,32), CSize(32,32));
	//////////////////////////////////////////////////////////////////////////

	// ����� ���� ���� ���� �۾��� ����մϴ�.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//���λ� ����
	m_wndStatusBar.SetPaneBackgroundColor(2, CDrawPropertyValue::m_sLineColor);
	COLORREF invertColor = CDrawPropertyValue::m_sLineColor ^ 0x00FFFFFF; //������
	m_wndStatusBar.SetPaneTextColor(2, invertColor);
	//ä���� ����
	m_wndStatusBar.SetPaneBackgroundColor(3, CDrawPropertyValue::m_sBrushColor);
	invertColor = CDrawPropertyValue::m_sBrushColor ^ 0x00FFFFFF; //������
	m_wndStatusBar.SetPaneTextColor(3, invertColor);

	//m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_TASK, SBPS_NORMAL, 100);
	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndReBar.EnableDocking(CBRS_TOP);
//	m_wndDrawToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockPane(&m_wndMenuBar);
	DockPane(&m_wndReBar);
//	DockPane(&m_wndToolBar);
	DockPane(&m_wndDrawToolBar);

	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ����� ���� ���� ���� �̹����� �ε��մϴ�.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// �޴� ���� ������ Ȱ��ȭ�մϴ�(���� �ֱٿ� ����� ���).
	// TODO: ������� �⺻ ����� �����Ͽ� �� Ǯ�ٿ� �޴��� �ϳ� �̻��� �⺻ ����� ���Խ�ŵ�ϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
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
	// �⺻ Ŭ������ ���� �۾��� �����մϴ�.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

}


void CMainFrame::OnUpdateDrawpropertyLinecolor(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable();
}

void CMainFrame::OnUpdateIndicatorTask(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable();
}


void CMainFrame::OnUpdateIndicatorColor(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable();
}


void CMainFrame::OnUpdateIndicatorFillcolor(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable();
}

//-------------------------------------------------------------------------
//���׸����Ϸ� ����
//-------------------------------------------------------------------------
void CMainFrame::OnFileSaveImage()
{
	//���� ���̾�α� ����
	CFileDialog dlg(FALSE, L"bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					L"Bitmap File(*.bmp) | *.bmp |JPG File(*.jpg) | *.jpg |PNG File(*.png) | *.png ||", this);

	if(dlg.DoModal() == IDOK)
	{
		//////////////////////////////////////////////////////////////////////////
		//��View ������ ��Ʈ������ ����
		CRect clientRect;
		CGraphicEditorView* psView = (CGraphicEditorView*)GetActiveView();
		psView->GetClientRect(clientRect);
		//���� ������ �׸��� �ʵ��� ���� ��� ����
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
		//������ ����
		Bitmap loadBitmap(cBitmap, NULL); //������ ������ CBitmap�� GDI+���� ����ϴ� Bitmap ��ü�� �ٲ�
		CLSID imgClsid; //�̹��� �ڵ� ����
		switch(dlg.m_ofn.nFilterIndex)
		{
		case 1:	//BMP�� ���� �̹��� �ڵ��� CLSID�� �޾ƿ�
			GetEncoderClsid(L"image/bmp", &imgClsid);
			break;
		case 2: //JPG�� ���� �̹��� �ڵ��� CLSID�� �޾ƿ�
			GetEncoderClsid(L"image/jpeg", &imgClsid);
			break;
		case 3: //PNG�� ���� �̹��� �ڵ��� CLSID�� �޾ƿ�
			GetEncoderClsid(L"image/png", &imgClsid);
			break;
		}
		//�׸����� ���� �� �Ķ����
		int nQuality = 100;
		EncoderParameters param;
		param.Count                       = 1; 
		param.Parameter[0].Guid           = EncoderQuality;
		param.Parameter[0].Type           = EncoderParameterValueTypeLong;
		param.Parameter[0].NumberOfValues = 1;
		param.Parameter[0].Value          = &nQuality;

		//�׸� ���� ����
		CString fileName = dlg.GetPathName();
		loadBitmap.Save(fileName,&imgClsid, &param);

		//////////////////////////////////////////////////////////////////////////
		//���� ������ �����Ǿ��ٸ� �ٽ� Ȱ��ȭ
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

//�̹��� �ڵ��� CLSID�� �������� �Լ�
BOOL CMainFrame::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;	// �̹��� ���ڴ� ��ȣ
	UINT  size = 0;	// �̹��� ���ڴ� �迭�� ������
 
	GetImageEncodersSize(&num, &size); //�̹��� ���ڴ��� ����� ����
	if(size == 0) return FALSE;

	ImageCodecInfo* pImageCodecInfo = NULL;
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size)); //�޸� �Ҵ�
	if(pImageCodecInfo == NULL) return FALSE;

	GetImageEncoders(num, size, pImageCodecInfo); //�ڵ������� ����
	for(UINT i=0; i < num; ++i)
	{
		//�ڵ� ������ ���޹��� ������ ������ ���
		if(wcscmp(pImageCodecInfo[i].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[i].Clsid; //CLSID�� ����
			free(pImageCodecInfo); //�޸� ����
			return TRUE;
		}    
	}
	free(pImageCodecInfo);
	return FALSE;
}