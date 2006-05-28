package net.sf.prenzl.launch;

import net.sf.prenzl.ICommandIds;
import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.views.Display2dView;

import org.eclipse.jface.action.Action;

public class NextStepAction extends Action {
	
	public NextStepAction() {
		setId(ICommandIds.CMD_NEXT_STEP);
		setActionDefinitionId(ICommandIds.CMD_NEXT_STEP);
    setImageDescriptor(SharedImages.IMGD_FORWARD);
	}
	
	public void run() {
		if(!PrenzlPlugin.getRunModel().isRunning()){
			Display2dView display2dView = (Display2dView)PrenzlPlugin.findView(Display2dView.ID);
			display2dView.getComputationUI().setToNextStep();
		}
	}

}
