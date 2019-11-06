import edu.princeton.cs.algs4.QuickX;
import java.util.Arrays;
import java.util.Comparator;
/*
 * Nicholas Meier
 * Project 2
 * CS251 LE2
 * October 16th 2017
 * 
 */
public class Filter{
	//Input == set s of N two-demensional integer records
	//there exists pair of coords (x[i], y[i]), for which 0 <= i <= N-1
	//No two pairs will be equal values (x[i] != y[i])
	
	private int N;
	private String[] in;
	
		
	public Filter(String[] in, int n){
		//sort and store input as arrays
		//line 1 --> integer N (length of set)
		//followed by list of N records, not ordered format = x y\n
		this.in = in;
		this.N = n;
	}

	

	public int[][] filter(String[] in){
		//eliminate from set s, every record of (x[i], y[i]), where there is
		//(x[j], y[j]) > (x[i], y[i]) (both x and y are lesser)

		//parses the string array into int array	
		int[] x = new int[this.N];
		int[] y = new int[this.N];		
		int[][] xy = new int[this.N][2];
		for(int j = 0; j < this.N; j++){
			String a = in[j];
			String b[] = a.split(" ");
			x[j] = Integer.parseInt(b[0]);
			y[j] = Integer.parseInt(b[1]);
			xy[j][0] = x[j];
			xy[j][1] = y[j];
		}

		
		
		QuickSortCustom.sort(xy); //Customized Quicksort for integers

		for(int j = 0; j < this.N; j++){
			x[j] = xy[j][0];
			y[j] = xy[j][1];
		}	
		
		//Loop that removes the values from the list
		int k = 1;
		int i;
		for (i = this.N-1; i > 0; i--){	
			if (y[i] > y[i-k]){ 
				//use zero as a "null" value, hopefully no lists have zero in them
				xy[i-k][0] = 0;
				xy[i-k][1] = 0;
				y[i-k] = 0;
				x[i-k] = 0;
				if (i-k-1 < 0){ //prevent outOfBounds Exceptions
					break;
				}
				k++;
				i++;
			}else{
				k = 1;
			}
		}

		return xy;
	}

	public static void main(String args[]){
		boolean firstInt = true;
		int[] xi = null;
		int[] yi = null;
		int[][] xyi = null;
		String[] in = null;
		int n = 0;
		int count = 0;
		
		//read the input
		while(StdIn.hasNextLine()){
			if (firstInt){
				//first integer is stored
				n = Integer.parseInt(StdIn.readLine());
				in = new String[n];
				firstInt = false;
			}else{
				//remaining text becomes string arrays
				String line = StdIn.readLine();
				in[count] = line;
				count++;
			}
			
		}

		Filter f = new Filter(in, n);
		int[][] xy = f.filter(in); 
		

		//print out the filtered array			
		for (int k = 0; k < n; k++){
			if (xy[k][0] != 0)
				StdOut.println(xy[k][0] + " " + xy[k][1]);
		}
		
	}
}
