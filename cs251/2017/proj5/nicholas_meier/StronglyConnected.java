import edu.princeton.cs.algs4.Graph;
import edu.princeton.cs.algs4.TarjanSCC;
import edu.princeton.cs.algs4.Digraph;
import java.lang.*;
import java.util.*;
/*
 * Project 5, Nicholas Meier
 * CS251, November 20th
 *
 */
public class StronglyConnected {

	public static Digraph dg; //Directed Graph for part 2
	public static ArrayList<String> edret = new ArrayList<String>(); //Used for printing edges

	public static void search(Graph g, int vc, boolean[] marked, int[][] ar){
		Iterable<Integer> ad = g.adj(vc);
		ArrayList<Integer> ads = new ArrayList<Integer>();
		for (int j : ad){
			ads.add(j);
		}
		Collections.sort(ads); //I didn't sort the edges when making the graph oops

		marked[vc] = true;
		for (int i : ads){
			if (!marked[i]){ //portion of code that does the DFS
				ar[i][vc]++;
				ar[vc][i]++;
				edret.add(vc + " " + i);
				search(g, i, marked, ar); //recursive call
				dg.addEdge(vc, i); //add edge to Digraph
			}else{ 
				//handles non-tree (non-DPS) edges
				if ((ar[vc][i] != 0 || ar[i][vc] != 0)){
					//uses 2d array system to prevent adding duplicates
					ar[i][vc]++;	
					ar[vc][i]++;
					edret.add(vc + " " + i);
					dg.addEdge(vc, i);
				}
			}
		}

	}
	
	//first DPS for kosa's alg.
	public static void kdps1(Stack<Integer> stack, int v, boolean marked[]){
		marked[v] = true;
		Iterable<Integer> ads = dg.adj(v);
		for (int j : ads){
			if (!marked[j]){
				kdps1(stack, j, marked);
			}
		}
		stack.push(new Integer(v));
		
	}
	
	//second DPS for kosa's alg.
	public static void kdps2(int v, boolean marked[], Digraph dgr, ArrayList<Integer> strcom){
		marked[v] = true;
		strcom.add(v);
	
		Iterable<Integer> ads = dgr.adj(v);
		for (int j : ads){
			if (!marked[j]){
				kdps2(j, marked, dgr, strcom);
			}
		}
		
		
	}

	//Customized Kosaraju Algorithm
	public static void kosa(int v){
		Stack<Integer> stack = new Stack<Integer>();
	
		boolean marked[] = new boolean[v];
		for(int i = 0; i < v; i++){
			if (marked[i] == false){
				kdps1(stack, i, marked);
			}
		}
		Digraph dgr = dg.reverse();
		for (int i = 0; i < v; i++){
			marked[i] = false;
		}
		ArrayList<ArrayList<Integer>> strcon = new ArrayList<ArrayList<Integer>>();
		while(stack.empty() == false){
			int i = (int)stack.pop();
			
			if (marked[i] == false){
				ArrayList<Integer> strcom = new ArrayList<Integer>();
				kdps2(i, marked, dgr, strcom);
				strcon.add(strcom);
			}
		}
		Digraph dag = new Digraph(strcon.size());
	
		//Best thing I could come up with to deal with 2d arraylists and finding stuff
		//very brute force-ish, but it works well enough and hopefully the break helps it
		//Consturcts a digraph which makes each SCC its own vertex
		for (int a = 0; a < strcon.size()-1; a++){
			for (int b = a+1; b < strcon.size(); b++){
				loop:
				for (int c = 0; c < strcon.get(a).size(); c++){
					for (int d = 0; d < strcon.get(b).size(); d++){
						int n = strcon.get(a).get(c);
						int m = strcon.get(b).get(d);
						
						Iterable<Integer> ads = dgr.adj(m);
						for (int z : ads){
							if (z == n){
								dag.addEdge(a, b);
								break loop;
							}
						}
					}
				}
			}
		}
		int outc = 0;
		int inc = 0;
		//Determines the number of edges needed to make a strong graph
		//counts the number of verticies with in/out deg. of zero
		for (int a = 0; a < strcon.size(); a++){
			if (dag.outdegree(a) == 0){ outc++; }
			if (dag.indegree(a) == 0) { inc++; }
		}
		if (outc > inc) { 
			StdOut.println(outc); 
		}else { StdOut.println(inc); }
	}

	public static void main(String args[]){
		boolean firstInt = true; //used for reading input

		//Initialize everything to null or -1
		int v = -1;
		int vert[] = null;
		int[][] ar = null;
		boolean[] marked = null;
		int e = -1;
		int big = -1;
		Graph g = null;

		while (StdIn.hasNextLine()){
			if (firstInt){
				v = Integer.parseInt(StdIn.readLine()); //number of verticies
				e = Integer.parseInt(StdIn.readLine()); //number of edges

				//initialize stuff
				g = new Graph(v);
				dg = new Digraph(v);
				big = v-1;

				//used for custom dfs and checking edge/vertex usage
				ar = new int[v][v];
				marked = new boolean[v];
				firstInt = false; //first 2 lines are read
			}else{	
				//read in edges from input and add them to graph
				String s = StdIn.readLine();
				String[] sa = s.split(" ");
				int a = Integer.parseInt(sa[0]);
				int b = Integer.parseInt(sa[1]);
				//used for dfs
				ar[a][b]--; 
				ar[b][a]--; 
				g.addEdge(a, b);
			}
		}

		int vc = big; //Start DFS with biggest vertex index

		//Depth first Search
		search(g, vc, marked, ar);

		//Determine number of Edges needed for strong connection
		//Initially use Tarjan algorithm from algs4
		//Use customized kosaraju alg. if not strongly connected initially
		TarjanSCC tj = new TarjanSCC(dg); 
		if (tj.count() == 1){
			StdOut.println(0); 
		}else{
			kosa(v);
		}
		//print edges from search
		for (String str : edret){
			StdOut.println(str);
		}
	}
}
