import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

public class MergeSort
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
        int[] b=new int[n];
        ForkJoinPool pool=new ForkJoinPool();
        pool.invoke(new SortTask(a,b,0,n-1));

        for(int num:a)
            System.out.print(num+" ");
        System.out.println();
    }
    static class SortTask extends RecursiveAction
    {
        private final int[] a;
        private final int[] b;
        private final int s;
        private final int t;
        
        public SortTask(int[] a,int[] b,int s,int t)
        {
            this.a=a;
            this.b=b;
            this.s=s;
            this.t=t;
        }
        
        @Override
        protected void compute()
        {
            
            if(s==t)
                return;
            int mid=(s+t)/2;
            SortTask task1=new SortTask(a,b,s,mid);
            SortTask task2=new SortTask(a,b,mid+1,t);
            invokeAll(task1,task2);
            new MergeTask(a,b,s,t).invoke();
/*
            System.out.print(s+" "+t+"\n");
            for(int num:a)
                System.out.print(num+" ");
            System.out.println();
*/
        }


    }
    static class MergeTask extends RecursiveAction
    {
        private final int[] a;
        private final int[] b;
        private final int s;
        private final int t;
        
        public MergeTask(int[] a,int[] b,int s,int t)
        {
            this.a=a;
            this.b=b;
            this.s=s;
            this.t=t;
        }
        @Override
        protected void compute()
        {
            int mid=(s+t)/2;
            int i=s,j=mid+1,k=s;
            while(i<=mid||j<=t)
                if(j==t+1||i!=mid+1&&a[i]<=a[j])
                    b[k++]=a[i++];
                else
                    b[k++]=a[j++];
            for(k=s;k<=t;k++)
                a[k]=b[k];
        }
    }
    
}
