package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.ui.computation.ComputationUI;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;

public class DisplayModeCI extends ContributionItem{

	private final ComputationUI computationUI;
	public DisplayModeCI(ComputationUI computationUI){
		this.computationUI = computationUI;
	}

	public void fill(Menu menu, int index) {
		MenuItem parentI = new MenuItem(menu,SWT.CASCADE);
		parentI.setText("Display");
		Menu parent = new Menu(menu);
		parentI.setMenu(parent);
		new DisplayModeMenu(ComputationUI.DISPLAY_ORIGINAL,parent);
		new DisplayModeMenu(ComputationUI.DISPLAY_FIT,parent);
		new DisplayModeMenu(ComputationUI.DISPLAY_STRECH,parent);
	}

	
	private class DisplayModeMenu implements Observer{
		final MenuItem item;
		final int displayMode;
		
		DisplayModeMenu(final int displayMode, Menu menu){
			this.displayMode = displayMode;
			item = new MenuItem(menu, SWT.CHECK);
			String label;
			switch (displayMode) {
			case ComputationUI.DISPLAY_ORIGINAL:label = "Original";break;
			case ComputationUI.DISPLAY_FIT:label = "Fit";break;
			case ComputationUI.DISPLAY_STRECH:label = "Strech";break;
			default:label = "<null>";break;
			}
			item.setText(label);
			item.addSelectionListener(new SelectionListener(){
				public void widgetDefaultSelected(SelectionEvent e) {
				}

				public void widgetSelected(SelectionEvent e) {
					computationUI.setDisplayMode(displayMode);
				}
			});
			update(computationUI,null);
			computationUI.addObserver(this);
		}

		public void update(Observable o, Object arg) {
			if(o instanceof ComputationUI){
				if(item!=null)item.setSelection(computationUI.getDisplayMode()==displayMode);
			}
		}
	}
}
