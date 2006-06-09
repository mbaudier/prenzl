package net.sf.prenzl.ui;

import org.eclipse.swt.widgets.Composite;

import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.launch.RuleProperties;

public abstract class RulePropertiesUI {
	public static RulePropertiesUI createRulePropertiesUI(Composite parent, Library library){
		return new DefaultRulePropertiesUI(parent);
	}
	
	public abstract void dispose();
	public abstract void setFocus();
	public abstract void setProperties(RuleProperties properties);
}
