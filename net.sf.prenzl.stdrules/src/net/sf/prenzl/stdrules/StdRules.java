/* Created on 7 mars 2006 */
package net.sf.prenzl.stdrules;

import net.sf.prenzl.adapter.Library;

public class StdRules extends Library{
	public static final String LIB_NAME = "stdrules";
	
	static{
		System.loadLibrary(LIB_NAME);
	}
	
	public StdRules()  {
		super(LIB_NAME,TYPE_JNI);
	} 

	protected synchronized native int init(String ruleName, String topologyName,int width, int height, byte[] firstGen, byte[] previous, String properties);
	public native String[] listComputations();
	public native String[] listTopologies();
	public native String describeRuleProperties(String ruleName);
}
