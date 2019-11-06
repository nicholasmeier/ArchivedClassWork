public class Query{
	//Store sorted N in arrays, sorted by x coords
	//represent array as binary tree T
	//For each node, v, of tree, make an array L[v]
	//where L is sorted by y coords, creates multiple copies of a record
	// create array L in bottum-up order
	public static void main(String args[]){
		int n = 0; //Length of in
		int m = 0; //Length of Q's columns
		boolean firstInt = true;
		boolean startQ = false;
		boolean inputM = true;
		int count0 = 0;
		int count1 = 0;
		String in[] = null;
		int[][] Q = null;
		//read input
		while(StdIn.hasNextLine()){
			if (firstInt){
				n = Integer.parseInt(StdIn.readLine());
				in = new String[n];
				//StdOut.println(n);
				firstInt = false;
			}else{
				if (!startQ){
					//StdOut.println(n + " " + count0); 
					String line = StdIn.readLine();
					in[count0] = line;
					count0++;
					if (count0 == n){
						startQ = true;
					}
								
				}else{
					if (inputM){
						m = Integer.parseInt(StdIn.readLine());
						Q = new int[m][2];
						inputM = false;
					}else{
						String line = StdIn.readLine();
						String[] la = line.split(" ");
						Q[count1][0] = Integer.parseInt(la[0]);
						Q[count1][1] = Integer.parseInt(la[1]);
						count1++;	
					}
				}	
			}
		}
		//parse string array in[] into int array xy[][]
		int[][] xy = new int[n][2];
		for(int j = 0; j < n; j++){
			String a = in[j];
			String[] b = a.split(" ");
			xy[j][0] = Integer.parseInt(b[0]);
			xy[j][1] = Integer.parseInt(b[1]);
		}
		//sorts xy in order of x
		QuickSortCustom.sort(xy);
		
		//the set of nested loops for printing out values
		for(int j = 0; j < m; j++){
			int c = 0;
			for (int i = 0; i < n; i++){
				if ((xy[i][0] > Q[j][0]) && (xy[i][1] > Q[j][1])){
					StdOut.println(xy[i][0] + " " + xy[i][1]);
					c++;
				}
				if ((c == 0) && (i == n-1)){
					StdOut.println("none");
				}
			} 
		}
	}
}
