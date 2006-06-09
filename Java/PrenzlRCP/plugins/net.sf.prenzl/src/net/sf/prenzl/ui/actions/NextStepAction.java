package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.ICommandIds;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.Action;

public class NextStepAction extends Action implements Observer{
	
	private final ComputationUI computationUI;

	public NextStepAction(ComputationUI computationUI) {
		setId(ICommandIds.CMD_NEXT_STEP);
		setActionDefinitionId(ICommandIds.CMD_NEXT_STEP);
	    setImageDescriptor(SharedImages.IMGD_FORWARD);
	    
	   // setAccelerator(SWT.PAGE_UP);
	    
		this.computationUI = computationUI;
		this.computationUI.addObserver(this);
	}
	
	public void run() {
		if(!computationUI.isRunning()){
			computationUI.setToNextStep();
		}
	}

	public void update(Observable o, Object arg) {
		if(o == computationUI){
			setEnabled(!computationUI.isRunning());
		}
		
	}
}
