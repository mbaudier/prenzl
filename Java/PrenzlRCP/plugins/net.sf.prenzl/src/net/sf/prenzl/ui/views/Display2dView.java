package net.sf.prenzl.ui.views;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.ui.ComputationUI;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

public class Display2dView extends ViewPart implements Observer{
	public static final String ID = PrenzlPlugin.ID+".display2dView";
	

	private ComputationUI computationUI = null;
	
	/** Creates the viewer and initialize it.*/
	public void createPartControl(Composite parent) {
		computationUI = new ComputationUI(parent);
		
		PrenzlPlugin.getLaunchModel().addObserver(this);
		PrenzlPlugin.getRunModel().addObserver(this);
		refresh();
	}

	public void update(Observable o, Object arg) {
		if (o==PrenzlPlugin.getLaunchModel()) {
			refresh();					
		}
		else if (o==PrenzlPlugin.getRunModel()) {
			if(computationUI!=null)
				computationUI.setRunning(PrenzlPlugin.getRunModel().isRunning());					
		}
	}

	/** Passing the focus request to the viewer's control.*/
	public void setFocus() {
		computationUI.setFocus();
	}

	public void refresh(){
		computationUI.paintBackground();
		
		LaunchModel launchModel = PrenzlPlugin.getLaunchModel();
		if(launchModel.isReadyToLaunch()){
			Library library = launchModel.getLibrary();
			String ruleName = launchModel.getRuleName();
			String firstGenPicPath = launchModel.getPicturePath();
			computationUI.reset(library,ruleName,firstGenPicPath);
		}
	}
	
	public void dispose() {
		PrenzlPlugin.getLaunchModel().deleteObserver(this);
		PrenzlPlugin.getRunModel().deleteObserver(this);
		computationUI.dispose();
		super.dispose();
	}

	public ComputationUI getComputationUI() {
		return computationUI;
	}

}