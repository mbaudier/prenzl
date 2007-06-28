package net.sf.prenzl.ui.views;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.ICountListener;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.ui.actions.DisplayModeCI;
import net.sf.prenzl.ui.actions.FirstGenerationAction;
import net.sf.prenzl.ui.actions.NextStepAction;
import net.sf.prenzl.ui.actions.PreviousStepAction;
import net.sf.prenzl.ui.actions.RunAction;
import net.sf.prenzl.ui.actions.SaveImageAction;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.IStatusLineManager;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.ui.part.ViewPart;

public class Display2dView extends ViewPart implements Observer, ICountListener{
	public static final String ID = PrenzlPlugin.ID+".display2dView";
	

	private ComputationUI computationUI = null;
	private MenuManager menuManager = null;
	private IStatusLineManager statusLineManager = null;
	
	/** Creates the viewer and initialize it.*/
	public void createPartControl(Composite parent) {
		computationUI = new ComputationUI(parent, new ComputationUIlMouseListener());
		parent.setToolTipText("Double-click to toggle full screen");

		FirstGenerationAction firstGenerationAction = new FirstGenerationAction(computationUI);
		PreviousStepAction previousStepAction = new PreviousStepAction(computationUI);
		NextStepAction nextStepAction = new NextStepAction(computationUI);
		RunAction runAction = new RunAction(computationUI);

		SaveImageAction saveDisplay = 
			new SaveImageAction(computationUI,true);
		SaveImageAction saveImage = 
			new SaveImageAction(computationUI,false);

		IToolBarManager toolBarManager= getViewSite().getActionBars().getToolBarManager();
	    toolBarManager.add(firstGenerationAction);
	    toolBarManager.add(previousStepAction);
	    toolBarManager.add(nextStepAction);
	    toolBarManager.add(runAction);
	    //toolBarManager.add();
	
	    RunAction runActionText = new RunAction(computationUI,"");
	    DisplayModeCI displayModeCIOriginal = new DisplayModeCI(computationUI);
		menuManager = new MenuManager();
		menuManager.add(runActionText);
		menuManager.add(new Separator());
		menuManager.add(displayModeCIOriginal);
		menuManager.add(new Separator());
		menuManager.add(saveDisplay);
		menuManager.add(saveImage);

		IMenuManager viewMenuManager = getViewSite().getActionBars().getMenuManager();
		viewMenuManager.add(runActionText);
		viewMenuManager.add(new Separator());
		viewMenuManager.add(displayModeCIOriginal);
 		viewMenuManager.add(new Separator());
 		viewMenuManager.add(saveDisplay);
 		viewMenuManager.add(saveImage);
       
		statusLineManager = getViewSite().getActionBars().getStatusLineManager();
		computationUI.addCountListener(this);
		
		PrenzlPlugin.getLaunchModel().addObserver(this);
	}

	
	public void update(Observable observable, Object arg) {
		if (observable==PrenzlPlugin.getLaunchModel() || observable == null) {
			LaunchModel launchModel = PrenzlPlugin.getLaunchModel();
			if(arg instanceof Configuration){
				Configuration configuration = (Configuration)arg; 
				computationUI.reset(configuration);
				setPartName(configuration.getRuleName());
			}
			else if(arg instanceof String){
				Rectangle bounds = getSite().getWorkbenchWindow().getShell().getDisplay().getBounds();
				
				computationUI.reset(launchModel.loadComputationInput(bounds.width,bounds.height));
				
			}
		}
	}

	/** Passing the focus request to the viewer's control.*/
	public void setFocus() {
		computationUI.setFocus();
	}

	public void dispose() {
		PrenzlPlugin.getLaunchModel().deleteObserver(this);
		computationUI.dispose();
		super.dispose();
	}

	public ComputationUI getComputationUI() {
		return computationUI;
	}
	public void setCount(final int count) {
		if(!getSite().getWorkbenchWindow().getShell().isDisposed()){
			getSite().getWorkbenchWindow().getShell().getDisplay().asyncExec(new Runnable(){
				public void run(){
					if(statusLineManager!=null)
						statusLineManager.setMessage(Integer.toString(count));
				}
			});
		}
	}

	
	
	/** Separate MenuManager code from ComputationUI in order to remove a dependency to JFace*/
	private class ComputationUIlMouseListener implements MouseListener {
		public void mouseDoubleClick(MouseEvent e) {}
		public void mouseUp(MouseEvent e) {}

		public void mouseDown(MouseEvent e) {
			if (e.button == 2 || e.button == 3) {
				Point pt = computationUI.getDc().getLabel().toDisplay(new Point(e.x, e.y));
				Menu menu = menuManager.createContextMenu(computationUI.getDc().getLabel());
				menu.setLocation(pt.x, pt.y);
				menu.setVisible(!menu.isVisible());
			}
		}

	}


}