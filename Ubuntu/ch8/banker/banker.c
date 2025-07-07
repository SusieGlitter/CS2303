#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#include <stdio.h>
#include <stdlib.h>

int num;
int count[NUMBER_OF_RESOURCES];

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


void show_info()
{
    printf("\n");
    printf("available array is\n");
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        printf("%d ",available[i]);
    printf("\n");
    printf("maximum matrix is\n");
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
    {
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
            printf("%d ",maximum[i][j]);
        printf("\n");
    }
    printf("allocation matrix is\n");
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
    {
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
            printf("%d ",allocation[i][j]);
        printf("\n");
    }
    printf("need matrix is\n");
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
    {
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
            printf("%d ",need[i][j]);
        printf("\n");
    }
}

int done[NUMBER_OF_CUSTOMERS];
int allDone()
{
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
        if(done[i]==0)
            return -1;
    return 0;
}

int check()
{
    if(allDone()==0)
        return 0;
    int flag=0;
    int num=-1;
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
    {
        if(done[i])
            continue;
        int flag=1;
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
            if(available[j]<need[i][j])
                flag=0;
        if(flag)
        {
            //show_info();
            //printf("done %d!!!!!!!!!\n",i);
            for(int j=0;j<NUMBER_OF_RESOURCES;j++)
                available[j]+=allocation[i][j];
                
            done[i]=1;
            int ret=check();
            
            for(int j=0;j<NUMBER_OF_RESOURCES;j++)
                available[j]-=allocation[i][j];
            
            return ret;
        }
    }
    return -1;
}
int request_resources(int customer_num,int request[])
{

    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
        done[i]=0;
        
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        if(available[i]<request[i]||need[customer_num][i]<request[i])
            return -1;
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
    {
        available[i]-=request[i];
        allocation[customer_num][i]+=request[i];
        need[customer_num][i]-=request[i];
    }
    int unsafe=check();
    if(unsafe)
    {
        for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        {
            available[i]+=request[i];
            allocation[customer_num][i]-=request[i];
            need[customer_num][i]+=request[i];
        }
        return -1;
    }
    else
        return 0;
}

int release_resources(int customer_num,int release[])
{
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        if(allocation[customer_num][i]<release[i])
            return -1;
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
    {
        available[i]+=release[i];
        allocation[customer_num][i]-=release[i];
        need[customer_num][i]+=release[i];
    }
    return 0;
}

int main(int argc,char *argv[])
{
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        available[i]=strtol(argv[i+1],NULL,10);
    for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
        {
            scanf("%d",&maximum[i][j]);
            getchar();
            need[i][j]=maximum[i][j];
            allocation[i][j]=0;
        }
    while(1)
    {
        printf(">");
        char s[256];
        scanf("%s",s);
        printf("%s",s);
        if(s[0]=='e')
            break;
        else if(s[0]=='*')
            show_info();
        else
        {
            scanf("%d",&num);
            printf(" %d",num);
            for(int i=0;i<NUMBER_OF_RESOURCES;i++)
                scanf("%d",&count[i]);
            for(int i=0;i<NUMBER_OF_RESOURCES;i++)
                printf(" %d",count[i]);
            printf("\n");
            if(s[1]=='Q')
            {
                int ret=request_resources(num,count);
                if(ret)
                    printf("The state is not safe!\n");
                else
                    printf("Successfully allocate the resources!\n");
            }
            else
            {
                int ret=release_resources(num,count);
                if(ret)
                    printf("%d customer doesn't have this much resources!\n",num);
                else
                    printf("Successfully release the resources!\n");
            }
                
        }
        printf("\n");
    }
    
    return 0;
}
