/* [ LineThickness Ŭ���� by ������ ]
** LineThickness Ŭ������ �������Դϴ�.
*/

#include "stdafx.h"
#include "GraphicEditor.h"
#include "LineThickness.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"

// LineThickness ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(LineThickness, CDialog)

// ������
LineThickness::LineThickness(CWnd* pParent /*=NULL*/)
	: CDialog(LineThickness::IDD, pParent)
{	
	// ���εβ��� �ʱ�ȭ
	m_npLineThickness = CDrawPropertyValue::m_nsThickness;
}

// �Ҹ���
LineThickness::~LineThickness()
{
}

void LineThickness::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_EDIT_LINETHICKNESS, m_npLineThickness);
	DDV_MinMaxInt(pDX, m_npLineThickness, 1, 30);
}


BEGIN_MESSAGE_MAP(LineThickness, CDialog)
/*	ON_BN_CLICKED(IDOK, &LineThickness::OnBnClickedOk)*/
END_MESSAGE_MAP()


// LineThickness �޽��� ó�����Դϴ�.

//------------------------------------------------------------
//����ȭ������ OK��ư Ŭ���� �Էµ� �β��� �о�� �� �β��� ����
//------------------------------------------------------------
// void LineThickness::OnBnClickedOk()
// {
// 	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
// 	CString str;
// 	this->GetDlgItem(IDC_EDIT_LINETHICKNESS)->GetWindowText(str);
// 	this->m_npLineThickness = _wtoi(str);
// 	CDrawPropertyValue::m_nsThickness = this->m_npLineThickness;
// 
// 	CDialog::OnOK();
// }

//------------------------------------------------------------
//����ȭ������ OK��ư Ŭ���� �Էµ� �β��� �о�� �� �β��� ����
//------------------------------------------------------------
int LineThickness::GetLineThickness() {
	return this->m_npLineThickness;
}