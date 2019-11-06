import edu.princeton.cs.algs4.WeightedQuickUnionUF;
public class PercolationVisualizer{
	public static void main(String[] args) {
		int[] input = StdIn.readAllInts();
		double sg = input[0];
		StdDraw.setCanvasSize(600,600);
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setScale(0, sg);
		StdDraw.filledSquare((sg)/2, (sg)/2, (sg)/2);
		PercolationVisualizer pv = new PercolationVisualizer(input[0]);
		StdDraw.setPenColor(StdDraw.WHITE);
		for (int b = 1; b < (input.length - 1); b+=2){
			StdDraw.setPenColor(StdDraw.WHITE);
			pv.open(input[b], input[b+1]);
			StdDraw.filledSquare(input[b]+.5, sg-1-input[b+1]+.5, .45);
			if (pv.isFull(input[b], input[b+1])){
				StdDraw.setPenColor(StdDraw.BLUE);
				StdDraw.filledSquare(input[b]+.5, sg-1-input[b+1]+.5,.45);
			}		
		}
		for (int c = 1; c < (input.length-1); c+=2){
			StdDraw.setPenColor(StdDraw.BLUE);
			if (pv.isFull(input[c], input[c+1])){
				StdDraw.filledSquare(input[c]+.5, sg-1-input[c+1]+.5, .45);
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////
	private int sizeGrid;
	private int sizeRow;
	private int sizeCol;
	private boolean[] grid;
	private int top;
	private int butt;	
	
	private WeightedQuickUnionUF uf; 
	
	//set up n*n grid
	public PercolationVisualizer(int n){
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
		
		/*	
		if (y == (sizeRow - 1)){
			uf.union(i, butt);	
		}*/
		for (int z = 0; z < 4; z++){
			int ia = getAdjCell(x, y, z);
			if ( (ia != -1) && (ia < sizeGrid) ){
				if (grid[ia]){
					uf.union(i, ia);
				}	
			}
		}
		
		if ( (y == (sizeRow - 1)) && (isFull(x, y)) ){
			uf.union(i, butt);
		}
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
	
	public void buttUnion(){
		for (int x = 0; x < sizeRow; x++){
			int i = indexSwitch (x, 0);
			if (isFull(x, 0)) { uf.union(i, butt); }
		}
	}

}
