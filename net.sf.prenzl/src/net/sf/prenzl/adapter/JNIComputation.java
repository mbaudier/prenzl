package net.sf.prenzl.adapter;


class JNIComputation extends Computation {
  private static String LIB_NAME = "prenzl";

  /** Creates registry on native side*/
  public synchronized static native void initRegistry();
  /** Cleans registry on native side*/
	public synchronized static native void cleanRegistry();

	static{
		System.loadLibrary(LIB_NAME);
	}
	
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
	private synchronized native void clean();

	public void cleanUp(){
		clean();
	}

	public int getId(){
		return id;
	}
}
