/* [ GText Ŭ���� by ����ö, ������ ]
** GText Ŭ������ �������Դϴ�.
*/

#include "StdAfx.h"
#include "GText.h"


IMPLEMENT_SERIAL(GText, GObject, 0)
//--------------------------------------------------------------------------
//�������� �� �Ҹ���
//--------------------------------------------------------------------------
//������
GText::GText(void)
{
	m_sType = TEXT; //�׷����� ��ü Ÿ���� TEXT
	::ZeroMemory(&m_sLogFont, sizeof(LOGFONT)); //�α���Ʈ �ʱ�ȭ
}

//���� ������
GText::GText(const GText& pGText)
{
	this->GObject::m_sType = pGText.GObject::m_sType;
	this->GObject::m_sStartPoint = pGText.GObject::m_sStartPoint;
	this->GObject::m_sLineColor = pGText.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pGText.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pGText.GObject::m_bsGrouped;

	this->m_sBgColor = pGText.m_sBgColor;
	this->m_sEndPoint = pGText.m_sEndPoint;
	this->m_sLogFont = pGText.m_sLogFont;
	for(int i=0;this-m_sArrayString.GetSize();i++)
	{
		this->m_sArrayString.Add(m_sArrayString.GetAt(i));
	}
}

GText::~GText(void)
{}

//--------------------------------------------------------------------------
//������ȭ
//--------------------------------------------------------------------------
void GText::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring()) //���� ��
	{
		ar << (WORD)m_bsTransparent;
		m_sArrayString.Serialize(ar);
		ar << m_sBgColor;
		ar.Write(&m_sLogFont, sizeof(LOGFONT));
	}
	else //�ε� ��
	{
		WORD wTemp;
		ar >> wTemp; m_bsTransparent = wTemp;
		m_sArrayString.Serialize(ar);
		ar >> m_sBgColor;
		ar.Read(&m_sLogFont, sizeof(LOGFONT));
	}
}

//--------------------------------------------------------------------------
//�������� ������ ���ϴ� �Լ�
//--------------------------------------------------------------------------
CRgn* GText::GetRegion()
{
	m_sRgn.DeleteObject();
	m_sRgn.CreateRectRgn(m_sStartPoint.x-5, m_sStartPoint.y-5, m_sEndPoint.x+5, m_sEndPoint.y+5);
	return &m_sRgn;
}

//--------------------------------------------------------------------------
//���̵� �Լ�
//--------------------------------------------------------------------------
void GText::Move(int dX, int dY)
{
	m_sStartPoint.x += dX;
	m_sStartPoint.y += dY;
	m_sEndPoint.x += dX;
	m_sEndPoint.y += dY;
}

//--------------------------------------------------------------------------
//���۲�(��Ʈ, ũ��) ���� �Լ�
//--------------------------------------------------------------------------
void GText::SetLogFont(LOGFONT lf)
{
	m_sLogFont = lf; //�۲�(��Ʈ, ũ��) ����(CFontDialog���� ������ ����)
}

//--------------------------------------------------------------------------
//������ ���� �Լ�
//--------------------------------------------------------------------------
void GText::SetBgColor(COLORREF color)
{
	m_sBgColor = color; //���� ����
}

//--------------------------------------------------------------------------
//������ ��� ���� ���� �Լ�
//--------------------------------------------------------------------------
void GText::SetBgTransparent(bool bTransparent)
{
	m_bsTransparent = bTransparent; //������ ���� ����
}
//--------------------------------------------------------------------------
//������ �߰� �Լ�
//--------------------------------------------------------------------------
void GText::AddChar(UINT nChar)
{
	m_sArrayString.Add(nChar); //���� �߰�
}

//--------------------------------------------------------------------------
//������ ����� �Լ�
//--------------------------------------------------------------------------
void GText::DelChar(void)
{
	if(m_sArrayString.GetSize() > 0) //���ڰ� �ԷµǾ��ִ� ���
		m_sArrayString.RemoveAt(m_sArrayString.GetSize() - 1); //������ ���� ����
}

//--------------------------------------------------------------------------
//���ؽ�Ʈ ���� ��ȯ �Լ� by ������
//--------------------------------------------------------------------------
TCHAR *GText::GetString() {
	return this->m_sArrayString.GetData();
}
//--------------------------------------------------------------------------
//���α� ��Ʈ ��ȯ �Լ� by ������
//--------------------------------------------------------------------------
LOGFONT GText::GetLogfont() {
	return this->m_sLogFont;
}
//--------------------------------------------------------------------------
//������ �� ��ȯ �Լ� by ������
//--------------------------------------------------------------------------
int GText::GetStringCount() {
	return this->m_sArrayString.GetSize();
}
//--------------------------------------------------------------------------
// �׸��� �Լ�
//--------------------------------------------------------------------------
void GText::Draw(CDC* pDC)
{
	CFont font;
	font.CreateFontIndirect(&m_sLogFont); //��Ʈ ����
	CFont *oldFont = pDC->SelectObject(&font); //��Ʈ ����
	
	COLORREF oldTextColor, oldBkColor;
	oldTextColor = pDC->SetTextColor(m_sLineColor); //���� �� ����
	oldBkColor = pDC->SetBkColor(m_sBgColor); //��� �� ����

	int oldBkMode;
	if(m_bsTransparent == TRUE) //��� ������ ���
		oldBkMode = pDC->SetBkMode(TRANSPARENT); //���� ���
	else //������ �ƴ� ���
		oldBkMode = pDC->SetBkMode(OPAQUE); //������ ������ ���

	CSize textSize = pDC->GetTextExtent(m_sArrayString.GetData(), m_sArrayString.GetSize()); //�ؽ�Ʈ�� �׷��� ������ ũ�⸦ ����
	
	//���� �� ��ġ ����
	m_sEndPoint.x = m_sStartPoint.x + textSize.cx;
	m_sEndPoint.y = m_sStartPoint.y + textSize.cy;

	CRect textRect(m_sStartPoint.x, m_sStartPoint.y, m_sEndPoint.x, m_sEndPoint.y); //�ؽ�Ʈ�� �׷��� rect
	pDC->DrawText(m_sArrayString.GetData(), m_sArrayString.GetSize(), &textRect, DT_LEFT | DT_SINGLELINE); //���� ����
	
	//���� �������� �ǵ�����
	pDC->SetTextColor(oldTextColor); //���� �ؽ�Ʈ �÷��� �ǵ�����
	pDC->SetBkColor(oldBkColor); //���� ��׶��� �÷��� �ǵ�����
	pDC->SetBkMode(oldBkMode); //���� ��׶��� ���� �ǵ�����
	pDC->SelectObject(oldFont); //���� ��Ʈ�� �ǵ�����
}

//--------------------------------------------------------------------------
//������ ���� ���� �׸��� �Լ�
//--------------------------------------------------------------------------
void GText::DrawSelectLine(CDC *pDC)
{
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //�������� �簢���� ����
	pDC->Rectangle(rect); //�簢�� ������ �׸�

	//���� ��� �귯�� �Ӽ����� �ǵ���
	pDC->SelectObject(oldPen);
}

