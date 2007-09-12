package net.sf.prenzl.minimal;

import java.io.InputStream;
import java.net.URL;
import java.util.List;
import java.util.StringTokenizer;
import java.util.Vector;

import net.sf.prenzl.adapter.Computation;
import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.launch.ComputationInput;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.ui.computation.ComputationUI;
import net.sf.prenzl.util.ImageUtil;

import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

public class Minimal {//implements MouseListener{
/*	public final static String PROP_LIBRARIES = "prenzl.libraries";
	public final static String PROP_CONFIGURATION_URL = "prenzl.configuration.url";
	public final static String PROP_INPUT_URL = "prenzl.input.url";

	private LaunchModel launchModel;
	
	private final ComputationUI computationUI;
	private final Display display;
	
	public Minimal(Display display, Shell shell) throws Exception{
		this.display = display;
		shell.setText("Prenzl!!");
		
		Rectangle bounds = display.getBounds();
		Computation.initRegistries();
		launchModel = new LaunchModel();

		launchModel.setLibraries(loadLibraries());

//		launchModel.setLoadMode(LaunchModel.LOADMODE_FIT_TO_SCREEN);
//		launchModel.setRuleDescriptor(new RuleDescriptor(LaunchModel
//				.findLibrary("stdrules"), "Phase Cheulou"));
//		launchModel
//				.setInputPath("C:/Documents and Settings/mbaudier/My Documents/Perso/Photos/060704-FreePartyBatroun/FILM1/QD0011001.jpg");

		launchModel.setConfiguration(loadConfiguration());
		
		computationUI = new ComputationUI(shell, this);
		computationUI.reset(launchModel.getConfiguration());
		computationUI.reset(loadComputationInput(bounds));
	}
	
	private List loadLibraries(){
		Vector libraries = new Vector();
		StringTokenizer st = new StringTokenizer(System.getProperty(PROP_LIBRARIES,"net.sf.prenzl.stdrules.StdRules"),",");
		while(st.hasMoreTokens()){
			try {
				Class clss = Class.forName(st.nextToken());
				libraries.add((Library)clss.newInstance());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return libraries;
	}
	
	private Configuration loadConfiguration() throws Exception{
		URL url = new URL(System.getProperty(PROP_CONFIGURATION_URL));
		InputStream in = url.openStream();
		Configuration configuration = new Configuration(in);
		in.close();
		return configuration;
	}
	private ComputationInput loadComputationInput(Rectangle bounds) throws Exception{
		URL url = new URL(System.getProperty(PROP_INPUT_URL));
		InputStream in = url.openStream();
		ImageData imageData = new ImageData(in);
		imageData = ImageUtil.fit(imageData,bounds.width,bounds.height);
		in.close();
		return new ComputationInput(imageData);
	}
	
	public void start() {
		start(true);
	}
	public void start(boolean fullScreen) {
		if(fullScreen){
			display.syncExec(new Runnable() {
				public void run() {
					computationUI.setFullScreen(true);
				}
			});
		}
		computationUI.setRunning(true);
	}

	public void dispose(){
	      computationUI.setRunning(false);
	      computationUI.dispose();
	      Computation.cleanRegistries();
	}
	
	public void mouseDoubleClick(MouseEvent e) {}

	public void mouseDown(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseUp(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public static void main(String[] args) {
		try {
			checkProperty(PROP_CONFIGURATION_URL);
			checkProperty(PROP_INPUT_URL);
			if(!System.getProperties().contains(PROP_LIBRARIES)){
				System.setProperty(PROP_LIBRARIES,"net.sf.prenzl.stdrules.StdRules");
			}

			Display display = new Display();
			Shell shell = new Shell(display);

			Minimal minimal = new Minimal(display, shell);
			shell.open();
			minimal.start();

			while (!shell.isDisposed()) {
				if (!display.readAndDispatch())
					display.sleep();
			}
			minimal.dispose();
			display.dispose();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void checkProperty(String propertyKey) throws Exception{
		if(!System.getProperties().containsKey(propertyKey)){
			throw new Exception("System property '"+propertyKey+"' has to be set.");
		}
	}
*/
}
