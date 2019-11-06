import edu.princeton.cs.algs4.WeightedQuickUnionUF;
/*
 * Nicholas Meier
 * Project 1
 * CS251 LE2 
 * Sept. 11th 2017
 * 
 */

public class Percolation{
	private int sizeGrid;
	private int sizeRow;
	private int sizeCol;
	private boolean[] grid;
	private int top;
	private int butt;	
	
	private WeightedQuickUnionUF uf; 
	
	//set up n*n grid
	public Percolation(int n){
		if (n <= 0){
			System.out.println("Invalid Number or Something");
			System.exit(1);
		}
		sizeGrid = n*n;
		sizeRow = n;
		sizeCol = n;
		grid = new boolean[sizeGrid];
		top = sizeGrid;
		butt = sizeGrid + 1;
		uf = new WeightedQuickUnionUF(sizeGrid + 2);
	}
	
	public int indexSwitch(int x, int y){
		int index1D = sizeRow * y + x;
		//System.out.println("(i, x, y): " + index1D + " " + x + " " + y + " ");
		return index1D;
	}
	
	public int getAdjCell(int x, int y, int z){
		int ia = -1;
		switch(z){
			case 0:	
				if (y > 0){
					ia = indexSwitch(x, (y-1));
				}else {ia = -1;}
				break;
			case 1:
				if (y < (sizeRow-1)){
					ia = indexSwitch(x, (y+1));
				}else{ ia = -1; }
				break;
			case 2:
				if (x > 0){
					ia = indexSwitch((x-1), y);
				}else{ ia = -1; }
				break;
			case 3:
				if (x < (sizeCol-1)){
					ia = indexSwitch((x+1), y);
				}else{ ia = -1; }
				break;
		}
		//System.out.println("AdjCellIndex: " + ia);
		return ia;
	}
	
	//set a cell on grid to be open. If the y coord == 0, union to top.
	//TODO
	//Find adjacent cells
	//union opened adjacent cells together
	public void open(int x, int y){
		if ( (x < 0) || (x > sizeRow) || (y < 0) || (y > sizeCol) ){
			System.out.println("Bad Numbers: " + x + " " + y);
			return ;	
		}
		int i = indexSwitch(x, y);
		grid[i] = true; 
		if (y == 0){
			uf.union(i, top);	
		}
		
			
		if (y == (sizeRow - 1)){
			uf.union(i, butt);	
		}
		for (int z = 0; z < 4; z++){
			int ia = getAdjCell(x, y, z);
			if ( (ia != -1) && (ia < sizeGrid) ){
				if (grid[ia]){
					uf.union(i, ia);
				}	
			}
		}
		/*
		if ( (y == (sizeRow - 1)) && (isFull(x, y)) ){
			uf.union(i, butt);
		}*/
		//System.out.println(" ");
	}
	
	//check if cell on grid is open
	public boolean isOpen(int x, int y){
		if ( (x < 0) || (x > sizeRow) || (y < 0) || (y > sizeCol) ){
			System.out.println("Bad Numbers: " + x + " " + y);
			return false;
		}
		int i = indexSwitch(x, y);
		return grid[i]; 
	}
	
	//check if cell is connected to surface
	public boolean isFull(int x, int y){
		int i = indexSwitch(x, y);
		return uf.connected(i, top);	
	}
	
	//check if entire grid percolates (flows up to down)
	public boolean percolates(){
		return uf.connected(top, butt);
	}
	/*
	public void buttUnion(){
		/*int i = indexSwitch(x, y);
		uf.union(i, butt);
		for (int x = 0; x < sizeRow; x++){
			int i = indexSwitch (x, 0);
			if (isFull(x, 0)) { uf.union(i, butt); }
		}
	}*/

	public int numOpened(){
		int c = 0;
		for (int x = 0; x < sizeRow; x++){
			for(int y = 0; y < sizeCol; y++){
				if (isOpen(x, y))
					c++;
		 	}
		}
		return c;
	}

	//run the thing
	public static void main(String[] args){
		int[] input = StdIn.readAllInts();
		Percolation p = new Percolation(input[0]);
		for (int a = 1; a < (input.length - 1); a+=2){
			p.open(input[a], input[a+1]);
		}
		//p.buttUnion();
		/*
		for (int a = 1; a < (input.length-1); a+=2){
			if ( p.isFull(input[a], input[a+1]) && (input[a+1] == (input[0]-1))){
				p.buttUnion(input[a], input[a+1]);
			}
		}*/
		if (p.percolates()){
			System.out.println("Percolates.");
		}else{
			System.out.println("Does not percolate.");		
		}
		
	}
	
}
