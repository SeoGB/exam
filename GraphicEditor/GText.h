/* [ GText Ŭ���� by ����ö, ������ ]
** GText Ŭ������ �ؽ�Ʈ ��ü�� �����ϴ� Ŭ�����Դϴ�.
*/

#pragma once
#include "gobject.h"
class GText : public GObject
{
protected:
	DECLARE_SERIAL(GText);
public:
	//������ �� �Ҹ���
	GText(void);				//������
	GText(const GText& pGText);	//���� ������
	~GText(void);				//�Ҹ���

	virtual void Serialize(CArchive& ar);

	//Accessor �Լ�
	void SetLogFont(LOGFONT lf);							//�۲� ����(��Ʈ, ũ��)
	void SetBgColor(COLORREF color);						//���� ����
	void SetBgTransparent(bool bTransparent);				//���� ��� ���� ����
	void AddChar(UINT nChar);								//���� �߰�
	void DelChar(void);										//�� ���� ����

	virtual CRgn* GetRegion();								//������ ����
	virtual int FindSelectPoint(CPoint pt) { return -1; }	//������ ���� ã�� �Լ�
	virtual void Resize(int index, int dX, int dY) { }		//ũ�� ����
	virtual void Move(int dX, int dY);						//��ǥ �̵�

	virtual void Draw(CDC* cdc);							//�� �Լ����� �׷����� ��ü �׸�
	virtual void DrawSelectLine(CDC *pDC);					//���� �� �������� ������ �׸�

	int GetStringCount();	//���� �� ��ȯ by ������
	TCHAR *GetString();		//�Էµ� �ؽ�Ʈ ��ȯ by ������
	LOGFONT GetLogfont();	//�α� ��Ʈ ��ȯ by ������

protected:
	CArray <TCHAR, TCHAR> m_sArrayString;	//���� ���� �迭
	COLORREF m_sBgColor;					//����
	LOGFONT m_sLogFont;						//�۲�(��Ʈ, ũ��)
	BOOL m_bsTransparent;					//���� ��� ����
};