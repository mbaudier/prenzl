package net.sf.prenzl.launch;

import net.sf.prenzl.adapter.Library;

public class RuleDescriptor {
	private final Library library; 
	private final String ruleName; 

	public RuleDescriptor(Library library, String ruleName) {
		this.library = library;
		this.ruleName = ruleName;
	}

	public Library getLibrary() {
		return library;
	}

	public String getRuleName() {
		return ruleName;
	}
	
	public RuleProperties getDefaultProperties(){
		return library.getDefaultRuleProperties(ruleName);
	}
	public boolean equals(Object o){
		if(o instanceof Configuration){
			RuleDescriptor other = (RuleDescriptor)o;
			return other.ruleName.equals(ruleName) && other.library.equals(library);
		}
		return false;
	}

}
