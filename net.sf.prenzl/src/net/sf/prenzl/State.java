package net.sf.prenzl;

import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.launch.RuleDescriptor;

import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.ui.IMemento;
import org.eclipse.ui.IPersistableElement;
import org.eclipse.ui.XMLMemento;

public class State implements IPersistableElement, IAdaptable {
	private final static String KEY_WIDTH = "width";
	private final static String KEY_HEIGHT = "height";
	
	private final static String CHILD_LAUNCH_MODEL = "LaunchModel";
	private final static String KEY_LIBARY = "library";
	private final static String KEY_RULE = "rule";
	
	
	private IMemento memento;

	/** Default values*/
	public State(){
		memento = XMLMemento.createWriteRoot("State");
		memento.putInteger(KEY_WIDTH,800);
		memento.putInteger(KEY_HEIGHT,600);
		IMemento mementoLaunchModel = memento.createChild(CHILD_LAUNCH_MODEL);
		mementoLaunchModel.putString(KEY_LIBARY,"stdrules");
		mementoLaunchModel.putString(KEY_RULE,"Phase Cheulou");
	}
	public State(IMemento memento){
		this.memento = memento;
	}
	
	public void saveState(IMemento memento) {
		memento.putMemento(this.memento);
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

	public int getHeight() {
		return memento.getInteger(KEY_HEIGHT).intValue();
	}

	public int getWidth() {
		return memento.getInteger(KEY_WIDTH).intValue();
	}

	public void setSize(int width, int height){
		memento.putInteger(KEY_WIDTH,width);
		memento.putInteger(KEY_HEIGHT,height);
	}
	
	public void restoreLaunchModel(LaunchModel launchModel){
		IMemento mementoLaunchModel = memento.getChild(CHILD_LAUNCH_MODEL);
		launchModel.setRuleDescriptor(new RuleDescriptor(
				LaunchModel.findLibrary(mementoLaunchModel.getString(KEY_LIBARY)),
				mementoLaunchModel.getString(KEY_RULE)));
	}
	
	public void saveLaunchModel(LaunchModel launchModel){
		IMemento mementoLaunchModel = memento.getChild(CHILD_LAUNCH_MODEL);
		mementoLaunchModel.putString(KEY_LIBARY,launchModel.getRuleDescriptor().getLibrary().getName());
		mementoLaunchModel.putString(KEY_RULE,launchModel.getRuleDescriptor().getRuleName());
	}
	
//	public RuleDescriptor getRuleDescriptor() {
//		return new RuleDescriptor(
//				LaunchModel.findLibrary(memento.getString(KEY_LIBARY)),
//				memento.getString(KEY_RULE));
//	}
		
//	public void setRule(RuleDescriptor rd){
//		memento.putString(KEY_LIBARY,rd.getLibrary().getName());
//		memento.putString(KEY_RULE,rd.getRuleName());
//	}
}
