/* [ LineThickness 클래스 by 박현덕 ]
** LineThickness를 설정하기 위한 대화상자 기반
** LineThickness 클래스입니다.
*/
#pragma once

// LineThickness 대화 상자입니다.

class LineThickness : public CDialog
{
	DECLARE_DYNAMIC(LineThickness)

public:
	LineThickness(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~LineThickness();

	int GetLineThickness();					// 라인 두께를 얻어옴

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LINETHICKNESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_npLineThickness;						// 라인두께
public :
	afx_msg void OnBnClickedOk();				// OK 버튼 이벤트처리 함수
};