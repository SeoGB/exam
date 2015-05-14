/* [ DrawPropertyValue Ŭ���� by ����ö ]
** �ش� Ŭ���������� �׸��� �Ӽ��� ���� ������ ������ �ֽ��ϴ�.
** �׸��� �Ӽ� ���̾�α��� �� ��Ʈ�ѿ� ���� value �������� ���� ����� �ξ�,
** �׸��� �Ӽ� ���̾�α׸� include ���� �ʰ� �� Ŭ������ include�ϸ�,
** �׸��� �Ӽ� ���̾�α׿��� ������ ������ ������ �� �� �ֽ��ϴ�.
** (���� �� ������ Ȯ�븦 ����)
*/

#pragma once
class CDrawPropertyValue
{
public:
	static LOGFONT m_sLogFont;		//��Ʈ ����
	static int m_nsFontSize;		//��Ʈ ũ��
	
	static COLORREF m_sLineColor;	//���� �÷�
	static int m_nsThickness;		//�� �β�
	static int m_nsPenStyle;		//�� ��Ÿ��
	static int m_nsStartCap;		//�� ���� ���
	static int m_nsEndCap;			//�� �� ���

	static COLORREF m_sBrushColor;	//�귯�� �÷�(ä��� ��)
	static int m_nsBrushStyle;		//�귯�� ��Ÿ��(�ָ���, ��ġ, �׶���Ʈ)
	static int m_nsHatchStyle;		//��ġ ��Ÿ��
	static int m_nsFaceType;		//�� Ÿ��
	static int m_nsGradientType;	//�׶���Ʈ Ÿ��
	static COLORREF m_sGradientEndColor; //�׶���Ʈ �� �÷�
	static int m_nsLineJoin;		//�ܰ��� ���

	static int m_nsAlpha;			//������

	static void InitializeLogFont(void); //�α� ��Ʈ �ʱ�ȭ
};

