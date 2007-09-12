package net.sf.prenzl.stdrules;

import net.sf.prenzl.adapter.Computation;


class JNIComputation extends Computation {
	
	private final int id;
	JNIComputation(String name, int id){
		super(name);
		this.id = id;
	}
	
	/** Computes one generation*/
	public synchronized native void compute();
	/** Retrieves one generation from native side
	 * @param imageBuffer the array where the native side will write*/
	public synchronized native void getCurrent(byte[] imageBuffer);
	/** Cleans assotiated resources and removes it from the registry*/
	public synchronized native void cleanUp();

	public int getId(){
		return id;
	}
}
