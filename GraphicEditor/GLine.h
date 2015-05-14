/* [ GLine Ŭ���� by ������ ]
** �׷����� ��ü�� ���� �ֻ��� Ŭ���� GObject�� ��ӹ޾� ������
** GLine Ŭ�����Դϴ�.
*/
#pragma  once
#include "GObject.h"

class GLine : public GObject {
protected:
	DECLARE_SERIAL(GLine);
public :		// ����Լ�
	GLine(void);				// ������
	GLine(const GLine& pGLine);	//���� ������
	~GLine(void);				// �Ҹ���

	virtual void Serialize(CArchive& ar);	// ����ȭ

	// Accessor �Լ�
	CRgn* GetRegion();						// ������ ��ȯ
	int GetPenStyle() const {return m_nsPenStyle; }	//�� ��Ÿ�� ��ȯ
	CPoint* GetRgnPoint();					// ������ �� �������� ��ȯ
	int GetStartCap(void) const {return m_nsStartCap; } //���� ���� ��� ��ȯ
	int GetEndCap(void) const {return m_nsEndCap; } //���� �� ��� ��ȯ

	// Mutator �Լ�
	void Move(int dX, int dY);				// ��ǥ �̵�
	void Resize(int index, int dX, int dY);	// �� �� �̵�
	int FindSelectPoint(CPoint pt);			// ���� ������ ��ǥ ������ �ε��� ��ȯ

	void SetPenStyle(int penStyle);			// �� ��Ÿ�� ����
	void SetStartCap(int startCap);								//�� ���� ��� ����
	void SetEndCap(int endCap);									//�� �� ��� ����

	void Draw(CDC* cdc);					// �׸��� �Լ�
	virtual void DrawSelectLine(CDC *pDC);	// ���� �� �������� ������ �׸�

private :		// ��� ����
	int m_nsPenStyle;						// �� ��Ÿ��
	CPoint m_pRgnPoint[4];					// ���ý� ������ �����ϴ� ����Ʈ�迭

	//for GDI+, 120528 �߰� by ����ö
	int m_nsStartCap;	//���� ���� ���
	int m_nsEndCap;		//���� �� ���
public:
};