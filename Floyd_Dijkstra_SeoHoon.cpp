#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>


using namespace std;

int** make_array(int n)	//n*n 2���� �迭 ����
{
	int** array;
	array=(int**)malloc(sizeof(int*)*n);
	for(int i=0; i<n; i++)
	{
		array[i]=(int*)malloc(sizeof(int)*n);
	}
	return array;
}

void floyd(int n, int *W[], int *D[], int *P[])	//floyd �˰��� �����ϴ� �Լ�
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
				if(D[i][k]==-1||D[k][j]==-1)	//-1�� ��� ���Ѵ븦 �ǹ��ϹǷ� D�迭 �ǵ帮�� ����
				{
					continue;
				}
				else if(D[i][k]+D[k][j]<D[i][j]||D[i][j]==-1)	//���������� ���� D�迭���� -1�� ��� ������ �ٲ�� ��
				{
					P[i][j]=k+1;	//�迭�� ÷�ڰ� �ƴ� ��� ��ȣ�� ���� ���� 1�� ���� ��, P�迭�� �⺻�� 0�� �����ϱ� ���ؼ���..
					D[i][j]=D[i][k]+D[k][j];
				}
			}
		}
	}
}

void dijkstra(int n, int*W[], int s)
{
	int *touch, *length;	//�迭�� ���� �ʱ�ȭ
	int vnear, i, k;
	bool *uncheck;
	touch=(int*)malloc(sizeof(int)*n);
	length=(int*)malloc(sizeof(int)*n);
	uncheck=(bool*)malloc(sizeof(bool)*n);	//���� üũ���� ���� ��� ÷�ڵ��� ����

	for(i=0; i<n; i++)	//touch�� ���۳�� s�� length�� s���� ���� �Ÿ��� �Է�
	{
		touch[i]=s;
		length[i]=W[s][i];
		uncheck[i]=true;	//���� üũ�� ���� �ϳ��� ����
	}
	for(k=0; k<n; k++)
	{
		int min=-1;
		for(i=0; i<n; i++)
		{
			if(uncheck[i]&&(min<0||((0<=length[i])&&(length[i]<min))))	//���� ����� ��� ã�� ����, -1���� ���Ѵ븦 �ǹ��ϹǷ� if ���ǹ����� �ɷ���
			{
				min=length[i];
				vnear=i;
			}
		}
		uncheck[vnear]=false;	//�ִܰŸ��� �˷��� ���� üũ�ؼ� �������� ����
		for(i=0; i<n; i++)
		{
			if(uncheck[i]&&((length[i]<0&&(length[vnear]>=0&&W[vnear][i]>=0))||((length[vnear]>=0&&W[vnear][i]>=0)&&(length[i]>length[vnear]+W[vnear][i]))))	//���������� -1�� ���Ѵ븦 �ǹ��ϹǷ� if ���ǹ���� ������ �ɷ���
			{
				length[i]=length[vnear]+W[vnear][i];
				touch[i]=vnear;	//���� ���� �����ؼ� ���°� �� ª���� ��� ��������
			}
		}
	}
	
	cout<<"-------���ͽ�Ʈ�� �˰��� : ��ο� �Ÿ� ���-------"<<endl;	//��� ���
	FILE *Dijf;	//���ͽ�Ʈ�� ����� txt���Ͽ� �����ϱ�
	Dijf = fopen("Dij.txt", "w");
	
	cout<<endl;
	fprintf(Dijf, "\n");

	for(int des=0; des<n; des++)	//�������÷�� des�� ���� ��� ���
	{
		if(des==s)
		{
			continue;
		}
		else if(length[des]<0)
		{
			cout<<s+1<<"���� "<<des+1<<"�� ���� ��δ� �����ϴ�"<<endl;	//�÷��̵� ���� ���������� ÷�ڹ�ȣ�� ������ �� ����Ҷ��� ��� ��ȣ�� ������ �� �ְ� 1�� ���� ��
			fprintf(Dijf, "%d���� %d�� ���� ��δ� �����ϴ�\n",s+1,des+1);
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
		cout<<"      ����� ���̴� : "<<length[des];	//��� ���� ���
		fprintf(Dijf,"      ����� ���̴� : %d",length[des]);
		cout<<endl;
		fprintf(Dijf,"\n");
	}
	fclose(Dijf);
}


void print_console(int* W[], int n)	//�ܼ� â�� �迭 ������ִ� �Լ�(�ɼ�)
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
	//���α׷� ����ڿ��� �˸� : �ؽ�Ʈ ���Ͽ� ���ͽ�Ʈ�� ���� ��� ���� �� ���ǻ��� : ÷��(0~n-1)�� �ƴ� ���� ��� ��ȣ(1~n)�� ����� �մϴ�.
	int **W, **P, **D;
	int n, s, si;
	
	FILE* file;
	try
	{
		fopen_s(&file, "input_array.txt","r");
	}
	catch(...)	//���� ���µ� ���� �߻��� ����ó��
	{
		cout<<"���� ���� ����, �̸� Ȯ�� ���"<<endl;
	}	
	fscanf_s(file, "%d", &n);	//���Ϸκ��� ��� ũ�� n�� �о����
	W=make_array(n);	//n*nũ���� 2���� ��ĵ� �����س���
	D=make_array(n);
	P=make_array(n);
	
	for(int i=0; i<n; i++)	//txt�� ����� ���� W�迭�� �����Ű��
	{
		for(int j=0; j<n; j++)
		{
			fscanf_s(file, "%d", &W[i][j]);
			
		}
	}
	fscanf_s(file, "%d", &si);	//���۳�� ÷�ڸ� s������ �����ϱ�.
	s=si-1;

	cout<<"W��� ���"<<endl;	//�о�� W��� ����ϱ�
	print_console(W,n);
	cout<<"���ͽ�Ʈ�� ���� ��� : "<<si<<endl;
	cout<<endl;

	floyd(n,W,D,P);	//�÷��̵� ����
	cout<<"D��� ���"<<endl;
	print_console(D,n);
	cout<<endl;

	cout<<"P��� ���"<<endl;
	print_console(P,n);
	cout<<endl;

	FILE *Df;	//D����� txt���Ͽ� �����ϱ�
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

	FILE *Pf;	//P����� txt���Ͽ� �����ϱ�
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