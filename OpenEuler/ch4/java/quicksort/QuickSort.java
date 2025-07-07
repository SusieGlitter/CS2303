import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

public class QuickSort
{
    public static void main(String[] args)
    {
        Scanner scanner=new Scanner(System.in);
        
        int n=scanner.nextInt();
        int[] a=new int[n];
        for(int i=0;i<n;i++)
            a[i]=scanner.nextInt();
        scanner.close();
        for(int num:a)
            System.out.print(num+" ");
        System.out.println();
        ForkJoinPool pool=new ForkJoinPool();
        pool.invoke(new SortTask(a,0,n-1));

        for(int num:a)
            System.out.print(num+" ");
        System.out.println();
    }
    static class SortTask extends RecursiveAction
    {
        private final int[] a;
        private final int s;
        private final int t;
        
        public SortTask(int[] a,int s,int t)
        {
            this.a=a;
            this.s=s;
            this.t=t;
        }
        private void swap(int[]a,int i,int j)
        {
            int temp=a[i];
            a[i]=a[j];
            a[j]=temp;
        }
        
        @Override
        protected void compute()
        {
            
            if(s==t)
                return;
            int i=s-1,j=t+1,x=a[(s+t)/2];
            while(i<j)
            {
                do i++;while(a[i]<x);
                do j--;while(a[j]>x);
                if(i<j)swap(a,i,j);
            }
            
            SortTask task1=new SortTask(a,s,j);
            SortTask task2=new SortTask(a,j+1,t);
            invokeAll(task1,task2);
/*
            System.out.print(s+" "+t+"\n");
            for(int num:a)
                System.out.print(num+" ");
            System.out.println();
*/
        }

    }
    
}
