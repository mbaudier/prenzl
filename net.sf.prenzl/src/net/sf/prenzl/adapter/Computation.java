package net.sf.prenzl.adapter;

public abstract class Computation {
	private final String name;
	
	//public abstract void init(int width, int height, byte[] firstGen);
	public abstract void compute();
	public abstract void getCurrent(byte[] imageBuffer);
	public abstract void cleanUp();

	public Computation(String name){
		this.name = name;
	}
	
	public String getName(){
		return name;
	}
	
	public static void initRegistries(){
		JNIComputation.initRegistry();
	}
	public static void cleanRegistries(){
		JNIComputation.cleanRegistry();
	}
}
