/* [ CDrawToolTipCtrl Ŭ���� by ����ö ]
** �ش� Ŭ���������� ���̾�α׿� ������ ���� ���� ������ Ŭ�����Դϴ�.
** CMFCToolTipCtrl�� ��ӹ޾ұ� ������ �Ϲ� ������ �޸� ��Ÿ���� ������ �� �ֽ��ϴ�.
*/

#pragma once


// CDrawToolTipCtrl

class CDrawToolTipCtrl : public CMFCToolTipCtrl
{
	DECLARE_DYNAMIC(CDrawToolTipCtrl)
public:
	CDrawToolTipCtrl(); //������

public:
	int	m_nsCurrID; //���� ���̵�

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTtnTooltipShow(NMHDR *pNMHDR, LRESULT *pResult); //���� ��½� ����
};