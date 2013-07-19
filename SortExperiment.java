/**
 *
 * @author Jessica 5/20/2013
 */
public class SortExperiment {
     
     public static void main(String[] args)
     {
         if(args.length != 2) exit();
         if(Integer.parseInt(args[1]) < 1 || Integer.parseInt(args[1]) > 6) 
             exit();
         SortExperiment s = new SortExperiment(args[0].charAt(0), Integer.parseInt(args[1]) );
     }
     
    
    SortExperiment(char choice, int power) 
    {

        long[] endTime = new long[power];
        long[] moves = new long[power];
        long[] comparisons = new long[power];
        SortAlgorithm s = new Selection(); //initialized to this so that the compiler wont complain that it is possibly uninitialized (it wouldnt be)
        SortTimer timer = new SortTimer(); 


        switch(choice)
        {
            case 's' : s = new Selection();
                       if(power == 6)
                         System.out.println("Warning: as an O(n^2) sorting method, \nselection sort on 10^6 can take \na large amount of time to complete.");
                       System.out.println("Selection Sort");
                       break;
            case 'h' : s = new HeapSort();
                       System.out.println("Heap Sort ");
                       break;
            case 'm' : s = new MergeSort();
                       System.out.println("Merge Sort ");
                       break;
            case 'q' : s = new QuickSort();
                       System.out.println("QuickSort  ");
                       break;
            default:   System.out.println("Not a valid choice. ");
                       exit();
        }//end switch
        
        
     
        for(int k = 0; k < power; k++) //generate the data from 0 - k 
        {   
            int bound = (int)Math.pow(10, k+1);

            for(int i = 0; i < 5; i++)  //do five runs and take the average time / moves / comparisons.
            {
                timer.reset(); 

                double[] array = new double[bound];

                for(int ii = 0; ii < bound; ii++)
                {
                    array[ii] = Math.random()*100;
                                    
                }
                
                s.sort(array, timer);

                verify(array); //notice that this is called on every array. It only prints a message if it finds an unsorted element. 

                endTime[k] += timer.getElapsedTime(); //sorttimer's elapsed time will to the division by 1000
                moves[k] += timer.getMoves();
                comparisons[k] += timer.getComparisons();


             }// end five loop

            //get the average of the data
            endTime[k] /= 5;
            moves[k] /= 5;
            comparisons[k] /= 5;

        } // end k loop

        
        System.out.printf("%8s%12s%12s%10s\n", "n", "microsecs", "comparisons", "moves");
        for(int k = 0; k < power; k++)
         {  System.out.printf("%8d%12d%12d%10d\n", (int)Math.pow(10, k+1), endTime[k], comparisons[k], moves[k]); }
    }
    
    public static void verify(double[] array) //this function makes sure that the input was totally sorted. 
    {
        boolean flag = true;
        
        for(int i =0; i < array.length - 1; i++)
        {
            if(!(array[i] <= array[i+1]))
            {
               flag = false;
               break;
            }
        }
        if (!flag)
        System.out.println("Input unsorted.");
    }
    
    
     public static void exit()
     {
         System.out.println("Usage: Java SortingExperiment options:[shmq] + power[1-6] \nSelection sort, Heap sort, Merge sort and Quicksort");
         System.exit(0);
     }
}

//////////////////// SortAlgorithim

public interface SortAlgorithm {
    public void sort(double[] a, SortTimer t);
}

//////////////////// SortTimer

public class SortTimer {
    private long moves, comparisons, time;
 
    
    public void reset()
    {
        moves = 0;
        comparisons = 0;
        time = System.nanoTime();
    }
    
    public void addComparisons(int n)
    {
      comparisons += n;
    }
      
    public void addComparison()
    {
        addComparisons(1);
    }
    public void addMove()
    {
        addMoves(1);
    }
    public void addMoves(int n)
    {
        moves += n;
    }
    public long getComparisons()
    {
        return comparisons;
    }
    public long getMoves()
    {
        return moves;
    }
    public long getElapsedTime()
    {
        return (System.nanoTime()-time)/1000;
    }
    
    
}

/////////////////////////////////////////////// Quick Sort

/*
 * quick: 
 * 1. select pivot
 * 2. swap values so that roughly every element to the left is <=, and every element to the right is > the pivot
 * 3. increment Lpos, decrement Rpos until they point to the same element (pivot)
 * 4. merge down until arrays are one element or less.
                     
 */
public class QuickSort implements SortAlgorithm{
    
    SortTimer timer;
    
     public void sort(double[] a, SortTimer t)
     {
         timer = t;
         quickSort(a,0,a.length-1);
     }

    
     public void quickSort(double[] a, int left, int right)
     {
         if(a.length < 2)
             return;
         
         int Lpos = left, Rpos = right;
         
         int randIndex = Lpos + (int)(Math.random() * ((Rpos - Lpos) + 1));
         double pivot = a[randIndex];
         timer.addMove();
          
         //partitioning:
         while(Lpos <= Rpos)
         {
              while(a[Lpos] < pivot)
              {
                  Lpos++;
              }
              while(a[Rpos] > pivot)
              {
                  Rpos--;
              }
              if(Lpos <= Rpos)
              {
                swap(a,Lpos,Rpos);
                Lpos++;
                Rpos--;
              }
              timer.addComparisons(2);
          }
         
         if(left < Rpos)
             quickSort(a,left,Rpos);
         if(Lpos < right)
             quickSort(a,Lpos,right);
                                      
     }
     
     
      public void swap(double[] array, int a, int b) //swaps element at a with element at b
    {
        double temp = array[a];
        array[a] = array[b];
        array[b] = temp;
        timer.addMoves(3);
    }
}

/////////////////////////////////////////////// Merge Sort


class MergeSort implements SortAlgorithm
{
     SortTimer timer; 
     
    public void sort(double[] a, SortTimer t)
    {
        timer = t;
        mergeSort(a, 0, a.length-1);               
    }
    
   public void mergeSort( double [] a, int left, int right )
   {
      int mid;
      if(right > left )
      {                                
         mid = (int)((left+right)/2);
         mergeSort(a, left, mid);
         mergeSort(a, mid+1, right);
         merge(a, left, mid+1, right);
      }
   }
   
   public void merge(double[] a, int left, int mid, int right)
   {
       double[] temp = new double[right - left+1];
       
       int posL = left;
       int posR = mid;
       int posT = 0;
       
       while(posL < mid && posR <= right)                           
       {
           if(a[posL] < a[posR])
           {
               temp[posT++] = a[posL++]; 
           }
           else if(a[posL] == a[posR])
           { 
               temp[posT++] = a[posL++]; 
               posR++; 
           }
           else
           { 
               temp[posT++] = a[posR++]; 
           }
           
           timer.addComparisons(2);
           timer.addMove();
       }
       
       while(posL < mid)
        { 
            temp[posT++] = a[posL++]; 
            timer.addMove(); 
        } 
       while(posR <= right)
       { 
           temp[posT++] = a[posR++]; 
           timer.addMove();        }
       
       System.arraycopy(temp, 0, a, left, temp.length);
       timer.addMoves(temp.length);
   }
   
}

/////////////////////////////////////////// Heap Sort


import java.util.Arrays;

/**
 *
 * @author Jessica Milne
 */
public class HeapSort implements SortAlgorithm {

   private static int size;
   private static double[] array;

    public void sort(double[] a, SortTimer t)
    {
        size = a.length; 
        array = new double[size];
        array = a.clone();
        t.addMoves(size);                      
        
        //heapify (rough sort)    
        int f = (size - 1)/2;
              
        for(int i = f; i >= 0; i--)
             siftDown(i, t);
       
        for(int i =0; i < array.length; i++)
        {
            a[i] = poll(t);
            t.addMove();
        }
            
    
    }
    
      public static void siftDown(int i, SortTimer t) 
    {
        int b = i;
                      
        while( b < size/2 && (array[b] > array[smallerChild(b)]) )
        {
            int smallCh = smallerChild(b);
            t.addComparisons(2);
            
            swap(b, smallCh);
            t.addMoves(3);
            
            b = smallCh;            
        }     

    }
    
    public static void swap(int a, int b) //swaps element at a with element at b
    {
        double temp = array[a];
        array[a] = array[b];
        array[b] = temp;
        
    }
    
    public static double poll(SortTimer t)
    {
        double e = array[0];
        
        array[0] = array[size-1];
        array[size-1] = Double.NaN;
        size--; 
        
        t.addMoves(3);
        
        siftDown(0, t);
        return e;
        
    }
    
    
    public static int getSize()
    {
        return size;
    }
    
  
    public static void toString(double[] ar)
    {
      System.out.println(Arrays.toString(ar));
        
    }
        

    public static int smallerChild( int parent) //(smaller child of parent )'s index
    {
       if(size > 2*parent+2 && array[2*parent+2] < array[2*parent+1])
           return 2*parent+2;
       else 
           return 2*parent+1;
               
    }
      

}

    
///////////////////////////////////////////////// Selection Sort


public class Selection implements SortAlgorithm {
    
    @Override
    public void sort(double[] a, SortTimer timer) {
        
        int max; 
        
        for(int i = a.length-1; i >= 1; i--)
        {
            max = i;
            for(int j = 0; j < i; j++)
            {
                if(a[max] < a[j])
                {
                    max = j;
                }
                timer.addComparison();
            }
            swap(a, i, max);
            timer.addMoves(3);
        }
    }
     public void swap(double[] array, int a, int b) //swaps element at a with element at b
    {
        double temp = array[a];
        array[a] = array[b];
        array[b] = temp;
        
    }
}

    

