#include "graph.h"
#include "general_cascade.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <time.h>

int GeneralCascade::n = 0;
int GeneralCascade::m = 0;
int	GeneralCascade::targetSize = 0;
int	GeneralCascade::resultSize = 0;
int GeneralCascade::resultRange = 0;
int	GeneralCascade::target[MAX_K]={0};
bool GeneralCascade::built = false;
EvaluationIndex r;

void GeneralCascade::Build()
{
	srand((unsigned)time(NULL));
	n = Graph::GetN();
	m = Graph::GetM();
	built = true;
}

void GeneralCascade::SetTarget(int size, int set[])
{
	if (!built) Build();
	targetSize = size;
	for (int i=0; i<size; i++)
		target[i] = set[i];
	resultSize = 0;
	resultRange = 0;
	

}

EvaluationIndex GeneralCascade::Run(int num_iter, int size, int set[])
{
	SetTarget(size, set);
	r.nodeNumbers = 0;
	r.rangeNumbers = 0;

	int		h, t;
	int		*list  = new int[n];
	bool	*active= new bool[n];
	int     *eachNodeRangeTmp = new int[n];
	int     *savedRangeNum = new int[n];
	int     *root = new int[n];

	for (int it=0; it<num_iter; it++)
	{
		//printf("%d\n",it);
		memset(active, 0, sizeof(bool)*n);
		for (int j = 0; j < n; j++) {
			eachNodeRangeTmp[j] = 1;
			root[j] = -1;
			savedRangeNum[j] = 0;
		}
		/*for (int j = 0; j < size; j++) {
			savedRangeNum[j] = 1;
		}*/
		//memset(savedRangeNum, 0, sizeof(int)*size);
		//memset(root, -1, sizeof(int)*n);

		for (int i=0; i<targetSize; i++) 
		{
			list[i] = target[i];
			active[target[i]] = true;
		}
		resultSize += targetSize;
		r.nodeNumbers += targetSize;

		h = 0;
		t = targetSize;

		
		
		while (h<t) 
		{
			int k = Graph::GetNeighbor(list[h]);
			//printf("%d \n",k);
			for (int i=0; i<k; i++)
			{
				
				Edge e = Graph::GetEdge(list[h], i);
				if (active[e.v]) continue;
					if ((((double)rand() / (double)RAND_MAX) < exp(-e.w1)))
					{
						//������ڵ������·����Ӱ�����Ϊ�����ߵ�Դ�ڵ��Ӱ�����+1
						eachNodeRangeTmp[e.v] = eachNodeRangeTmp[e.u] + 1;
						//�����·���ĸ��ڵ���Ǹ����ߵ�Դ�ڵ�
						if (root[e.u] == -1)
							//��ô���ñ�����ڵ�ĸ��ڵ�Ϊ�����ߵ�Դ�ڵ�
							root[e.v] = e.u;
						//�����·���ĸ��ڵ㲻�Ǹ����ߵ�Դ�ڵ�
						else{
							//�ұ�����֮�����·����Ӱ��������ڸ�·�����ڵ��Ӱ�����
							if (savedRangeNum[root[e.u]] < eachNodeRangeTmp[e.v]) {
								//����¸��ڵ��Ӱ�����
								savedRangeNum[root[e.u]] = eachNodeRangeTmp[e.v];
							}
							//���ñ�����ڵ�ĸ��ڵ�Ϊ�����ߵ�Դ�ڵ�ĸ��ڵ�
							root[e.v] = root[e.u];
						}
						list[t] = e.v;
						active[e.v] = true;
						t++;
						resultSize++;
						//break;
					}
			}
			h++;
		}
		for (int i = 0; i < n; i++) {
			resultRange += savedRangeNum[i];
		}
	}
	r.nodeNumbers = (double)resultSize / (double)num_iter;
	r.rangeNumbers = (double)resultRange / (double)num_iter;
	delete[] active;
	delete[] list;
	delete[] root;
	delete[] eachNodeRangeTmp;
	delete[] savedRangeNum;
	return r;
}
