/* Created on 4 mars 2006 */
package net.sf.prenzl.adapter;

import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.RuleProperties;


public abstract class Library {
	private final String name;
	
	public Library(String name){
		this.name = name;
	}
	
	public abstract String[] listRules();
	public abstract String[] listTopologies();
	public abstract String describeRuleProperties(String ruleName);
	public abstract Computation createComputation(
			String ruleName,
			int width, 
			int height, 
			byte[] firstGen,
			byte[] previous,
			String properties);
	
	public RuleProperties getDefaultRuleProperties(String ruleName){
		return new RuleProperties(describeRuleProperties(ruleName));
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
