import edu.princeton.cs.algs4.QuickX;
public class SumSort{
	public static void main(String args[]){
		boolean firstInt = true;
		Integer[] values = null;
		int n = 0;
		int count = 0;
		
		while(StdIn.hasNextLine()){
			if(firstInt){
				n = StdIn.readInt();
				values = new Integer[n];
				firstInt = false;
			}else{
				Integer i = StdIn.readInt();
				values[count] = i;
				count++;
				if (count == n){
					break;
				}
			}
		}
		QuickX.sort(values);
		
		int asdf = 0;
		for(int i = 0; i < n-3; i++){
			int a = values[i];
			if ( (i != 0) && (values[i] == values[i-1]) )
				continue;
			for(int j = i+1; j < n-2; j++){
				int b = values[j];
				if( (j != i+1) && (values[j] == values[j-1]) )
					continue;
				int k = j+1;
				int l = n-1;
				while(k < l){
					int c = values[k];
					int d = values[l];
					if ( (a + b - c - d) < 0){
						k++;
					}else{
						if ( (a + b - c - d) > 0){
							l--;
						}else{
							StdOut.println(i + " " + j + " " + k + " " + l);
							k++;
							l--;
							asdf++;
							while((k < l) && (values[l] == values[l+1]) ){
								l--;
							}
							while((k < l) && (values[l] == values[k-1]) ){
								k++;
							}

						}
					}
				}
			} 
		}
		/*
		//for (int i = 0; i < n; i++)
			//StdOut.println(values[i]);
		int oldc = 0;
		int oldd = 0;
		int asdf = 0;
		for(int i = n-1; i >= 3; i--){
			int a = values[i];
			for(int j = i-3; j >= 0; j--){
				int b = values[j];
				for(int k = i-1; k >= j+2; k--){
					int c = values[k];
					for(int l = k-1; l >= j+1; l--){
						int d = values[l];
						int cd = c+d;
						int ab = a+b;
						//StdOut.println(ab + " " + cd);
						if ( ((a+b) == (c+d)) && (i != j) && (i != k) && (i != k) && (j != k) && (j != l) && (k != l) && (oldc != d) && (oldd != c) ){
							//oldd = d;
							//oldc = c;
							asdf++;
							StdOut.println(i + " " + j + " " + k + " " + l);
						}
							
					}
				}
			}
		}	
		//StdOut.println(asdf);	
			
		int asdf = 0;
		for(int i = n-1; i >= 0; i--){
			int a = values[i];
			for(int j = i-1; j >= 0; j--){
				int b = values[j];
				int start = j+1;
				int end = n-2;
				//StdOut.println(c);
				while (start < end){
					int c = values[start];
					int d = values[end];
					int cd = c+d;
					int ab = a+b;
					//StdOut.println(ab + " " + cd);
					if (ab == cd){
						StdOut.println(a + " " + b + " " + c + " " + d);
						asdf++;
						if (c == values[start+1]){
							start = start+1;
						}else{
							end = end-1;
						}
					}else{
						if (cd > ab){
							end = end-1;
						}else{
							start = start+1;
						}
					}
				}	
			}
		}*/
		StdOut.println(asdf);
	}
}
