/* [ GEllipse Ŭ���� by ������ ]
** �׷����� ��ü�� ���� �ֻ��� Ŭ���� GObject�� ��ӹ޾� ������
** GEllipse Ŭ�����Դϴ�.
*/
#pragma once
#include "GObject.h"

// ������ ��� ���� TEXTURE�� ���� ���� ����
// ����å ���� 4���� ��Ÿ�� ���� 
// enum BrushType { SOLID, HATCH, GRADATION, TEXTURE};

class GEllipse : public GObject {
protected:
	DECLARE_SERIAL(GEllipse);
public:
	// ������ �� �Ҹ���
	GEllipse(void);							//������
	GEllipse(const GEllipse& pGEllipse);	//���� ������
	~GEllipse(void);						//�Ҹ���

	virtual void Serialize(CArchive& ar);	//����ȭ

	// Accessor �Լ�
	CRgn* GetRegion();										// ������ ��ȯ
	int GetPenStyle() const {return m_nsPenStyle; }	//�� ��Ÿ�� ��ȯ
	int GetBrushStyle(void) const { return m_esBrushStyle; } //ä��� Ÿ�� ��ȯ
	int GetFaceType(void) const { return m_esFaceType; } //�� Ÿ�� ��ȯ
	COLORREF GetFillColor(void) const { return m_pBrushColor; } //ä���� ��ȯ
	COLORREF GetGradientColor(void) const { return m_sGrdEndColor; } //�׶���Ʈ �� �� ��ȯ
	int GetLineJoin(void) const { return m_nsLineJoin; }	//�ܰ��� ó�� ��ȯ
	int GetHatchStyle(void) const {return m_nsHatchStyle; } //��ġ ��Ÿ�� ��ȯ
	int GetGradientMode(void) const { return m_npGrdMode; } //�׶���Ʈ ��� ��ȯ

	// Mutator �Լ�
	void Move(int dX, int dY);								// ��ǥ �̵�

	void SetPenStyle(int penStyle);							// �� ��Ÿ�� ����
	void SetBrushColor(COLORREF brushColor);				// �귯�� Į�� ����

	void SetFaceType(int faceType);							// �� Ÿ�� ����
	void SetBrushStyle(int brushStyle);						// �귯�� ��Ÿ�� ����
	void SetHatchStyle(int hatch);							// ��ġ ��Ÿ�� ����
	void SetLineJoinStyle(int lineJoin);					// ���� ���� ó�� ����
	void SetGradientMode(int grdMode);						// �׶���Ʈ ��� ����
	void SetTransparent(BOOL transparent);					// ���� ä��� ���� ����
	void SetGrdEndColor(COLORREF grdEndColor);				// �׶��̼��� �� �� ����

	// ũ�� �ٲٴ� �Լ�
	void Resize(int index, int dX, int dY);
	int FindSelectPoint(CPoint pt);

	// �׸��� �Լ�
	void Draw(CDC* cdc);									//�׸��� �Լ�
	virtual void DrawSelectLine(CDC *pDC);					//���� �� �������� ������ �׸�

private :
	int m_nsPenStyle;						// �� ��Ÿ��
	COLORREF m_pBrushColor;					// �귯�� Į��

	//for GDI+, 120528 �߰� by ����ö
	GFaceType m_esFaceType;		//�� Ÿ��
	GBrushType m_esBrushStyle;	//�귯�� Ÿ��
	int m_nsHatchStyle;			//��ġ ��Ÿ��=>Ÿ���� int�� ����
	COLORREF m_sGrdEndColor;	//�׶���Ʈ �� �÷�(�÷���ư)
	int m_nsLineJoin;			//���� ���(�޺��ڽ�-��Ӵٿ�)

	int m_npGrdMode;			// �׶���Ʈ ��� by ������
};

