package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.ICommandIds;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.launch.ICountListener;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.Action;

public class PreviousStepAction extends Action implements Observer,ICountListener{
	
	private final ComputationUI computationUI;
	
	public PreviousStepAction(ComputationUI computationUI) {
		setId(ICommandIds.CMD_PREVIOUS_STEP);
		setActionDefinitionId(ICommandIds.CMD_PREVIOUS_STEP);
		setImageDescriptor(SharedImages.IMGD_BACKWARD);
		
		this.computationUI = computationUI;
		this.computationUI.addObserver(this);
		this.computationUI.addCountListener(this);

	//    setAccelerator(SWT.PAGE_DOWN);
	}
	
	public void run() {
		if(!computationUI.isRunning()){
			computationUI.setToPreviousStep();
		}
	}


	public void update(Observable o, Object arg) {
		if(o == computationUI){
			setEnabled(!computationUI.isRunning() && !(computationUI.getCycleCount()==0));
		}
		
	}

	public void setCount(int count) {
		if(count==0 || count==1){
			setEnabled(!computationUI.isRunning() && !(computationUI.getCycleCount()==0));
		}
	}

}
