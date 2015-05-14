/* [ CDrawProperty 클래스 by 신희철 ]
** 메인 프레임에 항상 붙어있는 그리기 속성에 대한 다이얼로그 클래스입니다.
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
	CButton m_sFontBtn;					//폰트 버튼 컨트롤 변수

	CButton m_sFillColorBtn;			//채우기 색 버튼 컨트롤 변수
	CButton m_sLineColorBtn;			//라인 색 컨트롤 변수
	CButton m_sGradientEndColorBtn;		//그라디언트 끝 색 컨트롤 변수

	CSpinButtonCtrl m_sLineWeightSpin;	//라인 굵기 스핀 컨트롤 변수
	CEdit m_sThicknessEdit;				//라인 굵기 에디트 컨트롤 변수

	CDrawComboBox m_sLinePtnCombo;		//라인 패턴 컨트롤 변수
	CDrawComboBox m_sFillPtnCombo;		//채우기 색 버튼 컨트롤 변수

	CButton m_sFaceTypeOutline;			//면 타입 외곽선 라디오버튼 컨트롤 변수
	CButton m_sFaceTypeOutlinedFill;	//면 타입 외곽선+채우기 라디오버튼 컨트롤 변수
	CButton m_sFaceTypeFillOnly;		//면 타입 only 채우기 라디오버튼 컨트롤 변수

	CButton m_sFillTypeSolid;			//채우기 타입 솔리드 라디오버튼 컨트롤 변수
	CButton m_sFillTypeHatch;			//채우기 타입 해치 라디오버튼 컨트롤 변수
	CButton m_sFillTypeGradient;		//채우기 타입 그라디언트 라디오버튼 컨트롤 변수

	CSpinButtonCtrl m_sAlphaSpin;		//불투명도 스핀 컨트롤 변수
	CEdit m_sAlphaEdit;					//불투명도

	CComboBox m_sStartCapCombo;			//선 시작 모양 컨트롤 변수
	CComboBox m_sEndCapCombo;			//선 끝 모양 컨트롤 변수
	
	CDrawComboBox m_sGradientCombo;		//그라디언트 채우기 타입 컨트롤변수
	CComboBox m_sLineJoinCombo;			//외곽선 모양 컨트롤 변수

	//정적 컨트롤 변수
	CStatic m_sStcFont;			//폰트	
	CStatic m_sStcThickness;	//라인 굵기
	CStatic m_sStcLinePtn;		//라인 패턴
	CStatic m_sStcFillPtn;		//채우기 패턴
	CStatic m_sStcOpacity;		//불투명도
	CStatic m_sStcEndCap;		//라인 끝 모양
	CStatic m_sStcStartCap;		//라인 시작 모양
	CStatic m_sStcGradientMode;	//그라디언트 모드
	CStatic m_sStcLineJoin;		//외곽선 모양

	//컨트롤에 대한 value 변수
	int m_nsFaceType;					//면 타입의 선택값
	int m_nsBrushType;					//채우기 타입의 선택값

	//툴팁
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

	void SetAlphaSpin(void);			// 불투명도 값 설정
	void SetLinePattern(void);			// 라인 스타일 설정
	void SetStartCap(int startCap, BOOL isPropValue = FALSE);	// 라인의 시작 모양 설정
	void SetEndCap(int endCap, BOOL isPropValue = FALSE);		// 라인의 끝 모양 설정
	void SetLineJoin(int lineJoin);			// 외곽선 처리 설정
	void SetHatchPattern();				// 해치 스타일 설정
	void SetDrawStyle(int style);			// 그리기 종류 설정
	void SetFillStyle(int fillStyle);		// 채우기 종류 설정
	void SetGradientPattern(int grdPattern);	// 그라디언트 유형 설정

	afx_msg void OnCbnSelchangeComboStartcap();
	afx_msg void OnCbnSelchangeComboEndcap();
	afx_msg void OnCbnSelchangeComboGradient();
	afx_msg void OnBnClickedButtonFillcolor2();
	afx_msg void OnCbnSelchangeComboLinejoin();


	void AdjustControl(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
