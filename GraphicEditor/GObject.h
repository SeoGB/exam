/* [ GObject Ŭ���� by ����ö ]
** �׷����� ��ü�� ���� �ֻ��� Ŭ�����Դϴ�.
** Line, Polyline, Rectangle, Ellipse, Text, Group�� ��� �� Ŭ������ ��ӹ޾� ���ǵ˴ϴ�.
*/

#pragma once
enum GraphicalObjectType
{
	SELECT,
	LINE,
	POLYLINE,
	ELLIPSE,
	RECTANGLE,
	TEXT,
	GGROUP
}; //Ÿ�� ���� �� ���

enum GBrushType
{
	SOLID,		//�ָ��� �귯��
	HATCH,		//��ġ �귯��
	GRADIENT	//�׶���Ʈ �귯��
}; //�귯�� Ÿ�� ���� �� ���

enum GFaceType
{
	OUTLINE,		//�ܰ����� �׸���
	OUTLINEDFILL,	//�ܰ���+ä����ؼ� �׸���
	FILLONLY		//ä��⸸ �ϱ�
}; //���� �׸��� �� ���

class GObject : public CObject
{
protected:
	DECLARE_SERIAL(GObject);
public:		//��� �Լ�
	//������ & �Ҹ���
	GObject(void);
	~GObject(void);

	virtual void Serialize(CArchive& ar);						//����ȭ �Լ�

	//Accessor �Լ�
	GraphicalObjectType GetType(void) const { return m_sType; }	//�׷����� ��ü�� Ÿ���� ��ȯ
	
	CPoint GetStartPoint(void) const { return m_sStartPoint; }	//������ ��ȯ
	CPoint GetEndPoint(void) const { return m_sEndPoint;}		//������ ��ȯ
	
	COLORREF GetLineColor(void) const { return m_sLineColor; }	//���� �� ��ȯ
	int GetThickness(void) const { return m_nsThickness; }		//���� ���� ��ȯ
	
	int GetAplha(void) const { return m_nsAlpha; }				//������ ��ȯ

	BOOL PtInGObj(CPoint pt);									//Ŭ�� ������ �����ȿ� �ִ��� ���� ��ȯ
	virtual CRgn* GetRegion() { return &m_sRgn; }				//�׷����� ��ü�� ������ ����

	BOOL IsGrouped(void) const { return m_bsGrouped; }			//�׷� ���� ��ȯ

	Color COLORREFtoColor(COLORREF color);						//COLORREF���� GDI+���� ����ϴ� Color������ ��ȯ
	Point CPointToPoint(CPoint point);							//CPoint���� GDI+���� ����ϴ� Point������ ��ȯ

	//Mutator �Լ�
	void SetStartPoint(CPoint pt);								//������ ����
	void SetEndPoint(CPoint endPoint);							//���� ����
	void SetBothPoint(CPoint startPt, CPoint endPt);			//�� ������ ����
	
	virtual void Move(int dX, int dY) {};						//��ǥ �̵�
	virtual void Resize(int index, int dX, int dY) {}			//ũ��(�Ǵ� �� ��ġ) ����
	virtual int FindSelectPoint(CPoint pt) { return -1; }		//������ ���� ã�� �Լ�

	void SetLineColor(COLORREF color);							//���� �� ����
	void SetThickness(int thickness);							//���� ���� ����
	void SetGrouped(BOOL isGrouped);							//�׷� ���� ����

	void SetDashStyle(Pen& pen, int nPenStyle);					//�� ����

	void SetAlpha(int alpha);									//������ ����

	//�׸��� �Լ�
	virtual void Draw(CDC* cdc) {}								//�� �Լ����� �׷����� ��ü �׸�
	virtual void DrawSelectLine(CDC *pDC){}						//���� �� �������� ������ �׸�

	// PHD
	void SavePoints();
	void ApplyPoints(double px, double py, int sx, int sy);

	CPoint m_pResizeStart, m_pResizeEnd;

protected:	//��� ����
	GraphicalObjectType m_sType;	//�׷����� ��ü�� Ÿ��
	CPoint m_sStartPoint;			//������(left,top�� ��ǥ)
	CPoint m_sEndPoint;				//������(right,bottom�� ��ǥ)
	COLORREF m_sLineColor;			//�� ��
	int m_nsThickness;				//�� ����
	CRgn m_sRgn;					//�׷����� ��ü�� ����
	BOOL m_bsGrouped;				//�׷�ȭ ����

	//for GDI+, 120528 �߰� by ����ö
	int m_nsAlpha;					//������
};
