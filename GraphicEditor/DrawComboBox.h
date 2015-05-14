/* [ CDrawComboBox 클래스 by 신희철 ]
** CComboBox를 상속받아 콤보박스 내에 아이템에 그림까지 그릴 수 있는 새로운 콤보박스 클래스입니다.
**  라인패턴/격자무늬 패턴/그라디언트 패턴을 그릴 때 사용합니다.
*/

#pragma once
enum ComboBoxDrawType
{
	LINEPATTERN,
	HATCHPATTERN,
	GRADIENTPATTERN,
	LINECAP //콤보 박스 내의 그려질 타입을 구분하기 위해 사용
};

// CDrawComboBox

class CDrawComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CDrawComboBox)

protected:
	struct DrawItems 
	{
		int m_nsIndex; //인덱스
		ComboBoxDrawType m_esType; //타입
	}; //그려질 아이템의 구조체

public:
	DrawItems* m_psDrawItems;	//콤보 박스에 들어갈 아이템
	COLORREF m_sHighlight;		//선택 시 하이라이트 컬러

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


