/* [ CUndo 클래스 by 신희철 ]
** Undo/Redo 기능을 수행하는 클래스입니다.
** 도큐먼트에 해당 클래스를 상속받도록 하여,
** 체크 포인트 발생 시(그리기나 이동,크기조정,색변경 등의 작업)마다
** 직렬화 기능으로 현재 상태를 메모리에 저장합니다.
** 메모리에 저장한 것을 관리하는 리스트를 두어 Undo/Redo가 가능하게 합니다.
*/

#pragma once
class CUndo
{	
public:
	//Undo/Redo는 직렬화를 이용하므로 이 클래스를 상속 받으려면
	//아래 2개의 가상함수가 오버라이드 되어야 함
	virtual void Serialize(CArchive& ar) = 0;
	virtual void DeleteContents() = 0;

	//외부 접근 가능 함수
	CUndo(long undolevels = 256, long growsize = 32768);	// 생성자
	~CUndo();			// 소멸자
	BOOL CanUndo();		// Undo 가능 여부
	BOOL CanRedo();		// Redo 가능 여부
	void Undo();		// Undo 수행
	void Redo();		// Redo 수행
	void CheckPoint();	// 현재 상태 저장(데이터 변경시 마다 적용할 것) 
	void EnableCheckPoint();  //체크포인트 가능하게 변경
	void DisableCheckPoint(); //체크포인트 불가능하게 변경

private:
	CObList	m_undolist;		// Undo 상태 저장
	CObList	m_redolist;		// Redo 상태 저장
	long	m_growsize;		// 저장을 위한 조정
	long	m_undoLevels;	// Undo 횟수 제한
	long	m_chkpt;		// 체크 포인트

	void AddUndo(CMemFile*);		//Undo 추가
	void AddRedo(CMemFile *pFile);	//Redo 추가
	void Load(CMemFile*);			//로드
	void Store(CMemFile*);			//저장
	void ClearRedoList();			//Redolist 초기화
};
//--------------------------------------------------------------------------
// 생성자 및 소멸자
//--------------------------------------------------------------------------
//생성자
inline CUndo::CUndo(long undolevels, long growsize) 
	: m_undoLevels(undolevels), m_growsize(growsize)
{
	m_chkpt = 0;
}
//소멸자
inline CUndo::~CUndo() 
{
	// undo list 초기화
	POSITION pos = m_undolist.GetHeadPosition(); 
	CMemFile  *nextFile = NULL;
	while(pos)
	{
		nextFile = (CMemFile *) m_undolist.GetNext(pos);
		delete nextFile;
	}
	m_undolist.RemoveAll();	

	// redo list 초기화
	ClearRedoList();
}

//--------------------------------------------------------------------------
//　Redo 리스트의 내용 제거
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
//　Undo 가능 여부 체크
//--------------------------------------------------------------------------
inline BOOL CUndo::CanUndo()
{
	return (m_undolist.GetCount() > 1);
}

//--------------------------------------------------------------------------
//　Redo 가능 여부 체크
//--------------------------------------------------------------------------
inline BOOL CUndo::CanRedo() 
{
	return (m_redolist.GetCount() > 0);
}

//--------------------------------------------------------------------------
//　Undo 리스트에 되돌릴 상태 추가		
//--------------------------------------------------------------------------
inline void CUndo::AddUndo(CMemFile* file) 
{
	// undo 횟수보다 커지면 옛날 상태를 지움
	if (m_undolist.GetCount() > m_undoLevels)
	{
		CMemFile* pFile = (CMemFile *) m_undolist.RemoveTail();
		delete pFile;
	}
	m_undolist.AddHead(file); //새로운 상태를 head에 추가
}

//--------------------------------------------------------------------------
//　현재 상태를 저장
//--------------------------------------------------------------------------
inline void CUndo::Store(CMemFile* file) 
{
	file->SeekToBegin();
	CArchive ar(file, CArchive::store);
	Serialize(ar); 
	ar.Close();
}
//--------------------------------------------------------------------------
//　저장한 상태를 로드
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
//　현재 상태를 Undo 리스트에 저장
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
//　체크포인트 활성화/비활성화
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
//　redo 리스트에 상태 추가
//--------------------------------------------------------------------------
inline void CUndo::AddRedo(CMemFile *file) 
{
	m_redolist.AddHead(file);
}

//--------------------------------------------------------------------------
//　Undo 명령
//--------------------------------------------------------------------------
inline void CUndo::Undo() 
{
	if (CanUndo()) //Undo가 가능한 상태이면,
	{
		CMemFile *pFile = (CMemFile *) m_undolist.GetHead(); //undo 리스트의 가장 처음은 현재 상태
		m_undolist.RemoveHead(); //현재 상태를 지움
		AddRedo(pFile); //현재 상태를 Redo 리스트로 저장
		//이전 상태를 불러와 load
		pFile = (CMemFile *)m_undolist.GetHead();
		Load(pFile);
	}
}
//--------------------------------------------------------------------------
//　Redo 명령
//--------------------------------------------------------------------------
inline void CUndo::Redo() 
{
	if (CanRedo()) //Redo가 가능한 상태이면,
	{
		CMemFile *pFile = (CMemFile *) m_redolist.GetHead(); //redo 리스트의 가장 처음은 현재 상태
		m_redolist.RemoveHead(); //현재 상태를 지움
		AddUndo(pFile); //현재 상태를 Uedo 리스트로 저장
		Load(pFile); //이전 상태를 load
	}
}
