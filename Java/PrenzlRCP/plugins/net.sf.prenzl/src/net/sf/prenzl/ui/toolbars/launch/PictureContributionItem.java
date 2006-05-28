/* Created on 30 janv. 2006 */
package net.sf.prenzl.ui.toolbars.launch;

import java.io.File;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;

import org.eclipse.jface.action.ContributionItem;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

public class PictureContributionItem extends ContributionItem {
	private final static int WIDTH = 100;
	
	private Menu menuLastFiles = null;

	public PictureContributionItem(){
	}

	
	public void fill(final ToolBar parent, int index) {
		final ToolItem toolItem = new ToolItem(parent,SWT.DROP_DOWN);
    List lastPictures = PrenzlPlugin.getLaunchModel().getLastPictures();
		toolItem.setText(lastPictures.size()>0?new File(lastPictures.get(0).toString()).getName()
				:"<click to choose file>");
		toolItem.setImage(SharedImages.IMG_FILE);
		toolItem.setToolTipText("Click to choose a picture");
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
		}
		
		public void update(Observable o, Object arg) {
			if(o==PrenzlPlugin.getLaunchModel()){
				String path = PrenzlPlugin.getLaunchModel().getPicturePath();
				if(path!=null){					
					toolItem.setText(new File(path).getName());
					toolItem.setToolTipText(path);
					toolItem.setWidth(WIDTH);
				}
			}
		}

		public void handleEvent(Event event) {
			Rectangle rect = toolItem.getBounds();
	    if (event.detail == SWT.ARROW) {
	      Point pt = new Point(rect.x, rect.y + rect.height);
	      if(menuLastFiles==null){
		      menuLastFiles = createPicturesListMenu(pt);
		      menuLastFiles.setVisible(true);
	      }
	      else{
	      	menuLastFiles.setVisible(false);
	      	menuLastFiles = null;
	      }
	    }
	    else{
	    	FileDialog fileDialog = new FileDialog(toolBar.getShell(),SWT.OPEN);
	    	final String path;
	    	if((path = fileDialog.open())!=null){
	    		updatePicturePath(path);
	    	}
	    	else{
	    		// TODO: deal with exceptions
	    	}
	    		
	    	
	    }
		}
				
		private Menu createPicturesListMenu(Point pt){
	    pt = toolBar.toDisplay(pt);
	    Menu menu = new Menu(toolBar.getShell(), SWT.POP_UP);
	    List lastPictures = PrenzlPlugin.getLaunchModel().getLastPictures();
	    for(int i=0; i<lastPictures.size();i++){
		    MenuItem menuItem = new MenuItem(menu, SWT.PUSH);
		    final String path = lastPictures.get(i).toString();
		    menuItem.setText(path);
		    menuItem.setImage(SharedImages.IMG_FILE);
		    menuItem.addSelectionListener(new SelectionListener(){

					public void widgetDefaultSelected(SelectionEvent e) {}

					public void widgetSelected(SelectionEvent e) {
		    		updatePicturePath(path);
					}
		    });
	    }
	    menu.setLocation(pt.x, pt.y);
	    return menu;
		}
		void updatePicturePath(String path){
			PrenzlPlugin.getLaunchModel().setPicturePath(path);
			PrenzlPlugin.getLaunchModel().notifyObservers();
		}
		
	}
	

}
