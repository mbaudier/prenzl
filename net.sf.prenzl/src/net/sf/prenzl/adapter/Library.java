/* Created on 4 mars 2006 */
package net.sf.prenzl.adapter;

import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.RuleProperties;



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
	public abstract String describeRuleProperties(String ruleName);
	
	public RuleProperties getDefaultRuleProperties(String ruleName){
		return new RuleProperties(describeRuleProperties(ruleName));
	}
	
	protected abstract int init(
			String ruleName, 
			String topologyName, 
			int width, 
			int height, 
			byte[] firstGen, 
			byte[] previous, 
			String properties);
	
	/** If previous is null, firstgen will be used to initialize it*/
	public synchronized Computation createComputation(
			String ruleName,
			int width, 
			int height, 
			byte[] firstGen,
			byte[] previous,
			String properties)
	{
		Computation computation = null;
		
		if(type==TYPE_JNI){
			byte[] previousTemp = previous!=null?previous:firstGen;
			int id = init(ruleName,null,width,height,firstGen,previousTemp,properties);
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

	public boolean equals(Object o){
		if(o instanceof Configuration){
			Library other = (Library)o;
			return other.name.equals(name);
		}
		return false;
	}

}
