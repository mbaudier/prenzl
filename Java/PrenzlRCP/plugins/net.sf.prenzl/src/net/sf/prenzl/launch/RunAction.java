package net.sf.prenzl.launch;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.ICommandIds;
import net.sf.prenzl.Log;
import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;

import org.eclipse.jface.action.Action;

public class RunAction extends Action implements Observer {
	
	private final PreviousStepAction previousStepAction;
	private final NextStepAction nextStepAction;
	
	public RunAction(PreviousStepAction previousStepAction, NextStepAction nextStepAction) {
		this.previousStepAction = previousStepAction;
		this.nextStepAction = nextStepAction;
		setId(ICommandIds.CMD_RUN);
		setActionDefinitionId(ICommandIds.CMD_RUN);

		update(PrenzlPlugin.getRunModel(), null);
		PrenzlPlugin.getRunModel().addObserver(this);
	}
	
	public void run() {
		try {
			RunModel runModel = PrenzlPlugin.getRunModel();
			if(runModel.isRunning()){
				runModel.setRunning(false);
			}
			else{
				runModel.setRunning(true);
			}

		} catch (Exception e) {
			Log.error("Could not set running flag",e);
		}
		
	}

	public void update(Observable o, Object arg) {
		if(o==PrenzlPlugin.getRunModel()){
			RunModel runModel = PrenzlPlugin.getRunModel();
			if(!runModel.isRunning()){
		    setToolTipText("Run");
		    setImageDescriptor(SharedImages.IMGD_RUN);
		    previousStepAction.setEnabled(PrenzlPlugin.getRunModel().getCount()>0?true:false);
		    nextStepAction.setEnabled(true);
			}
			else{
				setToolTipText("Stop");
		    setImageDescriptor(SharedImages.IMGD_STOP);
		    previousStepAction.setEnabled(false);
		    nextStepAction.setEnabled(false);
			}
			
		}
	}
	
	
}
