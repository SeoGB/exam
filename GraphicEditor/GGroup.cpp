/* [ GGroup Ŭ���� by ����ö ]
** GGroup Ŭ������ ������ �Դϴ�.
*/

#include "StdAfx.h"
#include "GGroup.h"

IMPLEMENT_SERIAL(GGroup, GObject, 0)
// GGroup Ŭ���� ����
//--------------------------------------------------------------------------
//��GGroup ������ �� �Ҹ���
//--------------------------------------------------------------------------
//������
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

//�Ҹ���
GGroup::~GGroup(void)
{
}

//--------------------------------------------------------------------------
//������ȭ
//--------------------------------------------------------------------------
void GGroup::Serialize(CArchive& ar)
{
	GObject::Serialize(ar);
	if(ar.IsStoring())	//���� ��
	{
		m_sNodeData.Serialize(ar); 
		m_sChild.Serialize(ar);
	}
	else //�ε� ��
	{
		m_sNodeData.Serialize(ar);
		m_sChild.Serialize(ar);
	}
}

//--------------------------------------------------------------------------
//��Group �߰� �Լ�
//--------------------------------------------------------------------------
void GGroup::Add(CTypedPtrList<CObList, GObject*>& listGObj)
{
	GObject* pGObj;
	POSITION pos = listGObj.GetHeadPosition();
	while(pos)
	{
		pGObj = (GObject*)listGObj.GetNext(pos);
		pGObj->SetGrouped(TRUE); //���� ��ü�� �׷��� �Ǿ����� ǥ��
		if(pGObj->GetType() == GGROUP) //�׷��� ���
		{
			m_sChild.AddTail((GGroup*)pGObj); //���ϵ忡 �߰�
		}
		else //�׷��� �ƴ� ���
		{
			m_sNodeData.AddTail(pGObj); //��� �����Ϳ� �߰�
		}
	}
}

//--------------------------------------------------------------------------
//��Group ���� �Լ�
//--------------------------------------------------------------------------
void GGroup::Remove(CTypedPtrList<CObList, GObject*>& listGObj)
{
	POSITION pos = listGObj.GetHeadPosition();
	while(pos)
	{
		GObject* pGObj = (GObject*)listGObj.GetNext(pos);
		pGObj->SetGrouped(FALSE); //����Ʈ���� ������ ��ü�� �׷��� �ƴ��� ǥ��
		if(pGObj->GetType() == GGROUP) //�׷��� ���
		{
			//���ϵ忡�� ����
			POSITION delPos = m_sChild.Find(pGObj, NULL);
			m_sChild.RemoveAt(delPos);
		}
		else //�׷��� �ƴ� ���
		{
			//��� �����Ϳ��� ����
			POSITION delPos = m_sNodeData.Find(pGObj, NULL); 
			m_sNodeData.RemoveAt(delPos);
		}
	}
}
//--------------------------------------------------------------------------
//���׷� ����
//--------------------------------------------------------------------------
void GGroup::UnGroup(CTypedPtrList<CObList, GObject*>* listGObj)
{
	//��� ������ �κ��� �׷� ����
	POSITION posNode = this->m_sNodeData.GetHeadPosition();
	while(posNode) //��� ������ ����Ʈ ��ȸ
	{
		GObject* pGObj = (GObject*)this->m_sNodeData.GetNext(posNode); 
		pGObj->SetGrouped(FALSE); //�׷��� �ƴ��� ǥ��
		listGObj->AddTail(pGObj); //���� ����Ʈ�� ����
	}
	this->m_sNodeData.RemoveAll();
	
	//���ϵ� �κ��� �׷� ����
	POSITION posChild = this->m_sChild.GetHeadPosition();
	while(posChild) //���ϵ� ����Ʈ ��ȸ
	{
		GGroup* pGObj = (GGroup*)this->m_sChild.GetNext(posChild);
		pGObj->SetGrouped(FALSE); //�׷��� �ƴ��� ǥ��
		listGObj->AddTail(pGObj); //���� ����Ʈ�� ����
		pGObj->UnGroupChild(*pGObj, listGObj); //���ϵ�� �׷����� �Լ� ȣ��
	}
	delete this;
}

//--------------------------------------------------------------------------
//���׷� ����(���ϵ�)
//--------------------------------------------------------------------------
void GGroup::UnGroupChild(GGroup& childGroup, CTypedPtrList<CObList, GObject*>* listGObj)
{
	POSITION pos = childGroup.m_sChild.GetHeadPosition();
	while(pos) //�Ű������� ���� ���ϵ� ����Ʈ ��ȸ
	{
		GGroup* pGObj = (GGroup*)childGroup.m_sChild.GetNext(pos);
		listGObj->AddTail(pGObj); //���� ����Ʈ�� ����
		pGObj->UnGroupChild(*pGObj, listGObj); //recursion ��Ŵ
	}
}

//--------------------------------------------------------------------------
//�����ϵ忡 ���� �����͸� ��� �Լ�
//--------------------------------------------------------------------------
void GGroup::GetChild(CTypedPtrList<CObList, GObject*>* listGObj)
{
	//���ϵ� �κ��� �׷� ����
	POSITION posChild = this->m_sChild.GetHeadPosition();
	while(posChild) //���ϵ� ����Ʈ ��ȸ
	{
		GGroup* pGObj = (GGroup*)this->m_sChild.GetNext(posChild);
		listGObj->AddTail(pGObj); //����Ʈ�� ����
		pGObj->GetChild(listGObj); //���ϵ� �����͸� �޾ƿ��� �Լ� ��ȣ��
	}
}

//--------------------------------------------------------------------------
//���׷� �����͸� �Ű������� listGObj�� ���� 
//--------------------------------------------------------------------------
void GGroup::GetGroupData(CTypedPtrList<CObList, GObject*>* listGObj)
{
	POSITION pos = m_sNodeData.GetHeadPosition();
	while(pos) //��� ������ ����Ʈ ��ȸ
	{
		GObject* psGObj = (GObject*)m_sNodeData.GetNext(pos);
		listGObj->AddTail(psGObj);
	}

	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild) //���ϵ� ����Ʈ ��ȸ
	{
		GGroup* psGGroup = (GGroup*)m_sChild.GetNext(posChild);
		listGObj->AddTail(psGGroup);
		psGGroup->GetGroupData(listGObj);
	}
}

//--------------------------------------------------------------------------
//���������� �˻�
//--------------------------------------------------------------------------
BOOL GGroup::IsLeaf()
{
	if(m_sChild.GetSize() == 0) //���ϵ忡 �����Ͱ� ������ Leaf
		return TRUE;
	else //���ϵ忡 �����Ͱ� ������ non-leaf
		return FALSE;
}

//--------------------------------------------------------------------------
//���׷��� left,top ��ǥ�� right,bottom ��ǥ�� ã��
//--------------------------------------------------------------------------
void GGroup::FindStartEndPt()
{
	//�׷� ���� �׷����� ��ü���� �������� ������ x,y��ǥ�� ���� �迭�� ����
	CArray<int, int&>* arrayX = new CArray<int, int&>; //x��ǥ�� ���� �迭
	CArray<int, int&>* arrayY = new CArray<int, int&>; //y��ǥ�� ���� �迭
	MakeCoordinateArray(arrayX, TRUE);
	MakeCoordinateArray(arrayY, FALSE);

	//������ ���� �迭�� QuickSort�� ����
	CQuickSort* qSort = new CQuickSort();
	qSort->QuickSort(arrayX, 0, arrayX->GetSize()-1);
	qSort->QuickSort(arrayY, 0, arrayY->GetSize()-1);

	//������ ����(x,y�� �迭�� ���� ���� ���� ������)
	m_sStartPoint.x = arrayX->GetAt(0);
	m_sStartPoint.y = arrayY->GetAt(0);

	//���� ����(x,y�� �迭�� ���� ū ���� ����)
	m_sEndPoint.x = arrayX->GetAt(arrayX->GetSize()-1);
	m_sEndPoint.y = arrayY->GetAt(arrayX->GetSize()-1);

	delete qSort;
	delete arrayX;
	delete arrayY;
}

//--------------------------------------------------------------------------
//���׷�ȭ�� �׷����� ��ü���� x��ǥ �Ǵ� y��ǥ�� ���� �迭�� ����
//--------------------------------------------------------------------------
void GGroup::MakeCoordinateArray(CArray<int, int&>* array, BOOL bIsX)
{
	if(bIsX == TRUE)
	{
		//��� �������� �������� ������ x���� ���� array�� �߰�
		POSITION posNode = m_sNodeData.GetHeadPosition();
		while(posNode)
		{
			GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
			int x1 = pGObj->GetStartPoint().x;
			int x2 = pGObj->GetEndPoint().x;
			array->Add(x1); array->Add(x2);
		}
		//���ϵ� �������� �������� ������ x���� ���� array�� �߰�(recursion�� �Ͼ)
		POSITION posChild = m_sChild.GetHeadPosition();
		while(posChild)
		{
			GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);;
			pGGroup->MakeCoordinateArray(array, TRUE);
		}
	}
	else
	{
		//��� �������� �������� ������ x���� ���� array�� �߰�
		POSITION posNode = m_sNodeData.GetHeadPosition();
		while(posNode)
		{
			GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
			int y1 = pGObj->GetStartPoint().y;
			int y2 = pGObj->GetEndPoint().y;
			array->Add(y1); array->Add(y2);
		}
		//���ϵ� �������� �������� ������ x���� ���� array�� �߰�(recursion�� �Ͼ)
		POSITION posChild = m_sChild.GetHeadPosition();
		while(posChild)
		{
			GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
			pGGroup->MakeCoordinateArray(array, FALSE);
		}
	}
}

//--------------------------------------------------------------------------
//������ ��ȯ
//--------------------------------------------------------------------------
CRgn* GGroup::GetRegion()
{
	this->FindStartEndPt(); //���� �׷��� ���� ��� ������ ���� �ϴ� ������ ã��
	m_sRgn.DeleteObject();

	// 6. 5 ���� �� ���¾Ƽ� ũ�� �ణ ����
	// ������
	m_sRgn.CreateRectRgn(m_sStartPoint.x - m_nsThickness/2 - 10, m_sStartPoint.y - m_nsThickness/2 - 10,
		m_sEndPoint.x + m_nsThickness/2 + 10, m_sEndPoint.y + m_nsThickness/2 + 10); //���� ����
	return &m_sRgn; //���� ��ȯ
}

//--------------------------------------------------------------------------
//���̵�
//--------------------------------------------------------------------------
void GGroup::Move(int dX, int dY)
{
	//��� ������ ����
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->Move(dX, dY);
	}
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->Move(dX, dY);
	}
}

//--------------------------------------------------------------------------
//��ũ�� ����
//--------------------------------------------------------------------------
void GGroup::Resize(int index, int dX, int dY)
{
	//��� ������ ����
 	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->Resize(index, dX, dY);
	}
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->Resize(index, dX, dY);
	}
}

//--------------------------------------------------------------------------
//������Ʈ ��ġ ã�� �Լ�
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
//������ �÷� ����
//--------------------------------------------------------------------------
void GGroup::SetGroupLineColor(COLORREF color)
{
	//��� ������ ����
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetLineColor(color);
	}
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupLineColor(color);
	}
}

//--------------------------------------------------------------------------
//������ ���� ����
//--------------------------------------------------------------------------
void GGroup::SetGroupThickness(int thickness)
{
	m_nsThickness = thickness;
	//��� ������ ����
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetThickness(thickness);
	}
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupThickness(thickness);
	}
}

//--------------------------------------------------------------------------
//���� ��Ÿ�� ����
//--------------------------------------------------------------------------
void GGroup::SetPenStyle(int penStyle) 
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetPenStyle(penStyle);
	}
}

//--------------------------------------------------------------------------
//���� ���� ��� ����
//--------------------------------------------------------------------------
void GGroup::SetStartCap(int startCap)						
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetStartCap(startCap);
	}
}

//--------------------------------------------------------------------------
//���� �� ��� ����
//--------------------------------------------------------------------------
void GGroup::SetEndCap(int endCap)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetEndCap(endCap);
	}
}

//--------------------------------------------------------------------------
//���귯�� �÷� ����
//--------------------------------------------------------------------------
void GGroup::SetBrushColor(COLORREF brushColor)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushColor(brushColor);
	}
}

//--------------------------------------------------------------------------
//��ä��� ��Ÿ�� ����
//--------------------------------------------------------------------------
void GGroup::SetFaceType(int faceType)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetFaceType(faceType);
	}
}
//--------------------------------------------------------------------------
//���귯�� ��Ÿ�� ����
//--------------------------------------------------------------------------
void GGroup::SetBrushStyle(int brushStyle)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushStyle(brushStyle);
	}
}

//--------------------------------------------------------------------------
//����ġ ��Ÿ�� ����
//--------------------------------------------------------------------------
void GGroup::SetHatchStyle(int hatch)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetBrushStyle(hatch);
	}
}

//--------------------------------------------------------------------------
//������ ���� ó�� ����
//--------------------------------------------------------------------------
void GGroup::SetLineJoinStyle(int lineJoin)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetLineJoinStyle(lineJoin);
	}
}

//--------------------------------------------------------------------------
//���׶���Ʈ ��� ����
//--------------------------------------------------------------------------
void GGroup::SetGradientMode(int grdMode)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGradientMode(grdMode);
	}
}

//--------------------------------------------------------------------------
//���׶��̼��� �� �� ����
//--------------------------------------------------------------------------
void GGroup::SetGrdEndColor(COLORREF grdEndColor)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGrdEndColor(grdEndColor);
	}
}
//--------------------------------------------------------------------------
//�������� ����
//--------------------------------------------------------------------------
void GGroup::SetGroupAlpha(int nAlpha)
{
	m_nsAlpha = nAlpha;
	//��� ������ ����
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while(posNode)
	{
		GObject* pGObj = (GObject*)m_sNodeData.GetNext(posNode);
		pGObj->SetAlpha(nAlpha);
	}
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetGroupThickness(nAlpha);
	}
}
//--------------------------------------------------------------------------
//���α� ��Ʈ ����
//--------------------------------------------------------------------------
void GGroup::SetLogFont(LOGFONT lf)
{
	//��� ������ ����
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
	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->SetLogFont(lf);
	}
}

//--------------------------------------------------------------------------
//���׸���: nothing to do...
//--------------------------------------------------------------------------
void GGroup::Draw(CDC* pDC)
{
}

//--------------------------------------------------------------------------
//������ ���� ���� �׸��� �Լ�
//--------------------------------------------------------------------------
void GGroup::DrawSelectLine(CDC *pDC)
{
	//�׸��� �Ӽ� ����
	pDC->SelectStockObject(NULL_BRUSH);
	CPen penDotted(PS_DOT, 1, RGB(51,94,168));
	CPen* oldPen = pDC->SelectObject(&penDotted);

	CRect rect;
	GetRegion()->GetRgnBox(rect); //�������� �簢���� ����
	pDC->Rectangle(rect); //�簢�� ������ �׸�

	//�׸��� �Ӽ� ����
	CBrush brush(RGB(51,94,168)); //�귯�� ����
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN); //NULL_PEN���� ����

	//ũ�� ������ ����� ���� ��ġ�� ����
	CPoint sPointArray[8];
	sPointArray[0] = rect.TopLeft();
	sPointArray[1] = CPoint(rect.left, rect.CenterPoint().y);
	sPointArray[2] = CPoint(rect.left, rect.bottom);
	sPointArray[3] = CPoint(rect.CenterPoint().x, rect.bottom);
	sPointArray[4] = CPoint(rect.BottomRight());
	sPointArray[5] = CPoint(rect.right, rect.CenterPoint().y);
	sPointArray[6] = CPoint(rect.right, rect.top);
	sPointArray[7] = CPoint(rect.CenterPoint().x, rect.top);
	//ũ�� ������ ���� �� �׸���
	for(int i=0;i<8;i++)
		pDC->Ellipse(sPointArray[i].x-5, sPointArray[i].y-5, sPointArray[i].x+5, sPointArray[i].y+5);

	//���� ��� �귯�� �Ӽ����� �ǵ���
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

//--------------------------------------------------------------------------
//���׷� �������� ���� �� ȣ�� by ������
//--------------------------------------------------------------------------
void GGroup::StartResizing(int x, int y, int direction) {

	// �׷��� ������¡ ���۽�
	// ������ ������ �о�� �ʺ�, ����
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

	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->StartChildResizing(x, y, width, height, direction);
	}
}

//--------------------------------------------------------------------------
//���׷� ���ϵ� ��������
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
//������ ������¡ �κ�
//--------------------------------------------------------------------------
void GGroup::DoResizing(int x, int y) {

	int dw = 0, dh = 0;
	bool dx = false, dy = false;

	// ������¡�� �������
	// �׽�Ʈ����
	// �� �ð�������� 0 ~ 8
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

	// width, height�� 0�� �Ǹ� ���̻� ũ�⸦ �������� ����
	if (width <= 0 || height <= 0) {
		return;
	}
	
	double px = (double)width / this->m_pResizeWidth;
	double py = (double)height / this->m_pResizeHeight;

	//��� ������ ����
	POSITION posNode = m_sNodeData.GetHeadPosition();
	while (posNode) {
		GObject *pObj = (GObject*)m_sNodeData.GetNext(posNode);
		if(pObj->GetType() == POLYLINE)
			((GPolyline*)pObj)->ApplyPolylinePoints(px, py, dx ? m_sEndPoint.x : m_sStartPoint.x, dy ? m_sEndPoint.y : m_sStartPoint.y);
		else
			pObj->ApplyPoints(px, py, dx ? m_sEndPoint.x : m_sStartPoint.x, dy ? m_sEndPoint.y : m_sStartPoint.y);
	}

	//���ϵ� ������ ����(recursion�� �Ͼ)
	POSITION posChild = m_sChild.GetHeadPosition();
	while(posChild)
	{
		GGroup* pGGroup = (GGroup*)m_sChild.GetNext(posChild);
		pGGroup->DoResizing(x, y);
	}

	FindStartEndPt();
}