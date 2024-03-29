package net.sf.prenzl;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;

import net.sf.prenzl.util.Log;

import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IMemento;
import org.eclipse.ui.XMLMemento;
import org.eclipse.ui.application.ActionBarAdvisor;
import org.eclipse.ui.application.IActionBarConfigurer;
import org.eclipse.ui.application.IWorkbenchWindowConfigurer;
import org.eclipse.ui.application.WorkbenchWindowAdvisor;

public class ApplicationWorkbenchWindowAdvisor extends WorkbenchWindowAdvisor {
	public final static String PREVIOUS_STATE_FILE = "previousState.xml";
	private State state;

	public ApplicationWorkbenchWindowAdvisor(IWorkbenchWindowConfigurer configurer) {
		super(configurer);
	}

	public ActionBarAdvisor createActionBarAdvisor(
			IActionBarConfigurer configurer) {
		return new ApplicationActionBarAdvisor(configurer);
	}

	public void preWindowOpen() {
		restorePreviousState();
		
		IWorkbenchWindowConfigurer configurer = getWindowConfigurer();
		
		configurer.setInitialSize(state.getShellSize());
		configurer.setShowCoolBar(true);
		configurer.setShowStatusLine(true);
		configurer.setTitle("Prenzl!!");
	}

	public void postWindowOpen() {
		state.restoreLaunchModel(PrenzlPlugin.getLaunchModel());
		super.postWindowOpen();
	}

	public void createWindowContents(Shell shell) {
		super.createWindowContents(shell);
		//state.restoreShell(shell);
		shell.setLocation(state.getShellLocation());
	}

	public boolean preWindowShellClose() {
		savePreviousState();
		return super.preWindowShellClose();
	}

	private void restorePreviousState(){
		try {
			File file = new File(PrenzlPlugin.getDefault().getStateLocation()+File.separator+PREVIOUS_STATE_FILE);
			if(file.exists()){
				FileReader reader = new FileReader(file);
				IMemento memento = XMLMemento.createReadRoot(reader);
				reader.close();
				StateFactory factory = (StateFactory)PrenzlPlugin.getDefaultWorkbench().getElementFactory(StateFactory.ID);
				state = (State)factory.createElement(memento).getAdapter(State.class);
				
			}
		} 
		catch (Exception e) {
			Log.error("Cannot load previous state",e);
		}
		if(state==null)state = new State();// default values
	}
	
	private void savePreviousState(){
		Shell shell = PrenzlPlugin.getDefaultWorkbench().getActiveWorkbenchWindow().getShell();
		state.saveShell(shell);
//		state.saveShell(shell.getBounds());
		state.saveLaunchModel(PrenzlPlugin.getLaunchModel());

		XMLMemento memento = XMLMemento.createWriteRoot("State");
		state.saveState(memento);
		try {
			FileWriter writer = new FileWriter(PrenzlPlugin.getDefault().getStateLocation()+File.separator+PREVIOUS_STATE_FILE);
			memento.save(writer);
			writer.close();
		} catch (Exception e) {
			Log.error("Cannot save previous state",e);
		} 
	}

}
