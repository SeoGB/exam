/* [ LinePattern 클래스 by 박현덕 ]
** LinePattern을 설정하기 위한 대화상자 기반
** LinePattern 클래스입니다.
*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DrawComboBox.h"
#include "DrawToolTipCtrl.h"

// LinePattern 대화 상자입니다.

class LinePattern : public CDialog
{
	DECLARE_DYNAMIC(LinePattern)

public:
	LinePattern(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~LinePattern();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LINEPATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	CEdit m_pLineWidthEdit;					// 두께
	CSpinButtonCtrl m_pLineWidthSpin;		// 두께 스핀 컨트롤 변수

	CDrawComboBox m_pLinePtnCmb;			// 라인 패턴 컨트롤 변수

	CButton m_pLineColorBtn;				// 라인 색 버튼 컨트롤 변수

	CDrawComboBox m_pStartCapCombo;			// 라인 시작 모양 컨트롤 변수
	CDrawComboBox m_pEndCapCombo;			// 라인 끝 모양 컨트롤 변수

	CEdit m_pAlphaEdit;						// 투명도
	CSpinButtonCtrl m_pAlphaSpin;			// 투명도 스핀 컨트롤 변수

	// 툴팁
	CDrawToolTipCtrl m_pCtrlToolTip;
public :
	afx_msg void OnBnClickedButtonLinecolor();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnCbnSelchangeComboDashstyle();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCbnSelchangeComboStartcap();
	afx_msg void OnCbnSelchangeComboEndcap();
	afx_msg void OnBnClickedClose();

	void ChangeIndex(int *startIndex, int *endIndex);		// 라인의 시작 모양과 끝 모양 설정시
															// 해당되는 인덱스가 순차적이 아니기에
															// 설정해주어야 함
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnClose();

	void SetAlphaSpin(void);
	void SetLinePattern(void);
	void SetStartCap(void);
	void SetEndCap(void);
	void SetLineThickness(void);
};