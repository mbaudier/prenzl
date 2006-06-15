package net.sf.prenzl.ui.views;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.launch.RuleProperties;
import net.sf.prenzl.ui.RulePropertiesUI;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

public class PropertiesView extends ViewPart implements Observer{
	public static final String ID = PrenzlPlugin.ID+".propertiesView";
	
	private RulePropertiesUI propertiesUI;
	
	/** Creates the viewer and initialize it.*/
	public void createPartControl(Composite parent) {
		propertiesUI = RulePropertiesUI.createRulePropertiesUI(parent,null);//TODO: pass library
		propertiesUI.setProperties(PrenzlPlugin.getLaunchModel().getRuleDescriptor().getDefaultProperties());
		PrenzlPlugin.getLaunchModel().addObserver(this);
	}

	public void setProperties(RuleProperties properties) {
		propertiesUI.setProperties(properties);
	}

	public void update(Observable o, Object arg) {
		if(o instanceof LaunchModel){
			LaunchModel launchModel = PrenzlPlugin.getLaunchModel();
			if(arg instanceof Configuration){
				setProperties(launchModel.getConfiguration().getRuleProperties());
			}
		}
	}

	/** Passing the focus request to the viewer's control.*/
	public void setFocus() {
		if(propertiesUI!=null)propertiesUI.setFocus();
	}

	public void dispose() {
		if(propertiesUI!=null)propertiesUI.dispose();
		super.dispose();
	}
}