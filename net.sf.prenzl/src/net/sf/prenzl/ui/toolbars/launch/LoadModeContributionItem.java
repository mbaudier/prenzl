/* Created on 30 janv. 2006 */
package net.sf.prenzl.ui.toolbars.launch;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.launch.LaunchModel;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

public class LoadModeContributionItem extends ContributionItem implements Listener{

	private final static int WIDTH = 100;

	private ToolItem toolItem;

	public void fill(final ToolBar parent, int index) {
		toolItem = new ToolItem(parent, SWT.DROP_DOWN);

		toolItem.setText("Load mode");
		toolItem.setToolTipText("Transformation to apply on the loaded picture");
		toolItem.setWidth(WIDTH);

		toolItem.addListener(SWT.Selection, this);
		super.fill(parent, index);
	}

	public void handleEvent(Event event) {
		if(event.detail == SWT.ARROW){
			Rectangle rect = toolItem.getBounds();
			Point pt = new Point(rect.x, rect.y + rect.height);
			Menu menu = createLoadModesMenu(pt);
			menu.setVisible(true);
		}
	}

	private Menu createLoadModesMenu(Point pt) {
		pt = toolItem.getParent().toDisplay(pt);

		Menu menu = new Menu(toolItem.getParent().getShell(), SWT.POP_UP);
		createMenuItem(menu, LaunchModel.LOADMODE_ORIGINAL);
		createMenuItem(menu, LaunchModel.LOADMODE_FIT_TO_SCREEN);
		createMenuItem(menu, LaunchModel.LOADMODE_STRECH_TO_SCREEN);
		createMenuItem(menu, LaunchModel.LOADMODE_FIT_TO_800_600);
		createMenuItem(menu, LaunchModel.LOADMODE_STRECH_TO_800_600);
		createMenuItem(menu, LaunchModel.LOADMODE_FIT_TO_756_576);
		menu.setLocation(pt.x, pt.y);
		return menu;
	}

	private void createMenuItem(Menu menu, final int loadModeArg) {
		MenuItem menuItem = new MenuItem(menu, SWT.CHECK);
		menuItem.setText(LaunchModel.getLoadModeName(loadModeArg));
		int loadMode = PrenzlPlugin.getLaunchModel().getLoadMode();
		menuItem.setSelection(loadMode == loadModeArg);
		menuItem.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				PrenzlPlugin.getLaunchModel().setLoadMode(loadModeArg);
			}
		});
	}

}
