/* Created on 4 mars 2006 */
package net.sf.prenzl.adapter;



public abstract class Library {
	public final static int TYPE_JNI = 1;
	public final static int TYPE_JAVA = 2;
	
	private final int type;
	private final String name;
	
	public Library(String name, int type){
		this.name = name;
		this.type = type;
	}
	
	public abstract String[] listComputations();
	public abstract String[] listTopologies();
	protected abstract int init(String ruleName, String topologyName, int width, int height, byte[] firstGen, String properties);
	
	public synchronized Computation createComputation(String ruleName,int width, int height, byte[] firstGen){
		Computation computation = null;
		
		if(type==TYPE_JNI){			
			int id = init(ruleName,null,width,height,firstGen,null);
			computation = new JNIComputation(ruleName,id);
		}
		else{
			throw new RuntimeException("Type "+type+" is not supported.");
		}
		return computation;
	}

	public synchronized String getName() {
		return name;
	}


}
