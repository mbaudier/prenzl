/* Created on 12 mars 2006 */
package net.sf.prenzl;

import java.util.Hashtable;

import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.program.Program;

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
	public final static ImageDescriptor IMGD_FIRST_GENERATION_ACTION = 
		PrenzlPlugin.getImageDescriptor("icons/firstGenAction.gif"); 
	public final static ImageDescriptor IMGD_BACKWARD = 
		PrenzlPlugin.getImageDescriptor("icons/backward.gif"); 
	public final static ImageDescriptor IMGD_FORWARD = 
		PrenzlPlugin.getImageDescriptor("icons/forward.gif"); 
	public final static ImageDescriptor IMGD_SAVE = 
		PrenzlPlugin.getImageDescriptor("icons/save.gif"); 
	public final static ImageDescriptor IMGD_OPEN = 
		PrenzlPlugin.getImageDescriptor("icons/open.gif"); 
	public final static ImageDescriptor IMGD_FILE = 
		PrenzlPlugin.getImageDescriptor("icons/file.gif"); 
	
	public final static Hashtable extensionImageDescriptors = new Hashtable();
	
	public final static ImageDescriptor getExtensionIMGD(String extension){
		if(!extensionImageDescriptors.containsKey(extension)){
			Program program = Program.findProgram(extension);
			final ImageDescriptor imgd;
			if(program!=null){
				imgd = ImageDescriptor.createFromImageData(program.getImageData());
			}
			else{
				imgd = IMGD_FILE;
			}
			extensionImageDescriptors.put(extension,imgd);
		}
		return (ImageDescriptor)extensionImageDescriptors.get(extension);
	}
}
