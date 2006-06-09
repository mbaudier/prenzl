package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.ICommandIds;
import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.Action;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;

public class RunAction extends Action implements Observer {
	
	private final ComputationUI computationUI;
	
	public RunAction(ComputationUI computationUI) {
		setId(ICommandIds.CMD_RUN);
		setActionDefinitionId(ICommandIds.CMD_RUN);

		this.computationUI = computationUI; 
		this.computationUI.addObserver(this);
		
	   // setAccelerator(' ');

	    update(this.computationUI, null);
	}
	
	public RunAction(ComputationUI computationUI, String text) {
		this(computationUI);
		setText(text);
	}
	
	public void run() {
		if(computationUI.isRunning()){
			computationUI.setRunning(false);
			
			ImageData id = (ImageData)computationUI.getImageData().clone();
			float factor; 
			if(id.width>=id.height){
				factor = 128f/id.width;
			}
			else{
				factor = 128f/id.height;
			}
			id.scaledTo((int)(id.width*factor), (int)(id.height*factor));
			PrenzlPlugin.getLaunchModel().setConfigurationImage(
					new Image(computationUI.getDc().getDisplay(),id));
		}
		else{
			computationUI.setRunning(true);
			PrenzlPlugin.getLaunchModel().notifyConfigurationLaunch();
		}
	}

	public void update(Observable o, Object arg) {
		if(o==computationUI){
			if(!computationUI.isRunning()){
			    setToolTipText("Run");
			    if(getText()!=null){
			    	setText("Run");
			    }
			    setImageDescriptor(SharedImages.IMGD_RUN);
			}
			else{
				setToolTipText("Pause");
			    if(getText()!=null){
			    	setText("Pause");
			    }
				setImageDescriptor(SharedImages.IMGD_STOP);
			}
			
		}
	}
	
	
}
