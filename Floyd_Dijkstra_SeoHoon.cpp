#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>


using namespace std;

int** make_array(int n)	//n*n 2차원 배열 생성
{
	int** array;
	array=(int**)malloc(sizeof(int*)*n);
	for(int i=0; i<n; i++)
	{
		array[i]=(int*)malloc(sizeof(int)*n);
	}
	return array;
}

void floyd(int n, int *W[], int *D[], int *P[])	//floyd 알고리즘 수행하는 함수
{
	int i, j, k;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			P[i][j]=0;
		}
	}
	
	//D=W;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			D[i][j]=W[i][j];
		}
	}

	for(k=0; k<n; k++)
	{
		for(i=0; i<n; i++)
		{
			for(j=0; j<n; j++)
			{
				if(D[i][k]==-1||D[k][j]==-1)	//-1인 경우 무한대를 의미하므로 D배열 건드리지 않음
				{
					continue;
				}
				else if(D[i][k]+D[k][j]<D[i][j]||D[i][j]==-1)	//마찬가지로 원래 D배열값이 -1인 경우 무조건 바꿔야 함
				{
					P[i][j]=k+1;	//배열의 첨자가 아닌 노드 번호를 적기 위해 1을 더해 줌, P배열의 기본값 0과 구별하기 위해서라도..
					D[i][j]=D[i][k]+D[k][j];
				}
			}
		}
	}
}

void dijkstra(int n, int*W[], int s)
{
	int *touch, *length;	//배열과 변수 초기화
	int vnear, i, k;
	bool *uncheck;
	touch=(int*)malloc(sizeof(int)*n);
	length=(int*)malloc(sizeof(int)*n);
	uncheck=(bool*)malloc(sizeof(bool)*n);	//아직 체크되지 않은 노드 첨자들의 집합

	for(i=0; i<n; i++)	//touch에 시작노드 s를 length에 s와의 직접 거리를 입력
	{
		touch[i]=s;
		length[i]=W[s][i];
		uncheck[i]=true;	//아직 체크된 노드는 하나도 없음
	}
	for(k=0; k<n; k++)
	{
		int min=-1;
		for(i=0; i<n; i++)
		{
			if(uncheck[i]&&(min<0||((0<=length[i])&&(length[i]<min))))	//제일 가까운 노드 찾는 과정, -1값은 무한대를 의미하므로 if 조건문으로 걸러줌
			{
				min=length[i];
				vnear=i;
			}
		}
		uncheck[vnear]=false;	//최단거리로 알려진 노드는 체크해서 신입으로 받음
		for(i=0; i<n; i++)
		{
			if(uncheck[i]&&((length[i]<0&&(length[vnear]>=0&&W[vnear][i]>=0))||((length[vnear]>=0&&W[vnear][i]>=0)&&(length[i]>length[vnear]+W[vnear][i]))))	//마찬가지로 -1은 무한대를 의미하므로 if 조건문들로 조심히 걸러줌
			{
				length[i]=length[vnear]+W[vnear][i];
				touch[i]=vnear;	//신입 노드로 경유해서 가는게 더 짧으면 경로 갱신해줌
			}
		}
	}
	
	cout<<"-------다익스트라 알고리즘 : 경로와 거리 출력-------"<<endl;	//결과 출력
	FILE *Dijf;	//다익스트라 결과를 txt파일에 저장하기
	Dijf = fopen("Dij.txt", "w");
	
	cout<<endl;
	fprintf(Dijf, "\n");

	for(int des=0; des<n; des++)	//목적노드첨자 des로 가는 경로 출력
	{
		if(des==s)
		{
			continue;
		}
		else if(length[des]<0)
		{
			cout<<s+1<<"에서 "<<des+1<<"로 가는 경로는 없습니다"<<endl;	//플로이드 때와 마찬가지로 첨자번호로 연산한 뒤 출력할때는 노드 번호로 인지할 수 있게 1을 더해 줌
			fprintf(Dijf, "%d에서 %d로 가는 경로는 없습니다\n",s+1,des+1);
			continue;
		}
		else
		{
			int test=des;
			cout<<test+1;
			fprintf(Dijf,"%d",test+1);
			while(test!=s)
			{
				cout<<"<-"<<touch[test]+1;
				fprintf(Dijf,"<-%d",touch[test]+1);
				test=touch[test];
			}
		}
		cout<<"      경로의 길이는 : "<<length[des];	//경로 길이 출력
		fprintf(Dijf,"      경로의 길이는 : %d",length[des]);
		cout<<endl;
		fprintf(Dijf,"\n");
	}
	fclose(Dijf);
}


void print_console(int* W[], int n)	//콘솔 창에 배열 출력해주는 함수(옵션)
{
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if(W[i][j]==-1)
			{
				cout<<"inf"<<" ";
			}
			else
			{
				cout<<W[i][j]<<" ";
			}
		}
		cout<<endl;
	}
}

void main()
{
	//프로그램 사용자에게 알림 : 텍스트 파일에 다익스트라 시작 노드 적을 때 주의사항 : 첨자(0~n-1)가 아닌 실제 노드 번호(1~n)를 적어야 합니다.
	int **W, **P, **D;
	int n, s, si;
	
	FILE* file;
	try
	{
		fopen_s(&file, "input_array.txt","r");
	}
	catch(...)	//파일 여는데 오류 발생시 예외처리
	{
		cout<<"파일 열기 오류, 이름 확인 요망"<<endl;
	}	
	fscanf_s(file, "%d", &n);	//파일로부터 행렬 크기 n을 읽어오기
	W=make_array(n);	//n*n크기의 2차원 행렬들 생성해놓기
	D=make_array(n);
	P=make_array(n);
	
	for(int i=0; i<n; i++)	//txt에 저장된 값을 W배열에 저장시키기
	{
		for(int j=0; j<n; j++)
		{
			fscanf_s(file, "%d", &W[i][j]);
			
		}
	}
	fscanf_s(file, "%d", &si);	//시작노드 첨자를 s변수에 저장하기.
	s=si-1;

	cout<<"W행렬 출력"<<endl;	//읽어온 W행렬 출력하기
	print_console(W,n);
	cout<<"다익스트라 시작 노드 : "<<si<<endl;
	cout<<endl;

	floyd(n,W,D,P);	//플로이드 수행
	cout<<"D행렬 출력"<<endl;
	print_console(D,n);
	cout<<endl;

	cout<<"P행렬 출력"<<endl;
	print_console(P,n);
	cout<<endl;

	FILE *Df;	//D행렬을 txt파일에 저장하기
	Df = fopen("D.txt", "w");
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			fprintf(Df, "%d ", D[i][j]);
		}
		fprintf(Df, "\n");
	}
	fclose(Df);

	FILE *Pf;	//P행렬을 txt파일에 저장하기
	Pf = fopen("P.txt", "w");
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			fprintf(Pf, "%d ", P[i][j]);
		}
		fprintf(Pf, "\n");
	}
	fclose(Pf);
	dijkstra(n, W, s);
	
}