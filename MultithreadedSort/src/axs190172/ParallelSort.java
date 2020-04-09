package axs190172;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

class PThread {
	  int TID,size;
}

public class ParallelSort implements Runnable {
	
	private PThread psThread; //PThread instance for each thread instance created
	
	static ArrayList<Integer> list = new ArrayList<Integer>();
	static Semaphore semaOne;
	static Semaphore phb[]= new Semaphore[2];
	static PThread threadObj = new PThread();
	static int isSorted=2;
	static int tCount=0;
	static int shift = 0;
	static String mode;
	
	public ParallelSort(PThread threadObj) {
		this.psThread = threadObj;
	}
	
	private int log2(int x) {
		return (int) (Math.log(x) / Math.log(2));
	}
	
	private void swap(ArrayList<Integer> list, int d1, int d2) {
		int t = list.get(d1);
    	list.set(d1, list.get(d2));
    	list.set(d2, t);
	}
	
	private static void printList(ArrayList<Integer> list, int N){
		   int ct=1;
		   System.out.println();
		   for(int i=0;i<N;i++) {
			  if(N < 9) {
				  System.out.print(" " +list.get(i) + "   ");
				 if(i == N-1) {
					 System.out.println(" ");
				 }
			  } else {
				  if(ct == 8) {
						ct = 1;
						System.out.print(list.get(i)+"\n");
				  } else {
					  System.out.print(" "+list.get(i) + "   ");
						ct++;
				  }
			  }
		   }
		   System.out.println("\n");
		}
	
	/**
	 * @param args
	 * @throws InterruptedException 
	 * @throws IOException 
	 */
	public static void main(String args[]) throws InterruptedException, IOException {
		String inputFilePath= null;
		try {
			mode = (String) args[1];
			System.out.println("given mode= " + mode);
			inputFilePath = (String) args[0];
			System.out.println("given file= " + inputFilePath);
		} catch(Exception e) {
			System.err.println("ERROR:Enter valid file name and mode( -r, -o)\nExample: java axs190172.ParallelSort inputfile.txt -r");
			return;
		}
		if(mode == null) {
			System.err.println();
			mode = "-o";
		}
//		String basePath = new File("").getCanonicalPath();
//	    System.out.println("bP= " + basePath);
	    
	    if(inputFilePath == null) {
	    	System.err.println("Enter input filename");
	    	inputFilePath = "input.1";
	    }
	    String path = new File("axs190172/").getAbsolutePath();
//	    System.out.println("full path= "+path);
		Scanner sc = null;
		File file = new File(path+"/"+inputFilePath);
		try {
			sc = new Scanner(file);
		} catch(Exception e) {
			System.err.println("ERROR: Invalid file path... Paste input file "+ inputFilePath +" inside axs190172/ folder...");
			return;
		}
		int N = sc.nextInt();
		while(N != 0) {
			for(int i=0; i< N; i++) {
				int data = sc.nextInt();
				list.add(data);
			}
			printList(list, N);
			File semafile = new File(path+"/sema.init");
			Scanner semasc = new Scanner(semafile);
			int sm = semasc.nextInt();
			phb[0] = new Semaphore(sm);
			phb[1] = new Semaphore(sm);
			sm = semasc.nextInt();
			semaOne = new Semaphore(sm);
			semasc.close();
			
			Thread t[] = new Thread[N/2];
			  for(int i=0;i < N/2; i++) {
				  threadObj = new PThread();
				  threadObj.size = N;
				  threadObj.TID = i;
				  t[i] = new Thread(new ParallelSort(threadObj));
				  t[i].start(); 
			   }
			  
			  for(int i=0; i< N/2; i++) {
				  t[i].join();
			   }
	
			  printList(list, N);
			  N = sc.nextInt();
			  for(int i=1; i<70; i++){
					System.out.print("-");
			  }
			  System.out.println();
			  list.clear();
		}
		sc.close();
	}

	@Override
	public void run() {
		try {
//			System.out.println("TID= " + this.psThread.TID);
			sort(this.psThread);
			
		} catch (InterruptedException e) {
			System.err.println(e);
		}
	}
	
	void sort(PThread psThread) throws InterruptedException {
		int gStart,gEnd,noOfGroups,gSize,gindex;
			int chk,mindex,d1,d2;
		    int length = log2(psThread.size);
		    for(int i=1; i<= length; i++) {
		      for(int j= 1;j <= length; j++){
		        noOfGroups = (int) Math.pow(2, (j-1));
		        gSize = psThread.size/noOfGroups;
				mindex = (psThread.TID)%(gSize/2);
		        gindex = psThread.TID/(gSize/2);
		        gStart = gSize*gindex;
		        gEnd = ((gSize)*(gindex + 1)) - 1;
		        d1 = gStart + mindex;
		        d2 = gEnd - mindex;
		        if(list.get(d1) > list.get(d2)) {
		        	swap(list, d1, d2);
		        }
		        semaOne.acquire();
		        tCount++;
		        chk = shift;
		        if(tCount == (psThread.size/2)) {
					if("-o".equalsIgnoreCase(mode)) {
						System.out.println("Thread " + psThread.TID + " finished stage "+ i+ " phase " + j);
						  printList(list, psThread.size);
					}
					for(int k=0;k<(psThread.size)-1;k++) {
						if(list.get(k) > list.get(k+1)) {
						   isSorted=1; break;
						} else {
						  isSorted=2;
						}
					}
					for(int k=1; k <= ((psThread.size)/2); k++) {
						phb[chk].release();
					}
					shift = 1 - shift;
					tCount = 0;
				} else if("-o".equalsIgnoreCase(mode)) {
					System.out.println("Thread " + psThread.TID + " finished stage "+ i+ " phase " + j);
				}
		        semaOne.release();
		        phb[chk].acquire();
		     if(j == length && isSorted == 2){
				return;
			 }
		    }
		}
		return;
	}

}
