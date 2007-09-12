/* Created on 30 janv. 2006 */
package net.sf.prenzl.ui.toolbars.launch;

import java.util.List;
import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.launch.RuleDescriptor;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

public class ComputationContributionItem extends ContributionItem {

	private final static int WIDTH = 100;
	
	public void fill(final ToolBar parent, int index) {
		final ToolItem toolItem = new ToolItem(parent,SWT.PUSH);
		toolItem.setText("<click to choose rule>");
		toolItem.setImage(SharedImages.IMG_RULE);
		toolItem.setToolTipText("Click to choose a rule in one of the libraries");
		toolItem.setWidth(WIDTH);
		toolItem.addListener(SWT.Selection, new EvtListener(toolItem,parent));
		super.fill(parent, index);
	}
	
	private class EvtListener implements Listener,Observer{
		final private ToolItem toolItem;
		final private ToolBar toolBar;
		
		private EvtListener(ToolItem toolItem, ToolBar toolBar){
			this.toolItem = toolItem;
			this.toolBar = toolBar;
			PrenzlPlugin.getLaunchModel().addObserver(this);
			
			List libraries = PrenzlPlugin.getLaunchModel().getLibraries();
			if(libraries!=null && libraries.size()>0){
				Library library = (Library)libraries.get(0);
				String ruleNames[] = library.listRules();
				if(ruleNames!=null && ruleNames.length>0){
					String firstRule = library.listRules()[0];
					updateComputation(library,firstRule);
				}
			}
		}
		
		public void update(Observable o, Object arg) {
			if(o==PrenzlPlugin.getLaunchModel()){
				String ruleName = PrenzlPlugin.getLaunchModel().getRuleDescriptor().getRuleName();
				if(ruleName!=null){
					toolItem.setText(ruleName);
					//toolItem.setToolTipText(ruleName);
					toolItem.setWidth(WIDTH);
				}
			}
		}

		public void handleEvent(Event event) {
			Rectangle rect = toolItem.getBounds();
	    //if (event.detail == SWT.ARROW) {
	      Point pt = new Point(rect.x, rect.y + rect.height);
	      createComputationListMenu(pt).setVisible(true);
	   // }
		}
		
		private Menu createComputationListMenu(Point pt){
	    pt = toolBar.toDisplay(pt);
	    
	    Menu menu = new Menu(toolBar.getShell(), SWT.POP_UP);
	    
	    List libraries = PrenzlPlugin.getLaunchModel().getLibraries();
	    for(int j=0;j<libraries.size();j++){
	    	final Library library = (Library)libraries.get(j);
	    	MenuItem miLib = new MenuItem(menu,SWT.CASCADE);
	    	miLib.setText(library.getName());
	    	miLib.setImage(SharedImages.IMG_LIBRARY);
	    	Menu menuLib = new Menu(toolBar.getShell(),SWT.DROP_DOWN);
	    	miLib.setMenu(menuLib);

		    String[] computations = library.listRules();
		    for(int i=0; i<computations.length;i++){
		    	final String computation = computations[i];
			    MenuItem menuItem = new MenuItem(menuLib, SWT.PUSH);
			    menuItem.setText(computation);
			    menuItem.setImage(SharedImages.IMG_RULE);
			    menuItem.addSelectionListener(new SelectionListener(){
						public void widgetDefaultSelected(SelectionEvent e) {}
						public void widgetSelected(SelectionEvent e) {
							updateComputation(library,computation);
						}
			    });
		    }
	    }
	    menu.setLocation(pt.x, pt.y);
	    return menu;
		}
		void updateComputation(Library library,String computation){
			PrenzlPlugin.getLaunchModel().setRuleDescriptor(new RuleDescriptor(library,computation));
			PrenzlPlugin.getLaunchModel().notifyObservers();
		}
	}
	

}
