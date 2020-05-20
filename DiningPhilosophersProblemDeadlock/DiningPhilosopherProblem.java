
import java.util.Scanner;
import java.util.concurrent.Semaphore;

public class DiningPhilosopherProblem implements Runnable {
	static int N;
	static Semaphore fork[];
	int PhilID;
	static boolean enableDeadlock = false;
	private static final int defaultNoOfPhilosophers=5;
	public DiningPhilosopherProblem(int id) {
		PhilID = id;
	}
	public static void main(String[] args) throws InterruptedException {
		Scanner in = new Scanner(System.in);
		System.out.println("Do you want to enable deadlock? ( Y/N )");
		String resp = in.next();
		enableDeadlock = resp.equalsIgnoreCase("Y");
		System.out.println("Enter no of philosophers: ");
		int noOfPhil = in.nextInt();
		N= noOfPhil > 2? noOfPhil : defaultNoOfPhilosophers;
		DiningPhilosopherProblem phil[] = new DiningPhilosopherProblem[N];
		Thread t[] = new Thread[N];
		fork= new Semaphore[N];
		for(int i=0;i<N;i++) {
			fork[i]= new Semaphore(1);
		}
		for(int i=0;i<N;i++) {
			phil[i] = new DiningPhilosopherProblem(i);
			t[i] = new Thread(new DiningPhilosopherProblem(i));
			t[i].start();
		}
		for(int i=0; i< N; i++) {
			  t[i].join();
		}
		
	}
	
	@Override
	public void run() {
		try {
			philosopher();
		} catch (InterruptedException e) {
			System.err.println("Error= "+e);
		}
	}
	
	public void philosopher() throws InterruptedException {
		while(true) {
			think();
			if(!enableDeadlock) {
				if(this.PhilID == 0) {
					takeFork(this.PhilID,this.PhilID+1);
					takeFork(this.PhilID, this.PhilID);
				} else {
					takeFork(this.PhilID,this.PhilID);
					takeFork(this.PhilID, this.PhilID+1);
				}
			} else {
				takeFork(this.PhilID,this.PhilID);
				takeFork(this.PhilID, this.PhilID+1);
			}
			eat(); 
			putFork(this.PhilID,this.PhilID);
			putFork(this.PhilID, this.PhilID+1);
		}
		
	}
	void takeFork(int id ,int frk) throws InterruptedException {
		fork[(frk)%N].acquire();
		System.out.println("Philosopher " + id + " TOOK Fork"+frk);
	}
	void putFork(int id ,int frk) throws InterruptedException {
		fork[(frk)%N].release();
		System.out.println("Philosopher " + id + " PUT Fork"+frk);

	}
	void think() {
		System.out.println("Philosopher " + this.PhilID + " is Thinking");
	}
	void eat() {
		System.out.println("Philosopher " + this.PhilID + " is Eating");
	}

}
