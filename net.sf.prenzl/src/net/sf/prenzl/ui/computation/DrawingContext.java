package net.sf.prenzl.ui.computation;

import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;

/** Synchronizes access to a Label and its GC (can be changed, e.g. when going full screen)*/
public class DrawingContext {
	private GC gc = null;
	private Label label = null;
	
	public synchronized void setContext(Label labelArg){
		if(this.gc!=null){this.gc.dispose();}// GC should be disposed before label
		if(this.label!=null){this.label.dispose();}
		this.label = labelArg;
		this.gc = new GC(label);
	}
	
	public synchronized GC getGC(){
		return gc;
	}
	
	public synchronized Label getLabel(){
		return label;
	}
	public synchronized Display getDisplay(){
		return label.getDisplay();
	}
	
	public synchronized void dispose(){
		if(!gc.isDisposed() && label!=null && !label.isDisposed()){
			gc.dispose();
		}
	}

}
