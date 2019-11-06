public class Ferry{
	public static void main(String args[]){
		if (args.length <= 0){
			System.out.println("Error: Improper Usage");
			System.exit(1);
		}
		boolean firstInt = true;
		int numCar = -1;
		int lane1 = Integer.parseInt(args[0]);
		int lane2 = lane1;
		int lane1LC = 0;
		int lane2LC = 0;
		int count = 0;
		while(StdIn.hasNextLine()){
			if (firstInt){
				numCar = Integer.parseInt(StdIn.readLine());
				firstInt = false;
			}else{
				int i = Integer.parseInt(StdIn.readLine());
				if (lane1LC + i > lane1){
					if (!((lane2LC + i) > lane2)){
						count++;
						lane2LC += i;
					}else{
						break;
					}
				}else{
					count++;	
					lane1LC += i;
				}
			}
		}
		StdOut.println(lane1 + " " + count);
		
	}
}
