package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.Action;

public class NextStepAction extends Action implements Observer{
	
	private final ComputationUI computationUI;

	public NextStepAction(ComputationUI computationUI) {
		this.computationUI = computationUI;
		this.computationUI.addObserver(this);
	    setImageDescriptor(SharedImages.IMGD_FORWARD);
	    
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
