/* [ GGroup 클래스 by 신희철 ]
** 그래피컬 개체의 그룹과 관련된 클래스로 그룹의 정보와 속성을 가지고 있습니다.
*/
#pragma once
#include "gobject.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GRectangle.h"
#include "GEllipse.h"
#include "GText.h"
#include "QuickSort.h"

// GGroup 클래스
class GGroup : public GObject
{
protected:
	DECLARE_SERIAL(GGroup);
public:
	//생성자 및 소멸자
	GGroup(void);					//생성자
	GGroup(const GGroup& pGGroup);	//복사 생성자
	~GGroup(void);					//소멸자

	//직렬화
	virtual void Serialize(CArchive& ar);	//직렬화

	//그룹화 관련 함수
	void Add(CTypedPtrList<CObList, GObject*>& listGObj);			//추가
	void Remove(CTypedPtrList<CObList, GObject*>& listGObj);		//제거
	void UnGroup(CTypedPtrList<CObList, GObject*>* listGObj);		//그룹 해제
	void UnGroupChild(GGroup& childGroup, CTypedPtrList<CObList, GObject*>* listGObj);	//그룹 해제(차일드)
	void GetChild(CTypedPtrList<CObList, GObject*>* listGObj);		//차일드 그룹 가져오기
	BOOL IsLeaf(void);												//리프인지 판단

	//Accessor 함수
	void GetGroupData(CTypedPtrList<CObList, GObject*>* listGObj);	//그룹 데이터를 listGObj에 저장(즉, 반환값은 listGObj)
	virtual CRgn* GetRegion();										//그룹의 영역 반환

	//Mutator 함수
	virtual void Move(int dX, int dY);
	virtual void Resize(int index, int dX, int dY);
	int FindSelectPoint(CPoint pt);

	void SetGroupLineColor(COLORREF color);				// 라인 컬러 설정
	void SetGroupThickness(int thickness);				// 라인 두께 설정
	void SetPenStyle(int penStyle);						// 펜 스타일 설정
	void SetBrushColor(COLORREF brushColor);			// 브러쉬 컬러 설정

	void SetStartCap(int startCap);						// 선 시작 모양 설정
	void SetEndCap(int endCap);							// 선 끝 모양 설정

	void SetFaceType(int faceType);						// 면 타입 설정
	void SetBrushStyle(int brushStyle);					// 브러쉬 스타일 설정
	void SetHatchStyle(int hatch);						// 해치 스타일 설정
	void SetLineJoinStyle(int lineJoin);				// 라인 꺾임 처리 설정
	void SetGradientMode(int grdMode);					// 그라디언트 모드 설정
	void SetGrdEndColor(COLORREF grdEndColor);			// 그라데이션의 끝 색 설정

	void SetGroupAlpha(int nAlpha);						// 불투명도 지정
	void SetLogFont(LOGFONT lf);						// 로그 폰트 지정

	//그리기 함수
	virtual void Draw(CDC* cdc);							//이 함수에서 그래피컬 개체 그림
	virtual void DrawSelectLine(CDC *pDC);					//선택 시 점선으로 영역을 그림

	// 그룹화 리사이징 함수 by 박현덕
	void StartResizing(int x, int y, int direction);		// 리사이징 시작 결정
	void DoResizing(int x, int y);
	void StartChildResizing(int x, int y, int width, int height, int direction);

	int m_pResizeX, m_pReszieY;
	int m_pResizeWidth, m_pResizeHeight;
	int m_pResizeDirection;

private:
	void FindStartEndPt(); //데이터 및 차일드 노드를 전부 순회하여 leftTop Point와 rightBottom Point를 찾음
	void MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX);
	CTypedPtrList<CObList, GObject*> m_sNodeData; //데이터 노드
	CTypedPtrList<CObList, GGroup*> m_sChild;	//차일드 노드
};
