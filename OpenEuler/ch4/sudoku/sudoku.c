#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct para
{
    int *sudoku;
    int start;
    int step1;
    int step2;
    int *res;
};

void *check(void *pa)
{



    struct para *p=(struct para *)pa;

    
    int used[9];
    for(int i=0;i<9;i++)
        used[i]=0;
    
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            int num=p->sudoku[p->start+i*p->step1+j*p->step2];
            if(num>9||num<1||used[num-1])
            {
                *(p->res)=0;
                pthread_exit(0);
            }
            else
            {
                used[num-1]++;
            }
        }
            
    
}


int sudoku[9][9];
int result[27];
pthread_t tid[27];
pthread_attr_t tattr[27];
struct para pa[27];

int main()
{
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            scanf("%d",&sudoku[i][j]);

    for(int i=0;i<27;i++)
        result[i]=1;
    //row
    for(int i=0;i<9;i++)
    {
	int idx=i;
        pthread_attr_init(tattr+idx);
        pa[idx].sudoku=sudoku;
        pa[idx].start=9*i;
        pa[idx].step1=3;
        pa[idx].step2=1;
        pa[idx].res=result+idx;
        if(pthread_create(tid+idx,tattr+idx,check,pa+idx))
            printf("error\n");
    }
    //column
    for(int i=0;i<9;i++)
    {
	int idx=i+9;
        pthread_attr_init(tattr+idx);
        pa[idx].sudoku=sudoku;
        pa[idx].start=i;
        pa[idx].step1=27;
        pa[idx].step2=9;
        pa[idx].res=result+idx;
        if(pthread_create(tid+idx,tattr+idx,check,pa+idx))
            printf("error\n");
    }
    //block
    for(int i=0;i<9;i++)
    {
	int idx=i+18;
        pthread_attr_init(tattr+idx);
        pa[idx].sudoku=sudoku;
        pa[idx].start=(i/3)*27+(i%3)*3;
        pa[idx].step1=9;
        pa[idx].step2=1;
        pa[idx].res=result+idx;
        if(pthread_create(tid+idx,tattr+idx,check,pa+idx))
            printf("error\n");
    }

    for(int i=0;i<27;i++)
        pthread_join(tid[i],NULL);
/*
    for(int i=0;i<9;i++)
        printf("row %d is %s\n",i+1,result[i]?"valid":"invalid");
    for(int i=0;i<9;i++)
        printf("column %d is %s\n",i+1,result[i+9]?"valid":"invalid");
    for(int i=0;i<9;i++)
        printf("block %d is %s\n",i+1,result[i+18]?"valid":"invalid");
*/

    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            int k=j/3+i/3*3;
            if(result[i]&&result[j+9]&&result[k+18])
                printf("\033[0;32m%d \033[0m",sudoku[i][j]);
            else if(result[i]||result[j+9]||result[k+18])
                printf("\033[0;33m%d \033[0m",sudoku[i][j]);
            else
                printf("\033[0;31m%d \033[0m",sudoku[i][j]);
                
        }
        printf("\n");
    }
    return 0;
}





