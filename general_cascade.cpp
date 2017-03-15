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
						//被激活节点的所在路径的影响层数为该条边的源节点的影响层数+1
						eachNodeRangeTmp[e.v] = eachNodeRangeTmp[e.u] + 1;
						//如果该路径的根节点就是该条边的源节点
						if (root[e.u] == -1)
							//那么设置被激活节点的根节点为该条边的源节点
							root[e.v] = e.u;
						//如果该路径的根节点不是该条边的源节点
						else{
							//且被激活之后的新路径的影响层数大于该路径根节点的影响层数
							if (savedRangeNum[root[e.u]] < eachNodeRangeTmp[e.v]) {
								//则更新根节点的影响层数
								savedRangeNum[root[e.u]] = eachNodeRangeTmp[e.v];
							}
							//设置被激活节点的根节点为该条边的源节点的根节点
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
