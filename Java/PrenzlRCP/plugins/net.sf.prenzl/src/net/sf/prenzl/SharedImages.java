/* Created on 12 mars 2006 */
package net.sf.prenzl;

import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.graphics.Image;

public class SharedImages {
	public final static Image IMG_LIBRARY = 
		PrenzlPlugin.getImageDescriptor("icons/library.gif").createImage(); 
	public final static Image IMG_FILE = 
		PrenzlPlugin.getImageDescriptor("icons/file.gif").createImage(); 
	public final static Image IMG_RULE = 
		PrenzlPlugin.getImageDescriptor("icons/rule.gif").createImage(); 
	
	public final static ImageDescriptor IMGD_RUN = 
		PrenzlPlugin.getImageDescriptor("icons/run.gif"); 
	public final static ImageDescriptor IMGD_STOP = 
		PrenzlPlugin.getImageDescriptor("icons/stop.gif"); 
	public final static ImageDescriptor IMGD_BACKWARD = 
		PrenzlPlugin.getImageDescriptor("icons/backward.gif"); 
	public final static ImageDescriptor IMGD_FORWARD = 
		PrenzlPlugin.getImageDescriptor("icons/forward.gif"); 
}
