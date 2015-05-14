/* [ GGroup 클래스 by 신희철 ]
** GGroup 클래스의 구현부 입니다.
*/

#include "StdAfx.h"
#include "GGroup.h"

IMPLEMENT_SERIAL(GGroup, GObject, 0)
// GGroup 클래스 구현
//--------------------------------------------------------------------------
//　GGroup 생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
GGroup::GGroup(void)
{
	m_sType = GGROUP;
}

GGroup::GGroup(const GGroup& pGGroup)
{
	this->GObject::m_sType = pGGroup.GObject::m_sType;
	this->GObject::m_sStartPoint = pGGroup.GObject::m_sStartPoint;
	this->GObject::m_sLineColor = pGGroup.GObject::m_sLineColor;
	this->GObject::m_nsThickness = pGGroup.GObject::m_nsThickness;
	this->GObject::m_bsGrouped = pGGroup.GObject::m_bsGrouped;

	this->m_sEndPoint = pGGroup.m_sEndPoint;
}

//소멸자
GGroup::~GGroup(void)
{
}

//--------------------------------------------------------------------------
//　직렬화
//--------------------------------------------------------------------------
void GGroup::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring())	//저장 시
	{
		m_sNodeData.Serialize(ar); 
		m_sChild.Serialize(ar);
	}
	else //로드 시
	{
		m_sNodeData.Serialize(ar);
		m_sChild.Serialize(ar);
	}
}

//--------------------------------------------------------------------------
//　Group 추가 함수
//--------------------------------------------------------------------------
void GGroup::Add(CTypedPtrList<CObList, GObject*>& listGObj)
{
	GObject* pGObj;
	POSITION pos = listGObj.GetHeadPosition();
	while(pos)
	{
		pGObj = (GObject*)listGObj.GetNext(pos);
		pGObj->SetGrouped(TRUE); //현재 개체가 그룹이 되었음을 표시
		if(pGObj->GetType() == GGROUP) //그룹일 경우
		{
			m_sChild.AddTail((GGroup*)pGObj); //차일드에 추가
		}
		else //그룹이 아닐 경우
		{
			m_sNodeData.AddTail(pGObj); //노드 데이터에 추가
		}
	}
}

//--------------------------------------------------------------------------
//　Group 삭제 함수
//--------------------------------------------------------------------------
void GGroup::Remove(CTypedPtrList<CObList, GObject*>& listGObj)
{
	POSITION pos = listGObj.GetHeadPosition();
	while(pos)
	{
		GObject* pGObj = (GObject*)listGObj.GetNext(pos);
		pGObj->SetGrouped(FALSE); //리스트에서 삭제한 개체가 그룹이 아님을 표시
		if(pGObj->GetType() == GGROUP) //그룹일 경우
		{
			//차일드에서 제거
			POSITION delPos = m_sChild.Find(pGObj, NULL);
			m_sChild.RemoveAt(delPos);
		}
		else //그룹이 아닐 경우
		{
			//노드 데이터에서 제거
			POSITION delPos = m_sNodeData.Find(pGObj, NULL); 
			m_sNodeData.RemoveAt(delPos);
		}
	}
}
//--------------------------------------------------------------------------
//　그룹 해제
//--------------------------------------------------------------------------
void GGroup::UnGroup(CTypedPtrList<CObList, GObject*>* listGObj)
{
	//노드 데이터 부분의 그룹 해제
	POSITION posNode = this->m_sNodeData.GetHeadPosition();
	while(posNode) //노드 데이터 리스트 순회
	{
		GObject* pGObj = (GObject*)this->m_sNodeData.GetNext(posNode); 
		pGObj->SetGrouped(FALSE); //그룹이 아님을 표시
		listGObj->AddTail(pGObj); //선택 리스트에 넣음
	}
	this->m_sNodeData.RemoveAll();
	
	//차일드 부분의 그룹 해제
	POSITION posChild = this->m_sChild.GetHeadPosition();
	while(posChild) //차일드 리스트 순회
	{
		GGroup* pGObj = (GGroup*)this->m_sChild.GetNext(posChild);
		pGObj->SetGrouped(FALSE); //그룹이 아님을 표시
		listGObj->AddTail(pGObj); //선택 리스트에 넣음
		pGObj->UnGroupChild(*pGObj, listGObj); //차일드용 그룹해제 함수 호출
	}
	delete this;
}

//--------------------------------------------------------------------------
//　그룹 해제(차일드)
//--------------------------------------------------------------------------
void GGroup::UnGroupChild(GGroup& childGroup, CTypedPtrList<CObList, GObject*>* listGObj)
{
	POSITION pos = childGroup.m_sChild.GetHeadPosition();
	while(pos) //매개변수로 받은 차일드 리스트 순회
	{
		GGroup* pGObj = (GGroup*)childGroup.m_sChild.GetNext(pos);
		listGObj->AddTail(pGObj); //선택 리스트에 넣음
		pGObj->UnGroupChild(*pGObj, listGObj); //recursion 시킴
	}
}

//--------------------------------------------------------------------------
//　차일드에 대한 포인터를 얻는 함수
//--------------------------------------------------------------------------
void GGroup::GetChild(CTypedPtrList<CObList, GObject*>* listGObj)
{
	//차일드 부분의 그룹 해제
	POSITION posChild = this->m_sChild.GetHeadPosition();
	while(posChild) //차일드 리스트 순회
	{
		GGroup* pGObj = (GGroup*)this->m_sChild.GetNext(posChild);
		listGObj->AddTail(pGObj); //리스트에 넣음
		pGObj->GetChild(listGObj); //차일드 데이터를 받아오는 함수 재호출
	}
}

//--------------------------------------------------------------------------
//　그룹 데이터를 매개변수인 listGObj에 저장 
//--------------------------------------------------------------------------
void GGroup::GetGroupData(CTypedPtrList<CObList, GObject*>* listGObj)
{
	POSITION pos = m_sNodeData.GetHeadPosition();
	while(pos) //노드 데이터 리스트 순회
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(pos);
		listGObj->AddTail(psGObj);
	}

	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild) //차일드 리스트 순회
	{
		GGroup* psGGroup = (GGroup*)m_sChild.GetNext(posChild);
		listGObj->AddTail(psGGroup);
		psGGroup->GetGroupData(listGObj);
	}
}

//--------------------------------------------------------------------------
//　리프인지 검사
//--------------------------------------------------------------------------
BOOL GGroup::IsLeaf()
{
	if(m_sChild.GetSize() == 0) //차일드에 데이터가 없으면 Leaf
		return TRUE;
	else //차일드에 데이터가 있으면 non-leaf
		return FALSE;
}

//--------------------------------------------------------------------------
//　그룹의 left,top 좌표와 right,bottom 좌표를 찾음
//--------------------------------------------------------------------------
void GGroup::FindStartEndPt()
{
	//그룹 내의 그래피컬 개체들의 시작점과 끝점의 x,y좌표에 대한 배열을 만듬
	CArray<int, int&>* arrayX = new CArray<int, int&>; //x좌표에 대한 배열
	CArray<int, int&>* arrayY = new CArray<int, int&>; //y좌표에 대한 배열
	MakeCoordinateArray(arrayX, TRUE);
	MakeCoordinateArray(arrayY, FALSE);

	//위에서 만든 배열을 QuickSort로 정렬
	CQuickSort* qSort = new CQuickSort();
	qSort->QuickSort(arrayX, 0, arrayX->GetSize()-1);
	qSort->QuickSort(arrayY, 0, arrayY->GetSize()-1);

	//시작점 지정(x,y값 배열의 가장 작은 값이 시작점)
	m_sStartPoint.x = arrayX->GetAt(0);
	m_sStartPoint.y = arrayY->GetAt(0);

	//끝점 지정(x,y값 배열의 가장 큰 값이 끝점)
	m_sEndPoint.x = arrayX->GetAt(arrayX->GetSize()-1);
	m_sEndPoint.y = arrayY->GetAt(arrayX->GetSize()-1);

	delete qSort;
	delete arrayX;
	delete arrayY;
}

//--------------------------------------------------------------------------
//　그룹화된 그래피컬 개체들의 x좌표 또는 y좌표에 대한 배열을 만듬
//--------------------------------------------------------------------------
void GGroup::MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX)
{
	if(bIsX == TRUE)
	{
		//노드 데이터의 시작점과 끝점의 x값을 전부 array에 추가
		POSITION posNode = m_sNodeData.GetHeadPosition();
		while(posNode)
		{
			GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
			int x1 = pGObj->GetStartPoint().x;
			int x2 = pGObj->GetEndPoint().x;
			array->Add(x1); array->Add(x2);
		}
		//차일드 데이터의 시작점과 끝점의 x값을 전부 array에 추가(recursion이 일어남)
		POSITION posChild = m_sChild.GetHeadPosition();
		while(posChild)
		{
			GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);;
			pGGroup->MakeCoordinateArray(array, TRUE);
		}
	}
	else
	{
		//노드 데이터의 시작점과 끝점의 x값을 전부 array에 추가
		POSITION posNode = m_sNodeData.GetHeadPosition();
		while(posNode)
		{
			GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
			int y1 = pGObj->GetStartPoint().y;
			int y2 = pGObj->GetEndPoint().y;
			array->Add(y1); array->Add(y2);
		}
		//차일드 데이터의 시작점과 끝점의 x값을 전부 array에 추가(recursion이 일어남)
		POSITION posChild = m_sChild.GetHeadPosition();
		while(posChild)
		{
			GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
			pGGroup->MakeCoordinateArray(array, FALSE);
		}
	}
}

//--------------------------------------------------------------------------
//　리전 반환
//--------------------------------------------------------------------------
CRgn* GGroup::GetRegion()
{
	this->FindStartEndPt(); //현재 그룹의 제일 상단 지점과 제일 하단 지점을 찾음
	m_sRgn.DeleteObject();

	// 6. 5 리전 딱 들어맞아서 크기 약간 조정
	// 박현덕
	m_sRgn.CreateRectRgn(m_sStartPoint.x - m_nsThickness/2 - 10, m_sStartPoint.y - m_nsThickness/2 - 10,
		m_sEndPoint.x + m_nsThickness/2 + 10, m_sEndPoint.y + m_nsThickness/2 + 10); //리전 생성
	return &m_sRgn; //리전 반환
}

//--------------------------------------------------------------------------
//　이동
//--------------------------------------------------------------------------
void GGroup::Move(int dX, int dY)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->Move(dX, dY);
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->Move(dX, dY);
	}
}

//--------------------------------------------------------------------------
//　크기 변경
//--------------------------------------------------------------------------
void GGroup::Resize(int index, int dX, int dY)
{
	//노드 데이터 변경
 	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->Resize(index, dX, dY);
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->Resize(index, dX, dY);
	}
}

//--------------------------------------------------------------------------
//　포인트 위치 찾는 함수
//--------------------------------------------------------------------------
int GGroup::FindSelectPoint(CPoint pt)
{
	CRect rect(m_sStartPoint, m_sEndPoint);
	int tempThick = (GObject::m_nsThickness > 4 ? GObject::m_nsThickness : 5);

	CRgn sPointRgn[8];
	sPointRgn[0].CreateEllipticRgn(rect.left-tempThick/2-12, rect.top-tempThick/2-12, rect.left+tempThick/2+12, rect.top+tempThick/2+12);
	sPointRgn[1].CreateEllipticRgn(rect.left-tempThick/2-12, rect.CenterPoint().y-tempThick/2-12, rect.left+tempThick/2+12, rect.CenterPoint().y+tempThick/2+12);
	sPointRgn[2].CreateEllipticRgn(rect.left-tempThick/2-12, rect.bottom-tempThick/2-12, rect.left+tempThick/2+12, rect.bottom+tempThick/2+12);
	sPointRgn[3].CreateEllipticRgn(rect.CenterPoint().x-tempThick/2-12, rect.bottom-tempThick/2-12, rect.CenterPoint().x+tempThick/2+12, rect.bottom+tempThick/2+12);
	sPointRgn[4].CreateEllipticRgn(rect.right-tempThick/2-12, rect.bottom-tempThick/2-12, rect.right+tempThick/2+12, rect.bottom+tempThick/2+12);
	sPointRgn[5].CreateEllipticRgn(rect.right-tempThick/2-12, rect.CenterPoint().y-tempThick/2-12, rect.right+tempThick/2+12, rect.CenterPoint().y+tempThick/2+12);
	sPointRgn[6].CreateEllipticRgn(rect.right-tempThick/2-12, rect.top-tempThick/2-12, rect.right+tempThick/2+12, rect.top+tempThick/2+12);
	sPointRgn[7].CreateEllipticRgn(rect.CenterPoint().x-tempThick/2-12, rect.top-tempThick/2-12, rect.CenterPoint().x+tempThick/2+12, rect.top+tempThick/2+12);

	for(int i=0;i<8;i++)
		if(sPointRgn[i].PtInRegion(pt) == TRUE)
			return i;

	return -1;
}

//--------------------------------------------------------------------------
//　라인 컬러 설정
//--------------------------------------------------------------------------
void GGroup::SetGroupLineColor(COLORREF color)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetLineColor(color);
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupLineColor(color);
	}
}

//--------------------------------------------------------------------------
//　라인 굵기 설정
//--------------------------------------------------------------------------
void GGroup::SetGroupThickness(int thickness)
{
	m_nsThickness = thickness;
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetThickness(thickness);
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupThickness(thickness);
	}
}

//--------------------------------------------------------------------------
//　펜 스타일 설정
//--------------------------------------------------------------------------
void GGroup::SetPenStyle(int penStyle) 
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case LINE:
			{
				GLine* psGLine = (GLine*)psGObj;
				psGLine->SetPenStyle(penStyle);
				break;
			}
		case POLYLINE:
			{
				GPolyline* psGPolyline = (GPolyline*)psGObj;
				psGPolyline->SetPenStyle(penStyle);
				break;
			}
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetPenStyle(penStyle);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetPenStyle(penStyle);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetPenStyle(penStyle);
	}
}

//--------------------------------------------------------------------------
//　선 시작 모양 설정
//--------------------------------------------------------------------------
void GGroup::SetStartCap(int startCap)						
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case LINE:
			{
				GLine* psGLine = (GLine*)psGObj;
				psGLine->SetStartCap(startCap);
				break;
			}
		case POLYLINE:
			{
				GPolyline* psGPolyline = (GPolyline*)psGObj;
				psGPolyline->SetStartCap(startCap);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetStartCap(startCap);
	}
}

//--------------------------------------------------------------------------
//　선 끝 모양 설정
//--------------------------------------------------------------------------
void GGroup::SetEndCap(int endCap)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case LINE:
			{
				GLine* psGLine = (GLine*)psGObj;
				psGLine->SetEndCap(endCap);
				break;
			}
		case POLYLINE:
			{
				GPolyline* psGPolyline = (GPolyline*)psGObj;
				psGPolyline->SetEndCap(endCap);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetEndCap(endCap);
	}
}

//--------------------------------------------------------------------------
//　브러쉬 컬러 설정
//--------------------------------------------------------------------------
void GGroup::SetBrushColor(COLORREF brushColor)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetBrushColor(brushColor);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetBrushColor(brushColor);
				break;
			}
		case TEXT:
			{
				GText* psGText = (GText*)psGObj;
				psGText->SetBgColor(brushColor);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushColor(brushColor);
	}
}

//--------------------------------------------------------------------------
//　채우기 스타일 설정
//--------------------------------------------------------------------------
void GGroup::SetFaceType(int faceType)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetFaceType(faceType);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetFaceType(faceType);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetFaceType(faceType);
	}
}
//--------------------------------------------------------------------------
//　브러쉬 스타일 설정
//--------------------------------------------------------------------------
void GGroup::SetBrushStyle(int brushStyle)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetBrushStyle(brushStyle);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetBrushStyle(brushStyle);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushStyle(brushStyle);
	}
}

//--------------------------------------------------------------------------
//　해치 스타일 설정
//--------------------------------------------------------------------------
void GGroup::SetHatchStyle(int hatch)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetHatchStyle(hatch);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetBrushStyle(hatch);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushStyle(hatch);
	}
}

//--------------------------------------------------------------------------
//　라인 꺾임 처리 설정
//--------------------------------------------------------------------------
void GGroup::SetLineJoinStyle(int lineJoin)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetLineJoinStyle(lineJoin);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetLineJoinStyle(lineJoin);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetLineJoinStyle(lineJoin);
	}
}

//--------------------------------------------------------------------------
//　그라디언트 모드 설정
//--------------------------------------------------------------------------
void GGroup::SetGradientMode(int grdMode)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetGradientMode(grdMode);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetGradientMode(grdMode);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGradientMode(grdMode);
	}
}

//--------------------------------------------------------------------------
//　그라데이션의 끝 색 설정
//--------------------------------------------------------------------------
void GGroup::SetGrdEndColor(COLORREF grdEndColor)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(posNode);
		switch(psGObj->GetType())
		{
		case RECTANGLE:
			{
				GRectangle* psGRectangle = (GRectangle*)psGObj;
				psGRectangle->SetGrdEndColor(grdEndColor);
				break;
			}
		case ELLIPSE:
			{
				GEllipse* psGEllipse = (GEllipse*)psGObj;
				psGEllipse->SetGrdEndColor(grdEndColor);
				break;
			}
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGrdEndColor(grdEndColor);
	}
}
//--------------------------------------------------------------------------
//　불투명도 지정
//--------------------------------------------------------------------------
void GGroup::SetGroupAlpha(int nAlpha)
{
	m_nsAlpha = nAlpha;
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetAlpha(nAlpha);
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupThickness(nAlpha);
	}
}
//--------------------------------------------------------------------------
//　로그 폰트 지정
//--------------------------------------------------------------------------
void GGroup::SetLogFont(LOGFONT lf)
{
	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		if(pGObj->GetType() == TEXT)
		{
			GText* pGText = (GText*)pGObj;
			pGText->SetLogFont(lf);
		}
	}
	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetLogFont(lf);
	}
}

//--------------------------------------------------------------------------
//　그리기: nothing to do...
//--------------------------------------------------------------------------
void GGroup::Draw(CDC* pDC)
{
}

//--------------------------------------------------------------------------
//　선택 영역 점선 그리기 함수
//--------------------------------------------------------------------------
void GGroup::DrawSelectLine(CDC *pDC)
{
	//그리기 속성 설정
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //리전으로 사각형을 만듬
	pDC->Rectangle(rect); //사각형 점선을 그림

	//그리기 속성 지정
	CBrush brush(RGB(51,94,168)); //브러쉬 지정
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN); //NULL_PEN으로 지정

	//크기 조정시 사용할 점의 위치를 구함
	CPoint sPointArray[8];
	sPointArray[0] = rect.TopLeft();
	sPointArray[1] = CPoint(rect.left, rect.CenterPoint().y);
	sPointArray[2] = CPoint(rect.left, rect.bottom);
	sPointArray[3] = CPoint(rect.CenterPoint().x, rect.bottom);
	sPointArray[4] = CPoint(rect.BottomRight());
	sPointArray[5] = CPoint(rect.right, rect.CenterPoint().y);
	sPointArray[6] = CPoint(rect.right, rect.top);
	sPointArray[7] = CPoint(rect.CenterPoint().x, rect.top);
	//크기 조정을 위한 점 그리기
	for(int i=0;i<8;i++)
		pDC->Ellipse(sPointArray[i].x-5, sPointArray[i].y-5, sPointArray[i].x+5, sPointArray[i].y+5);

	//이전 펜과 브러쉬 속성으로 되돌림
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

//--------------------------------------------------------------------------
//　그룹 리사이즈 시작 시 호출 by 박현덕
//--------------------------------------------------------------------------
void GGroup::StartResizing(int x, int y, int direction) {

	// 그룹의 리사이징 시작시
	// 각각의 정보를 읽어옴 너비, 높이
	int width = m_sEndPoint.x - m_sStartPoint.x;
	int height = m_sEndPoint.y - m_sStartPoint.y;

	this->m_pResizeX = x;
	this->m_pReszieY = y;
	this->m_pResizeWidth = width;
	this->m_pResizeHeight = height;
	this->m_pResizeDirection = direction;

	POSITION pos = m_sNodeData.GetHeadPosition();
	while (pos) {
		GObject *obj = (GObject *)m_sNodeData.GetNext(pos);
		if(obj->GetType() == POLYLINE)
			((GPolyline*)obj)->SavePolylinePoints();
		else
			obj->SavePoints();
	}

	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->StartChildResizing(x, y, width, height, direction);
	}
}

//--------------------------------------------------------------------------
//　그룹 차일드 리사이즈
//--------------------------------------------------------------------------
void GGroup::StartChildResizing(int x, int y, int width, int height, int direction)
{
	this->m_pResizeX = x;
	this->m_pReszieY = y;
	this->m_pResizeWidth = width;
	this->m_pResizeHeight = height;
	this->m_pResizeDirection = direction;

	POSITION pos = m_sNodeData.GetHeadPosition();
	while (pos) {
		GObject *obj = (GObject *)m_sNodeData.GetNext(pos);
		if(obj->GetType() == POLYLINE)
			((GPolyline*)obj)->SavePolylinePoints();
		else
			obj->SavePoints();
	}

	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->StartChildResizing(x, y, width, height, direction);
	}
}

//--------------------------------------------------------------------------
//　최종 리사이징 부분
//--------------------------------------------------------------------------
void GGroup::DoResizing(int x, int y) {

	int dw = 0, dh = 0;
	bool dx = false, dy = false;

	// 리사이징의 방향결정
	// 테스트버전
	// 반 시계방향으로 0 ~ 8
	switch (this->m_pResizeDirection) {
		case 0: case 1: case 2:
			 dw = this->m_pResizeX - x; dx = true;
			 break;

		case 4: case 5: case 6:
			 dw = x - this->m_pResizeX;
			 break;
	}

	switch (this->m_pResizeDirection) {
		case 0: case 6: case 7:
			 dh = this->m_pReszieY - y; dy = true;
			 break;

		case 2: case 3: case 4:
			 dh = y - this->m_pReszieY;
			 break;
	}

	int width = this->m_pResizeWidth + dw;
	int height = this->m_pResizeHeight + dh;

	// width, height가 0이 되면 더이상 크기를 조정하지 않음
	if (width <= 0 || height <= 0) {
		return;
	}
	
	double px = (double)width / this->m_pResizeWidth;
	double py = (double)height / this->m_pResizeHeight;

	//노드 데이터 변경
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while (posNode) {
		GObject *pObj = (GObject*)m_sNodeData.GetNext(posNode);
		if(pObj->GetType() == POLYLINE)
			((GPolyline*)pObj)->ApplyPolylinePoints(px, py, dx ? m_sEndPoint.x : m_sStartPoint.x, dy ? m_sEndPoint.y : m_sStartPoint.y);
		else
			pObj->ApplyPoints(px, py, dx ? m_sEndPoint.x : m_sStartPoint.x, dy ? m_sEndPoint.y : m_sStartPoint.y);
	}

	//차일드 데이터 변경(recursion이 일어남)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->DoResizing(x, y);
	}

	FindStartEndPt();
}