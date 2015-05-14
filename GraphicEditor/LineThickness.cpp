/* [ LineThickness 클래스 by 박현덕 ]
** LineThickness 클래스의 구현부입니다.
*/

#include "stdafx.h"
#include "GraphicEditor.h"
#include "LineThickness.h"
#include "afxdialogex.h"
#include "DrawPropertyValue.h"

// LineThickness 대화 상자입니다.

IMPLEMENT_DYNAMIC(LineThickness, CDialog)

// 생성자
LineThickness::LineThickness(CWnd* pParent /*=NULL*/)
	: CDialog(LineThickness::IDD, pParent)
{	
	// 라인두께를 초기화
	m_npLineThickness = CDrawPropertyValue::m_nsThickness;
}

// 소멸자
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


// LineThickness 메시지 처리기입니다.

//------------------------------------------------------------
//　대화상자의 OK버튼 클릭시 입력된 두께를 읽어와 그 두께로 설정
//------------------------------------------------------------
// void LineThickness::OnBnClickedOk()
// {
// 	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString str;
// 	this->GetDlgItem(IDC_EDIT_LINETHICKNESS)->GetWindowText(str);
// 	this->m_npLineThickness = _wtoi(str);
// 	CDrawPropertyValue::m_nsThickness = this->m_npLineThickness;
// 
// 	CDialog::OnOK();
// }

//------------------------------------------------------------
//　대화상자의 OK버튼 클릭시 입력된 두께를 읽어와 그 두께로 설정
//------------------------------------------------------------
int LineThickness::GetLineThickness() {
	return this->m_npLineThickness;
}