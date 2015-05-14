/* [ CQuickSort Ŭ���� by ����ö]
** �������ΰ� ���� ���� ������ ���� �׷����� ��ü��,
** �׷�ó�� ���� �׷����� ��ü���� ������ ���� ���,
** �� Ŭ������ �̿��Ͽ� ��� ������ ���Ľ���,
** ������ ������(left,top)�� ������(right,bottom)�� ���� �� �ֵ��� �մϴ�.
** ������������ ���ĵǹǷ� �迭�� 0��° �ε����� ������, ������ �ε����� �������� �˴ϴ�. 
** ������ �ڷᰡ ��� ����Ʈ�� �Ű������� �Ѱ��ָ� �ش� �Լ� ������ ���� �����ݴϴ�.
*/

#pragma once
class CQuickSort
{
public:
	void QuickSort(CArray<int, int&>* array, int first, int last);
};

//QuickSort �˰��� ����
inline void CQuickSort::QuickSort(CArray<int, int&>* array, int first, int last)
{
	if(first < last)
	{
		int pivot = array->GetAt(first);
		int i=first;
		int j=last;
		while(i<j)
		{
			while(array->GetAt(i) <= pivot && i<last)
				i++;
			while(array->GetAt(j) >= pivot && j>first)
				j--;

			if(i<j) //swap��Ŵ
			{
				int temp = array->GetAt(i);
				array->SetAt(i, array->GetAt(j));
				array->SetAt(j,temp);
			}
		}
		//swap��Ŵ
		int temp = array->GetAt(first);
		array->SetAt(first, array->GetAt(j));
		array->SetAt(j, temp);
		QuickSort(array, first, j-1);
		QuickSort(array, j+1, last);
	}
}