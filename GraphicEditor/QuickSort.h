/* [ CQuickSort 클래스 by 신희철]
** 폴리라인과 같은 많은 점들을 가진 그래피컬 개체나,
** 그룹처럼 여러 그래피컬 개체들의 점들을 가진 경우,
** 이 클래스를 이용하여 모든 점들을 정렬시켜,
** 영역의 시작점(left,top)과 종료점(right,bottom)를 구할 수 있도록 합니다.
** 오름차순으로 정렬되므로 배열의 0번째 인덱스가 시작점, 마지막 인덱스가 종료점이 됩니다. 
** 정렬할 자료가 담긴 리스트를 매개변수로 넘겨주면 해당 함수 내에서 정렬 시켜줍니다.
*/

#pragma once
class CQuickSort
{
public:
	void QuickSort(CArray<int, int&>* array, int first, int last);
};

//QuickSort 알고리즘 적용
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

			if(i<j) //swap시킴
			{
				int temp = array->GetAt(i);
				array->SetAt(i, array->GetAt(j));
				array->SetAt(j,temp);
			}
		}
		//swap시킴
		int temp = array->GetAt(first);
		array->SetAt(first, array->GetAt(j));
		array->SetAt(j, temp);
		QuickSort(array, first, j-1);
		QuickSort(array, j+1, last);
	}
}