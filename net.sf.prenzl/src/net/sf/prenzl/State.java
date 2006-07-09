package net.sf.prenzl;

import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.launch.RuleDescriptor;

import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IMemento;
import org.eclipse.ui.IPersistableElement;

public class State implements IPersistableElement, IAdaptable {
	private final static String KEY_SHELL_X = "shell.x";
	private final static String KEY_SHELL_Y = "shell.y";
	private final static String KEY_SHELL_WIDTH = "shell.width";
	private final static String KEY_SHELL_HEIGHT = "shell.height";
	
	private final static String CHILD_LAUNCH_MODEL = "LaunchModel";
	private final static String KEY_LIBARY = "library";
	private final static String KEY_RULE = "rule";
	private final static String KEY_LOAD_MODE = "loadMode";
	
	private final static String CHILD_COMPUTATION_INPUT = "ComputationInput";
	private final static String KEY_PATH= "path";
	
	private Point shellLocation, shellSize;
	private RuleDescriptor ruleDescriptor;
	private List inputLocations;
	private int loadMode;

	/** Default values*/
	public State(){
		shellLocation = new Point(200,200);
		shellSize = new Point(800,600);
		ruleDescriptor = new RuleDescriptor(LaunchModel.findLibrary("stdrules"),"Phase Cheulou");
		inputLocations = new Vector();
		loadMode = LaunchModel.LOADMODE_ORIGINAL;
	}
	public State(IMemento memento){
		shellLocation = new Point(memento.getInteger(KEY_SHELL_X).intValue(),memento.getInteger(KEY_SHELL_Y).intValue());
		shellSize = new Point(memento.getInteger(KEY_SHELL_WIDTH).intValue(),memento.getInteger(KEY_SHELL_HEIGHT).intValue());
		
		IMemento mementoLM = memento.getChild(CHILD_LAUNCH_MODEL);
		ruleDescriptor = new RuleDescriptor(
				LaunchModel.findLibrary(mementoLM.getString(KEY_LIBARY)),
				mementoLM.getString(KEY_RULE));
		loadMode = mementoLM.getInteger(KEY_LOAD_MODE).intValue();
		
		inputLocations = new Vector();
		IMemento[] ci = mementoLM.getChildren(CHILD_COMPUTATION_INPUT);
		for (int i = 0; i < ci.length; i++) {
			inputLocations.add(ci[i].getString(KEY_PATH));
		}
	}
	
	
	
	public void saveState(IMemento memento) {
		memento.putInteger(KEY_SHELL_X,shellLocation.x);
		memento.putInteger(KEY_SHELL_Y,shellLocation.y);
		memento.putInteger(KEY_SHELL_WIDTH,shellSize.x);
		memento.putInteger(KEY_SHELL_HEIGHT,shellSize.y);
		
		IMemento mementoLM = memento.createChild(CHILD_LAUNCH_MODEL);
		mementoLM.putString(KEY_LIBARY,ruleDescriptor.getLibrary().getName());
		mementoLM.putString(KEY_RULE,ruleDescriptor.getRuleName());
		mementoLM.putInteger(KEY_LOAD_MODE,loadMode);
		
		Iterator it = inputLocations.iterator();
		while (it.hasNext()) {
			String ci = (String) it.next();
			IMemento mementoCi = mementoLM.createChild(CHILD_COMPUTATION_INPUT);
			mementoCi.putString(KEY_PATH,ci);
		}
	}

	public String getFactoryId() {
		return StateFactory.ID;
	}

	public Object getAdapter(Class adapter) {
		if(adapter.equals(State.class)){
			return this;
		}
		return null;
	}

	public Point getShellSize(){
		return shellSize;
	}
	
	public Point getShellLocation(){
		return shellLocation;
	}
	
	public void saveShell(Shell shell){
		Rectangle bounds = shell.getBounds();
		shellLocation = new Point(bounds.x,bounds.y);
		shellSize = new Point(bounds.width,bounds.height);
	}
	
	public void restoreLaunchModel(LaunchModel launchModel){
		launchModel.setRuleDescriptor(ruleDescriptor);
		launchModel.setLoadMode(loadMode);// has to be before load lastInput
		launchModel.addLastInputLocations(inputLocations);
	}
	
	public void saveLaunchModel(LaunchModel launchModel){
		ruleDescriptor = launchModel.getRuleDescriptor();
		inputLocations = launchModel.getLastInputLocations();
		loadMode = launchModel.getLoadMode();
	}
}
