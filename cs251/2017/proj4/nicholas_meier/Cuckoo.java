import java.util.*;
@SuppressWarnings("unchecked")
public class Cuckoo<K, V>{
	//TODO: Read in commands and key/type values
	//The class isn't generic, will need to work on that
	//first just make sure it works with integers	


	private int count; //n from specs
	private long a1; //first prime # from file
	private long a2; //second prime # from file
	private int size; //r from the specs
	private double e; //epsilon from specs
	private int[][] table;
	private boolean verbose;

	private int Lmax;
	public int Lcount = 0;

	public Cuckoo(){
		this.count = 0;  
		this.size = 256; 
		this.e = 0.05;
		this.Lmax = (int)((Math.log(this.size+1))/(Math.log(e+1)) * 3);
		this.table = new int[2][this.size];
		this.verbose = true;
	}
	
	public void initializeTable(){
		for (int i = 0; i < this.size; i++){
			for (int j = 0; j < 2; j++){ //may want to remove second for loop later
				this.table[j][i] = -1;
				//-1 to show the space is not taken
			}
		}
	}
	
	public void seta1(long a1){
		this.a1 = a1;
	}

	public void seta2(long a2){
		this.a2 = a2;
	}

	public void setLC(){
		Lcount = 0;
	}

	//TODO DEAL WITH OVERFLOWING INTEGERS --> CONVERT TO LONGS ETC	

	public int hash(int x){
		//h(x) = ((a*x)/16)%(r)
		int r = this.size;
		long h = ((this.a1*x) / (65536));
		//StdOut.println(h);
		h = h%r;
		return (int)h;
	}

	public int rehash(int x){
		//h(x) = ((a*x)/16)%(r)
		long y = (long) x;
		int r = this.size;
		//StdOut.println(" " + this.a2);
		//StdOut.println("r " + r);
		long g = ((this.a2*y) / (65536));
		//StdOut.println("g " + g);
		int h = (int)(g%r);
		return h;
	}


	public void grow(){
		if (this.size < ((2 * e)*this.count)){
			int[][] tempTable = new int[2][this.size*2];
			//takes a lot of time, but hopefully it is okay
			//should only occur after ~138 entries for the first instance and then doubles
			for (int i = 0; i < this.size; i++){
				for (int j = 0; j < 2; j++){
					tempTable[j][i] = this.table[j][i];
				}
			}
			this.table = tempTable;
			this.size = this.size * 2;
			if (this.verbose == true){
				StdOut.println("(hash " + this.a1 + " " + this.a2 + " " + this.size + ")");
			}
		}
	}

	//Inset pair into hash table
	public void put(int key, int value){
		int h = this.hash(key); //1st hash
		//StdOut.println("H = " + h);
		if (this.table[0][h] == -1){
			//if 1st hashed index is open, then put key and value in at index
			//StdOut.println("First Hash");
			this.table[0][h] = key;
			this.table[1][h] = value;
			if (this.verbose == true){
				StdOut.println("(" + h + " " + key + " " + value + ")");
			}
		}else{ //1st hashed index is taken, rehash the key
			int g = this.rehash(key);
			//StdOut.println("G = " + g);
			if (this.table[0][g] == -1){
				//2nd hashed index is free; put the key and value in at 2nd index
				//StdOut.println("Second Hash");
				this.table[0][g] = key;
				this.table[1][g] = value;
				if (this.verbose == true){
					StdOut.println("(" + g + " " + key + " " + value + ")");
				}
			}else{
				//2nd hashed index is not free; take original key/val from 1st index
				//put key and value in at first index
				//recursively call put for displaced key/value
				//StdOut.println("Third + Recursive");
				int tkey = this.table[0][h];
				int tval = this.table[1][h];
				this.table[0][h] = key;
				this.table[1][h] = value;
				if (this.verbose == true){
					StdOut.println("(" + h + " " + key + " " + value + ")");
				}
				Lcount++;
				this.pushPut(tkey, tval); //maybe should add a 3rd arg for recursive sake
			}
		}
		//at the end of each put, rehash because reasons
		this.count++; //increment the listed number of items in table
		this.grow();
	}
	
	public void pushPut(int key, int value){
		if (Lcount > this.Lmax){
			this.grow();
		}
		int h = this.rehash(key);
		if (this.table[0][h] == -1){
			this.table[0][h] = key;
			this.table[1][h] = value;
			StdOut.println("(" + h + " " + key + " " + value + ")");
		}else{
			int tkey = this.table[0][h];
			int tval = this.table[1][h];
			Lcount++;
			pushPut(tkey, tval);
		}
	}
	
	//print value corresponding to key
	//print "none" if not found
	public int get(int key){
		int h = this.hash(key);
		if (this.table[0][h] == key){
			return this.table[1][h];
		}
		int g = this.rehash(key);
		if (this.table[0][g] == key){
			return this.table[1][g];
		}
		return -1;
	}
	
	//delete key from table, no output
	public void delete(int key){
		int h = this.hash(key);
		if (this.table[0][h] == key){
			this.table[0][h] = -1;
			this.table[1][h] = -1;
			return ;
		}
		int g = this.rehash(key);
		if (this.table[0][g] == key){
			this.table[0][g] = -1;
			this.table[1][g] = -1;
		}
	}
	
	//print yes if key is in table, print no if otherwise
	public boolean contains(int key){
		//maybe merge the two conditional statements
		int h = this.hash(key);
		if (this.table[0][h] == key){	
			return true;
		}
		int g = this.rehash(key);
		if (this.table[0][g] == key){
			return true;
		}
		return false;
	}
	
	//if active, then print extra info
	public void setVerbose(boolean active){
		this.verbose = active;
	}

	//return the size of the hash map (the number of values)
	public int size(){
		return this.count;
	}
	
	public static void main(String args[]){
		int nCom = -1;
		Cuckoo cuckoo = new Cuckoo();
		cuckoo.initializeTable();
		boolean firstLine = true;
		while(StdIn.hasNextLine()){	
			if (firstLine == true){
				String s = StdIn.readLine();
				String[] sa = s.split(" ");
				long ia = Long.parseLong(sa[0], 10);
				long ib = Long.parseLong(sa[1], 10);
				cuckoo.seta1(ia);
				cuckoo.seta2(ib);
				nCom = Integer.parseInt(StdIn.readLine()); 
				firstLine = false;
			}else{
				String s = StdIn.readLine();
				if (s.contains(" ")){
					String[] sa = s.split(" ");
					if(sa[0].equals("put")){
						cuckoo.put(Integer.parseInt(sa[1]), Integer.parseInt(sa[2]));
					}
					if(sa[0].equals("delete")){
						cuckoo.delete(Integer.parseInt(sa[1]));
					}
					if(sa[0].equals("contains")){
						boolean boo = cuckoo.contains(Integer.parseInt(sa[1]));
						if (boo){ 
							StdOut.println("yes");
						}else{
							StdOut.println("no");
						}
					}
					if(sa[0].equals("get")){
						int x = cuckoo.get(Integer.parseInt(sa[1]));
						if (x != -1){
							StdOut.println(x);
						}else{
							StdOut.println("none");
						}
					}
				}else{
					StdOut.println(cuckoo.size());
				}
			}
		}	
	}
}

