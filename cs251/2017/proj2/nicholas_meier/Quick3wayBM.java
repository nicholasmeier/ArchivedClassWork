import java.util.*;
@SuppressWarnings("unchecked")
public class Quick3wayBM{
	private Quick3wayBM() {}

	public static void sort(Comparable[] a){
		sort(a, 0, a.length-1);
	}

	public static void sort(Comparable[] a, int lo, int hi){
			
	
		//insertion sort
		if ((hi-lo+1) <= 8){
			for (int n = lo; n <= hi; n++){
				for (int m = n; m > lo; m--){
					if(a[m].compareTo(a[m-1]) < 0){
						exch(a,m,m-1);
					}else{
						break;
					}
				}
			}
			return ;
		}else {
			//median of 3
			if ((hi-lo+1) <= 40){
				int middle = findMedian(a, lo, (lo+hi+1)/2, hi);
				exch(a, lo, middle);
			}else{
				//turkey ninther for partition
				int length = a.length;
				int middle1 = findMedian(a, lo, lo+length/8, lo+length/4);
				int middle2 = findMedian(a, lo+length/2-length/8, lo+length/2, lo+length/2+length/8);
				int middle3 = findMedian(a, hi-length/4, hi-length/8, hi);
				int tn = findMedian(a, middle1, middle2, middle3);
				exch(a,tn,lo);
			}
		}
		Comparable v = a[lo];
		int i = lo+1;
		int j = hi;
		int p = lo;
		int q = hi+1;
		while(i <= j){
			if(a[i].compareTo(v) < 0){i++;}
			if(a[j].compareTo(v) > 0){j--;}
			if (i < j) {exch(a, i, j);}
			if(a[i].compareTo(v) == 0){exch(a,i,++p);}
			if(a[j].compareTo(v) == 0){exch(a,j,--q);}
		}
		int pc = p;
		int qc = q;
		while(pc >= 1){exch(a,pc--,j--);}
		while(qc <= hi){exch(a,qc++,i++);}
		sort(a,lo,j);
		sort(a,i,hi);
		for(int x = 0; x < a.length; x++){
			StdOut.print(a[x]);
		}
	} 

	public static int findMedian(Comparable[] a, int b, int c, int d){
		if (a[b].compareTo(a[c]) > 0){
			exch(a, b, c);
		}
		if (a[b].compareTo(a[d]) > 0){
			exch(a, b, d);
		}
		if (a[c].compareTo(a[d]) > 0){
			exch(a, c, d);
		}
		return c;

	}
	
	//exchange things here
	public static void exch(Comparable[] a, int b, int c){
		Comparable d = a[b];
		a[b] = a[c];
		a[c] = d;
	}

	public static void show(Comparable[] a){
		for (int x = 0; x < a.length; x++){
			StdOut.println(a[x]);
		}
	}
	
	public static void main(String[] args){
		String[] a = StdIn.readAllStrings();
		Quick3wayBM.sort(a);
		Quick3wayBM.show(a);
	}	

	
}
