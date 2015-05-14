/* [ CUndo Ŭ���� by ����ö ]
** Undo/Redo ����� �����ϴ� Ŭ�����Դϴ�.
** ��ť��Ʈ�� �ش� Ŭ������ ��ӹ޵��� �Ͽ�,
** üũ ����Ʈ �߻� ��(�׸��⳪ �̵�,ũ������,������ ���� �۾�)����
** ����ȭ ������� ���� ���¸� �޸𸮿� �����մϴ�.
** �޸𸮿� ������ ���� �����ϴ� ����Ʈ�� �ξ� Undo/Redo�� �����ϰ� �մϴ�.
*/

#pragma once
class CUndo
{	
public:
	//Undo/Redo�� ����ȭ�� �̿��ϹǷ� �� Ŭ������ ��� ��������
	//�Ʒ� 2���� �����Լ��� �������̵� �Ǿ�� ��
	virtual void Serialize(CArchive& ar) = 0;
	virtual void DeleteContents() = 0;

	//�ܺ� ���� ���� �Լ�
	CUndo(long undolevels = 256, long growsize = 32768);	// ������
	~CUndo();			// �Ҹ���
	BOOL CanUndo();		// Undo ���� ����
	BOOL CanRedo();		// Redo ���� ����
	void Undo();		// Undo ����
	void Redo();		// Redo ����
	void CheckPoint();	// ���� ���� ����(������ ����� ���� ������ ��) 
	void EnableCheckPoint();  //üũ����Ʈ �����ϰ� ����
	void DisableCheckPoint(); //üũ����Ʈ �Ұ����ϰ� ����

private:
	CObList	m_undolist;		// Undo ���� ����
	CObList	m_redolist;		// Redo ���� ����
	long	m_growsize;		// ������ ���� ����
	long	m_undoLevels;	// Undo Ƚ�� ����
	long	m_chkpt;		// üũ ����Ʈ

	void AddUndo(CMemFile*);		//Undo �߰�
	void AddRedo(CMemFile *pFile);	//Redo �߰�
	void Load(CMemFile*);			//�ε�
	void Store(CMemFile*);			//����
	void ClearRedoList();			//Redolist �ʱ�ȭ
};
//--------------------------------------------------------------------------
// ������ �� �Ҹ���
//--------------------------------------------------------------------------
//������
inline CUndo::CUndo(long undolevels, long growsize) 
	: m_undoLevels(undolevels), m_growsize(growsize)
{
	m_chkpt = 0;
}
//�Ҹ���
inline CUndo::~CUndo() 
{
	// undo list �ʱ�ȭ
	POSITION pos = m_undolist.GetHeadPosition(); 
	CMemFile  *nextFile = NULL;
	while(pos)
	{
		nextFile = (CMemFile *) m_undolist.GetNext(pos);
		delete nextFile;
	}
	m_undolist.RemoveAll();	

	// redo list �ʱ�ȭ
	ClearRedoList();
}

//--------------------------------------------------------------------------
//��Redo ����Ʈ�� ���� ����
//--------------------------------------------------------------------------
inline void CUndo::ClearRedoList()
{
	POSITION pos = m_redolist.GetHeadPosition(); 
	CMemFile* nextFile = NULL;
	while(pos)
	{
		nextFile = (CMemFile *) m_redolist.GetNext(pos);
		delete nextFile;
	}
	m_redolist.RemoveAll();	
}

//--------------------------------------------------------------------------
//��Undo ���� ���� üũ
//--------------------------------------------------------------------------
inline BOOL CUndo::CanUndo()
{
	return (m_undolist.GetCount() > 1);
}

//--------------------------------------------------------------------------
//��Redo ���� ���� üũ
//--------------------------------------------------------------------------
inline BOOL CUndo::CanRedo() 
{
	return (m_redolist.GetCount() > 0);
}

//--------------------------------------------------------------------------
//��Undo ����Ʈ�� �ǵ��� ���� �߰�		
//--------------------------------------------------------------------------
inline void CUndo::AddUndo(CMemFile* file) 
{
	// undo Ƚ������ Ŀ���� ���� ���¸� ����
	if (m_undolist.GetCount() > m_undoLevels)
	{
		CMemFile* pFile = (CMemFile *) m_undolist.RemoveTail();
		delete pFile;
	}
	m_undolist.AddHead(file); //���ο� ���¸� head�� �߰�
}

//--------------------------------------------------------------------------
//������ ���¸� ����
//--------------------------------------------------------------------------
inline void CUndo::Store(CMemFile* file) 
{
	file->SeekToBegin();
	CArchive ar(file, CArchive::store);
	Serialize(ar); 
	ar.Close();
}
//--------------------------------------------------------------------------
//�������� ���¸� �ε�
//--------------------------------------------------------------------------
inline void CUndo::Load(CMemFile* file) 
{
	DeleteContents(); 
	file->SeekToBegin();
	CArchive ar(file, CArchive::load);
	Serialize(ar); 
	ar.Close();
}
//--------------------------------------------------------------------------
//������ ���¸� Undo ����Ʈ�� ����
//--------------------------------------------------------------------------
inline void CUndo::CheckPoint() 
{
	if (m_chkpt <= 0)
	{
		CMemFile* file = new CMemFile(m_growsize);
		Store(file);
		AddUndo(file);
		ClearRedoList();
	}
}

//--------------------------------------------------------------------------
//��üũ����Ʈ Ȱ��ȭ/��Ȱ��ȭ
//--------------------------------------------------------------------------
inline void CUndo::EnableCheckPoint()
{
	if (m_chkpt > 0) {
		m_chkpt--;
	}
}

inline void CUndo::DisableCheckPoint()
{
	m_chkpt++;
}

//--------------------------------------------------------------------------
//��redo ����Ʈ�� ���� �߰�
//--------------------------------------------------------------------------
inline void CUndo::AddRedo(CMemFile *file) 
{
	m_redolist.AddHead(file);
}

//--------------------------------------------------------------------------
//��Undo ���
//--------------------------------------------------------------------------
inline void CUndo::Undo() 
{
	if (CanUndo()) //Undo�� ������ �����̸�,
	{
		CMemFile *pFile = (CMemFile *) m_undolist.GetHead(); //undo ����Ʈ�� ���� ó���� ���� ����
		m_undolist.RemoveHead(); //���� ���¸� ����
		AddRedo(pFile); //���� ���¸� Redo ����Ʈ�� ����
		//���� ���¸� �ҷ��� load
		pFile = (CMemFile *)m_undolist.GetHead();
		Load(pFile);
	}
}
//--------------------------------------------------------------------------
//��Redo ���
//--------------------------------------------------------------------------
inline void CUndo::Redo() 
{
	if (CanRedo()) //Redo�� ������ �����̸�,
	{
		CMemFile *pFile = (CMemFile *) m_redolist.GetHead(); //redo ����Ʈ�� ���� ó���� ���� ����
		m_redolist.RemoveHead(); //���� ���¸� ����
		AddUndo(pFile); //���� ���¸� Uedo ����Ʈ�� ����
		Load(pFile); //���� ���¸� load
	}
}
