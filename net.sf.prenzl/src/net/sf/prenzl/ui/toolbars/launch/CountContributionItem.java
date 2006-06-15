/* Created on 30 janv. 2006 */
package net.sf.prenzl.ui.toolbars.launch;

import net.sf.prenzl.launch.ICountListener;
import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.jface.action.IStatusLineManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

public class CountContributionItem extends ContributionItem implements ICountListener{
	private ToolItem toolItem = null;
	private IStatusLineManager statusLineManager = null;

	private ComputationUI computationUI;
	
	public CountContributionItem(ComputationUI computationUI){
		this.computationUI = computationUI;
	}
	public CountContributionItem(ComputationUI computationUI, IStatusLineManager statusLineManager){
		this(computationUI);
		this.statusLineManager = statusLineManager;
	}
	
	public void fill(final ToolBar parent, int index) {
		toolItem = new ToolItem(parent,SWT.NONE | SWT.FLAT);
		toolItem.setText("0");
		toolItem.setWidth(30);
		computationUI.addCountListener(this);
		
		super.fill(parent, index);
	}
	
	
	
	public synchronized void setStatusLineManager(IStatusLineManager statusLineManager) {
		this.statusLineManager = statusLineManager;
	}

	public void setCount(final int count) {
		toolItem.getDisplay().asyncExec(new Runnable(){
			public void run(){
				if(!toolItem.isDisposed()){
					if(toolItem!=null)toolItem.setText(Long.toString(count));
					if(statusLineManager!=null)statusLineManager.setMessage(Integer.toString(count));
				}
			}
		});
	}
}
