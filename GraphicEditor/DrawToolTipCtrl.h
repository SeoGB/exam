/* [ CDrawToolTipCtrl 클래스 by 신희철 ]
** 해당 클래스에서는 다이얼로그에 툴팁을 띄우기 위해 선언한 클래스입니다.
** CMFCToolTipCtrl을 상속받았기 때문에 일반 툴팁과 달리 스타일을 지정할 수 있습니다.
*/

#pragma once


// CDrawToolTipCtrl

class CDrawToolTipCtrl : public CMFCToolTipCtrl
{
	DECLARE_DYNAMIC(CDrawToolTipCtrl)
public:
	CDrawToolTipCtrl(); //생성자

public:
	int	m_nsCurrID; //현재 아이디

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTtnTooltipShow(NMHDR *pNMHDR, LRESULT *pResult); //툴팁 출력시 설정
};