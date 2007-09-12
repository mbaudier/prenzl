/* Created on 7 mars 2006 */
package net.sf.prenzl.stdrules;

import net.sf.prenzl.adapter.Computation;
import net.sf.prenzl.adapter.Library;

public class StdRules extends Library {
	public static final String LIB_NAME = "stdrules";

	static {
		System.loadLibrary(LIB_NAME);
	}

	public StdRules() {
		super(LIB_NAME);
	}

	protected synchronized native int initJNIComputation(String ruleName,
			String topologyName, int width, int height, byte[] firstGen,
			byte[] previous, String properties);

	public native String[] listJNIRules();

	public native String[] listJNITopologies();

	public native String describeJNIRuleProperties(String ruleName);

	/** If previous is null, firstgen will be used to initialize it */
	public synchronized Computation createComputation(String ruleName,
			int width, int height, byte[] firstGen, byte[] previous,
			String properties) {
		Computation computation = null;
		byte[] previousTemp = previous != null ? previous : firstGen;
		int id = initJNIComputation(ruleName, null, width, height, firstGen, previousTemp,
				properties);
		computation = new JNIComputation(ruleName, id);
		return computation;
	}

	public String describeRuleProperties(String ruleName) {
		return describeJNIRuleProperties(ruleName);
	}

	public String[] listRules() {
		return listJNIRules();
	}

	public String[] listTopologies() {
		return listJNITopologies();
	}

}
