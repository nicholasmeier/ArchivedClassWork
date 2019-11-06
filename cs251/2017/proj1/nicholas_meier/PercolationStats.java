public class PercolationStats{
	public static void main(String[] args){
		int N = Integer.parseInt(args[0]);
		int T = Integer.parseInt(args[1]);
		String type = args[2];
		double est[] = new double[T];
		double elstime[] = new double[T];
		double time = 0;
		if (type.charAt(0) == 'f'){
			Stopwatch s1 = new Stopwatch();
			for (int z = 0; z < T; z++){
				Percolation ps = new Percolation(N);
				while(!(ps.percolates())){
					int x = StdRandom.uniform((N));
					int y = StdRandom.uniform((N));
					ps.open(x, y);
					//ps.buttUnion();
				}
				elstime[z] = s1.elapsedTime();
				est[z] = ( (double)(ps.numOpened()) ) / ( (double)(N*N) );
			}
			time = s1.elapsedTime();
		}else {
			if (type.charAt(0) == 's'){
				Stopwatch s2 = new Stopwatch();
				for (int z = 0; z < T; z++){
				PercolationSlow pss = new PercolationSlow(N);
					while(!(pss.percolates())){
						int x = StdRandom.uniform((N));
						int y = StdRandom.uniform((N));
						pss.open(x, y);
						//pss.buttUnion();
					}
					elstime[z] = s2.elapsedTime();
					est[z] = ( (double)(pss.numOpened()) ) / ( (double)(N*N) );
				}
				time = s2.elapsedTime();
			}
		}
		double estp = StdStats.mean(est);
		double stdp = StdStats.stddev(est);
		double avgt = StdStats.mean(elstime);
		double stdt = StdStats.stddev(elstime);
		System.out.println("Mean threshold =  " + estp);
		System.out.println("std dev = " + stdp);
		System.out.println("Total time = " + time);
		System.out.println("Average time = " + avgt);
		System.out.println("std dev time = " + stdt);
	}
}
