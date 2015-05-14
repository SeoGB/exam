/* [ BrushPattern 클래스 by 박현덕 ]
** BrushPattern을 설정하기 위한 대화상자 기반
** BrushPattern 클래스입니다.
** Brush 종류, 그리기 종류, 그라디언트 등의 속성지정 가능
*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DrawComboBox.h"
#include "DrawToolTipCtrl.h"

// BrushPattern 대화 상자입니다.

class BrushPattern : public CDialog
{
	DECLARE_DYNAMIC(BrushPattern)

public:
	BrushPattern(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BrushPattern();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BRUSHPATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public :
	CComboBox m_pLineJoinCombo;				// 외곽선 처리 컨트롤 변수

	CButton m_pFillTypeSolid;				// 채우기 타입 솔리드 컨트롤 변수
	CButton m_pFillTypeHatch;				// 채우기 타입 해치 컨트롤 변수
	CButton m_pFillTypeGradient;			// 채우기 타입 그라디언트  컨트롤 변수

	CButton m_pFillColorBtn;				// 채우기 색 버튼 컨트롤 변수
	CButton m_pGrdColorBtn;					// 그라디언트 색 버튼 컨트롤 변수

	CDrawComboBox m_pHatchPatternCombo;		// 채우기 패턴 해치 컨트롤 변수
	CDrawComboBox m_pGrdPtnCombo;			// 채우기 패턴 그라디언트 컨트롤 변수
	
	CEdit m_pFillAlphaEdit;					// 투명도
	CSpinButtonCtrl m_pFillAlphaSpin;		// 투명도 스핀 컨트롤 변수

	CButton m_pOutlineOnly;					// 면 타입 외곽선 라디오버튼 컨트롤 변수
	CButton m_pFillOutline;					// 면 타입 외곽선 + 채우기라디오버튼 컨트롤 변수
	CButton m_pFillOnly;					// 면 타입 채우기 라디오버튼 컨트롤 변수

	//툴팁
	CDrawToolTipCtrl m_pCtrlToolTip;		// 툴팁 컨트롤

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