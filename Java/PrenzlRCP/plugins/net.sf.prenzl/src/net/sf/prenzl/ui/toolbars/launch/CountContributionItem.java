/* Created on 30 janv. 2006 */
package net.sf.prenzl.ui.toolbars.launch;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.launch.ICountListener;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

public class CountContributionItem extends ContributionItem implements ICountListener{
	private ToolItem toolItem = null;

	public void fill(final ToolBar parent, int index) {
		toolItem = new ToolItem(parent,SWT.NONE | SWT.FLAT);
		toolItem.setText("0");
		toolItem.setWidth(30);
		
		PrenzlPlugin.getRunModel().registerCountListener(this);
		
		super.fill(parent, index);
	}
	
	
	
	public void setCount(long count) {
		toolItem.getDisplay().asyncExec(new Runnable(){
			public void run(){
				if(!toolItem.isDisposed()){
					toolItem.setText(Long.toString(PrenzlPlugin.getRunModel().getCount()));
				}
			}
		});
	}


/*
	public void update(Observable o, Object arg) {
		toolItem.getDisplay().asyncExec(new Runnable(){
			public void run(){
				if(!toolItem.isDisposed()){
					toolItem.setText(Long.toString(PrenzlPlugin.getRunModel().getCount()));
				}
			}
		});
	}
	*/
}
