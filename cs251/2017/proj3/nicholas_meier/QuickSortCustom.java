public class QuickSortCustom {
	public static void sort(int[][] xy){
		QuickSortCustom(xy, 0, xy.length-1, 0);
	}
	public static void QuickSortCustom(int[][] a, int l, int r, int c) {
		int i;
		if(r > l){
			i = partition(a, l ,r, c);
			QuickSortCustom(a, l, i-1, c);
			QuickSortCustom(a, i+1, r, c);
		}
	}

	private static int partition(int[][] a, int l, int r, int c){
		int v = a[r][c];
	        int i = l;
        	int j = r;
	        int[] temp;
        	while (i < j){
	        	while (a[i][c] < v){
                		i = i + 1;
        	    	}
		        while ((i < j) && (a[j][c] >= v)){
                		j = j - 1;
        		}
		        temp = a[i];
			if (i < j){
	        	        a[i] = a[j];
        			a[j] = temp;
		        }else{
        		       	a[i] = a[r];
        			a[r] = temp;
			}	
	       	}
	        return i;
	}
	/*	
	public static void main(String[] args) {
        	int[][] A = {{4, 1}, {7, 2}, {5, 9}};
	        int l = 0;
        	int r = A.length-1;
	        QuickSortCustom(A, l, r, 0);
        	for (int i = 0; i < A.length; i++){
	            System.out.println(A[i][0] + " " + A[i][1]);
        	}
    	}*/
		
	
}
