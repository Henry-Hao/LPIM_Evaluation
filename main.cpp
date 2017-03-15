#include "limit.h"
#include "graph.h"

#include "random.h"
#include "pmia.h"
#include "general_cascade.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

FILE* timetmpfile;
double timer;
LARGE_INTEGER start, End, freq;


struct myclass {
	int id;
	double rangenumber;
	double nodenumber;
};

vector<myclass> sortingresult;

void toSimulate(char *file, int (*GetNode)(int i), EvaluationIndex (*Run)(int num_iter, int size, int set[]))
{
	FILE *out;
	fopen_s(&out, file, "w");
	int set[50];
	int cnt=0;
	
	int n = Graph::GetN();
	/*for (int t = 0; t < n; t++) {
		set[0] = t;
		EvaluationIndex tt = Run(NUM_ITER, 1, set);
		fprintf(out, "node:%02d \t nodeNumbers:%10Lg \t rangeNumbers:%10Lg\n", t, tt.nodeNumbers,tt.rangeNumbers);
		printf("node:%02d \t nodeNumbers:%10Lg \t rangeNumbers:%10Lg\n", t, tt.nodeNumbers, tt.rangeNumbers);
	}*/


	for (int t = 0;t<50;t++)
	{
		for (int i = 0; i < t; i++)
			set[i] = sortingresult[i].id;
			/*set[i] = GetNode(i);*/
		EvaluationIndex tt = Run(NUM_ITER, t, set);
		fprintf(out, "nodes:%02d \t nodeNumbers:%10Lg \t rangeNumbers:%10Lg\n", t, tt.nodeNumbers, tt.rangeNumbers);
		printf("nodes:%02d \t nodeNumbers:%10Lg \t rangeNumbers:%10Lg\n", t, tt.nodeNumbers, tt.rangeNumbers);
	}
	fclose(out);
	getchar();
}

bool mycompare(const myclass &m1, const myclass &m2) {
	return m1.rangenumber > m2.rangenumber;
}

vector<myclass> mysort(string filename) {
	string line;
	vector<myclass> result;
	ifstream graphfile(filename.c_str());
	int v1, v2;
	if (graphfile.is_open()) {
		int i = 0;
		while (getline(graphfile, line)) {
			i++;
			printf("%d\n", i);
			int nodeindex = atoi(line.c_str())-1;
			int set[1];
			set[0] = nodeindex;
			EvaluationIndex e = GeneralCascade::Run(NUM_ITER, 1, set);
			myclass tmp;
			tmp.id = nodeindex;
			tmp.rangenumber = e.rangeNumbers;
			tmp.nodenumber = e.nodeNumbers;
			result.push_back(tmp);
		}
		graphfile.close();
		//qsort(sortingresult, i, sizeof(double), mycompare);
		sort(result.begin(), result.end(), mycompare);
		return result;
	}
	else
		cout << "Unable to open file " << filename;
}


int main(int argc, char * argv[])
{
	
	srand(time_t(NULL));
	//srand(100);
	Graph::Build2WC();
	GeneralCascade::Build();
	char File1[]="all.txt";
	FILE *out1;
	fopen_s(&out1, File1, "w");

	char File2[]="first50.txt";
	sortingresult =  mysort(argv[1]);
	for (auto it = sortingresult.begin(); it != sortingresult.end(); it++)
		fprintf(out1, "nodes:%02d \t nodeNumbers:%10Lg \t rangeNumbers:%10Lg\n", (*it).id, (*it).nodenumber, (*it).rangenumber);
	fclose(out1);
	printf("\n");
	toSimulate(File2, SPT_new::GetNode, GeneralCascade::Run);
	getchar();
//}

	

}

//int main(int argc, char * argv[])
//{
//	srand(time_t(NULL));
//	//srand(100);
//	QueryPerformanceFrequency(&freq);
//	system("mkdir tmp");
//	system("cd tmp");
//
//	//control bound to test PMIA_GC
//	//if (!s.compare(argv[1])) {
//	Graph::Build2WC();
//	GeneralCascade::Build();
//	int bound1 = 10, bound2 = 2000;
//	if (argc >= 3) sscanf_s(argv[2], "%d", &bound1);
//	if (argc >= 4) sscanf_s(argv[3], "%d", &bound2);
//	char SPTfilename[] = "PMIA_control.txt";
//	FILE *out;
//	fopen_s(&out, SPTfilename, "w");
//	char timefilename[] = "time_PMIA_0000.txt";
//	char SPT_new_WC[] = "GC_PMIA_0000.txt";
//	for (int bound = bound1; bound<bound2; bound += bound) {
//		printf("%d ", bound);
//		double spread, treesize = 0;
//#ifdef COUNT
//		{
//			spread = SPT_new::Build(SET_SIZE, bound);
//			printf("%lg\n", spread);
//			continue;
//		}
//#endif
//		sprintf_s(timefilename, "time_PMIA_%04d.txt", bound);
//		sprintf_s(SPT_new_WC, "GC_PMIA_%04d.txt", bound);
//		{
//			QueryPerformanceCounter(&start);
//			treesize = SPT_new::Build(SET_SIZE, bound);
//			QueryPerformanceCounter(&End);
//			printf("\n");
//			timer = (double)(End.QuadPart - start.QuadPart) / freq.QuadPart;
//			fopen_s(&timetmpfile, timefilename, "w");
//			fprintf(timetmpfile, "%lg\n", timer);
//			fclose(timetmpfile);
//			spread = 0;
//			toSimulate(SPT_new_WC, SPT_new::GetNode, GeneralCascade::Run);
//		}
//		fprintf(out, "%lg %lg %d %lg\n", timer, spread, bound, treesize);
//	}
//	fclose(out);
//	getchar();
//	//}
//
//
//
//}

