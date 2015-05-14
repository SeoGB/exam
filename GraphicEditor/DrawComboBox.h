/* [ CDrawComboBox Ŭ���� by ����ö ]
** CComboBox�� ��ӹ޾� �޺��ڽ� ���� �����ۿ� �׸����� �׸� �� �ִ� ���ο� �޺��ڽ� Ŭ�����Դϴ�.
**  ��������/���ڹ��� ����/�׶���Ʈ ������ �׸� �� ����մϴ�.
*/

#pragma once
enum ComboBoxDrawType
{
	LINEPATTERN,
	HATCHPATTERN,
	GRADIENTPATTERN,
	LINECAP //�޺� �ڽ� ���� �׷��� Ÿ���� �����ϱ� ���� ���
};

// CDrawComboBox

class CDrawComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CDrawComboBox)

protected:
	struct DrawItems 
	{
		int m_nsIndex; //�ε���
		ComboBoxDrawType m_esType; //Ÿ��
	}; //�׷��� �������� ����ü

public:
	DrawItems* m_psDrawItems;	//�޺� �ڽ��� �� ������
	COLORREF m_sHighlight;		//���� �� ���̶���Ʈ �÷�

public:
	CDrawComboBox();
	virtual ~CDrawComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void AddItem(int nIndex, ComboBoxDrawType m_esType);
	afx_msg void OnDestroy();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
};


