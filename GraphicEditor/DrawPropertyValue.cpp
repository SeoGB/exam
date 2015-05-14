#include "StdAfx.h"
#include "DrawPropertyValue.h"

//�� ��� �ʱ�ȭ
LOGFONT	CDrawPropertyValue::	m_sLogFont;							//��Ʈ ����
int CDrawPropertyValue::		m_nsFontSize = 10;					//��Ʈ ũ��

COLORREF CDrawPropertyValue::	m_sLineColor = RGB(0,0,0);			//���� �÷�
int CDrawPropertyValue::		m_nsThickness = 1;					//�� �β�
int CDrawPropertyValue::		m_nsPenStyle = PS_SOLID;			//�� ��Ÿ��
int CDrawPropertyValue::		m_nsStartCap = 2;					//�� ���� ���
int CDrawPropertyValue::		m_nsEndCap = 2;						//�� �� ���

COLORREF CDrawPropertyValue::	m_sBrushColor = RGB(255,255,255);	//�귯�� �÷�(ä��� ��)
int CDrawPropertyValue::		m_nsBrushStyle = 0;					//�귯�� ��Ÿ��
int CDrawPropertyValue::		m_nsHatchStyle = 0;					//��ġ ��Ÿ��
int CDrawPropertyValue::		m_nsFaceType = 1;					//�� Ÿ��
int CDrawPropertyValue::		m_nsGradientType = 0;				//�׶���Ʈ Ÿ��
COLORREF CDrawPropertyValue::	m_sGradientEndColor = RGB(0,0,0);	//�׶���Ʈ ���÷�(ä��� ��2)
int CDrawPropertyValue::		m_nsLineJoin = 0;

int CDrawPropertyValue::		m_nsAlpha = 100;					//������

//�α� ��Ʈ �ʱ�ȭ �Լ�
void CDrawPropertyValue::InitializeLogFont(void)
{
	HDC hdc = ::GetDC(::GetDesktopWindow());
	::ZeroMemory(&m_sLogFont, sizeof(LOGFONT)); //�α���Ʈ �ʱ�ȭ
	m_sLogFont.lfHeight = -MulDiv(10, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
	_tcscpy_s(m_sLogFont.lfFaceName, LF_FACESIZE, _T("����"));
	::ReleaseDC(::GetDesktopWindow(), hdc);
}
