/* [ GRectangle Ŭ���� by ������ ]
** �׷����� ��ü�� ���� �ֻ��� Ŭ���� GObject�� ��ӹ޾� ������
** GRectangle Ŭ�����Դϴ�.
*/
#pragma once
#include "GObject.h"
class GRectangle : public GObject
{
protected:
	DECLARE_SERIAL(GRectangle);
public:
	// ������ �� �Ҹ���
	GRectangle(void);							//������
	GRectangle(const GRectangle& pGRectangle);	//���� ������
	~GRectangle(void);							//�Ҹ���

	virtual void Serialize(CArchive& ar);		//����ȭ

	// Accessor �Լ�
	CRgn* GetRegion();									// ������ ��ȯ
	int GetPenStyle() const {return m_npPenStyle; }	//�� ��Ÿ�� ��ȯ
	int GetBrushStyle(void) const { return m_esBrushStyle; } //ä��� Ÿ�� ��ȯ
	int GetFaceType(void) const { return m_esFaceType; } //�� Ÿ�� ��ȯ
	COLORREF GetFillColor(void) const { return m_pBrushColor; } //ä���� ��ȯ
	COLORREF GetGradientColor(void) const { return m_sGrdEndColor; } //�׶���Ʈ �� �� ��ȯ
	int GetLineJoin(void) const { return m_nsLineJoin; }	//�ܰ��� ó�� ��ȯ
	int GetHatchStyle(void) const {return m_nsHatchStyle; } //��ġ ��Ÿ�� ��ȯ
	int GetGradientMode(void) const { return m_npGrdMode; } //�׶���Ʈ ��� ��ȯ

	// Mutator �Լ�
	void Move(int dX, int dY);							// ��ǥ �̵�

	void SetPenStyle(int penStyle);						// �� ��Ÿ�� ����
	void SetBrushColor(COLORREF brushColor);			// �귯�� ��Ÿ�� ����
	
	void SetFaceType(int faceType);							// �� Ÿ�� ����
	void SetBrushStyle(int brushStyle);						// �귯�� ��Ÿ�� ����
	void SetHatchStyle(int hatch);							// ��ġ ��Ÿ�� ����
	void SetLineJoinStyle(int lineJoin);					// ���� ���� ó�� ����
	void SetGradientMode(int grdMode);						// �׶���Ʈ ��� ����
	void SetGrdEndColor(COLORREF grdEndColor);				// �׶��̼��� �� �� ����

	// �׸��� �Լ�
	void Draw(CDC* cdc);
	virtual void DrawSelectLine(CDC *pDC);				//���� �� �������� ������ �׸�

	// ũ�� �ٲٴ� �Լ�
	void Resize(int index, int dX, int dY);

	// ���� ��ġ ã�� �Լ�
	int FindSelectPoint(CPoint pt);

private :
	COLORREF m_pBrushColor;					// �귯�� �� ������
	int m_npPenStyle;						// �� ��Ÿ��

	//for GDI+, 120528 �߰� by ����ö
	GBrushType m_esBrushStyle;	//�귯�� Ÿ��
	GFaceType m_esFaceType;		//ä��� ��Ÿ��

	int m_nsHatchStyle;			//��ġ ��Ÿ��=>Ÿ���� int�� ����
	COLORREF m_sGrdEndColor;	//�׶���Ʈ �� �÷�(�÷���ư)
	int m_nsLineJoin;			//���� ���(�޺��ڽ�-��Ӵٿ�)

	int m_npGrdMode;			// �׶���Ʈ ��� by ������
};