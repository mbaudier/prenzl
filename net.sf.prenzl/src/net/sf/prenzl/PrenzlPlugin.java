package net.sf.prenzl;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.List;
import java.util.Vector;

import net.sf.prenzl.adapter.Computation;
import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.launch.ICountListener;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.ui.views.Display2dView;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.IExtensionPoint;
import org.eclipse.core.runtime.IExtensionRegistry;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.ui.IViewPart;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

/**
 * The main plugin class to be used in the desktop.
 */
public class PrenzlPlugin extends AbstractUIPlugin {
	// Constants
	public final static String ID = "net.sf.prenzl";
	public final static String ID_LIBRARY = ID+".library";

	public final static String STATE_LAST_PICFILES = "prenzl-lastpictfiles";
	
	//The shared instance.
	private static PrenzlPlugin plugin;
	
	private LaunchModel launchModel;

	private State state;

	public PrenzlPlugin() {
		plugin = this;
	}

	public static LaunchModel getLaunchModel(){
		return getDefault().launchModel;
	}

	public void start(BundleContext context) throws Exception {
		super.start(context);

		Computation.initRegistries();
		LaunchModel.setLibraries(loadLibraries());
		
		launchModel = new LaunchModel();

		//restorePreviousState();
		launchModel.addLastInputLocations(loadLastPicFilesState());
		launchModel.notifyObservers();
	}
	

  private List loadLibraries() {
		Vector libraries = new Vector();
		IExtensionRegistry registry = Platform.getExtensionRegistry();
		IExtensionPoint extensionPoint = registry.getExtensionPoint(ID_LIBRARY);
		if (extensionPoint != null) {
			IConfigurationElement points[] = extensionPoint.getConfigurationElements();
			for (int i = 0; i < points.length; i++) {
				try {
					Library library = (Library) points[i].createExecutableExtension("class");
					libraries.add(library);
				}
				catch (CoreException e) {
					e.printStackTrace();
				}
			}
		}
		return libraries;
	}

	/**
	 * This method is called when the plug-in is stopped
	 */
	public void stop(BundleContext context) throws Exception {
		saveLastPicFilesState();
		//savePreviousState();
		Computation.cleanRegistries();

		super.stop(context);
		plugin = null;
	}

	private List loadLastPicFilesState(){
		List list = new Vector();
		try {
			File f = new File(getStateLocation()+STATE_LAST_PICFILES);		
			f.createNewFile();
			BufferedReader in = new BufferedReader(new FileReader(f));
			String line = null;
			while((line=in.readLine())!=null){
				list.add(line);
			}
			in.close();
		}
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		return list;
	}
	
	private void saveLastPicFilesState(){
		List list = launchModel.getLastInputLocations();
		try {
			File f = new File(getStateLocation()+STATE_LAST_PICFILES);		
			f.createNewFile();
			Writer out = new FileWriter(f);
			for(int i=0;i<list.size();i++){
				out.write((String)list.get(i));
				out.write("\n");
			}
			out.close();
		}
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	/**
	 * Returns the shared instance.
	 */
	public static PrenzlPlugin getDefault() {
		return plugin;
	}

	public static IWorkbench getDefaultWorkbench() {
		return plugin.getWorkbench();
	}

	/**
	 * Returns an image descriptor for the image file at the given
	 * plug-in relative path.
	 *
	 * @param path the path
	 * @return the image descriptor
	 */
	public static ImageDescriptor getImageDescriptor(String path) {
		return AbstractUIPlugin.imageDescriptorFromPlugin(ID, path);
	}
	
	public static IViewPart findView(String id){
		return getDefault().getWorkbench().getActiveWorkbenchWindow().getActivePage().findView(id);
	}

	public static Display2dView findRunView(){
		return (Display2dView)getDefault().getWorkbench().getActiveWorkbenchWindow().getActivePage().findView(Display2dView.ID);
	}

	
	public static void removeCountListener(ICountListener countListener){
		findRunView().getComputationUI().removeCountListener(countListener);
	}

	public State getPreviousState() {
		return state;
	}

}
