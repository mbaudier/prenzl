/* Created on 30 janv. 2006 */
package net.sf.prenzl.launch;

import java.util.List;
import java.util.Observable;
import java.util.Vector;

import net.sf.prenzl.adapter.Library;
import net.sf.prenzl.util.ImageUtil;

import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;

public class LaunchModel extends Observable{
	
	public final static int LOADMODE_ORIGINAL = 0;
	public final static int LOADMODE_FIT_TO_SCREEN = 1;
	public final static int LOADMODE_STRECH_TO_SCREEN = 2;
	public final static int LOADMODE_FIT_TO_800_600 = 3;
	public final static int LOADMODE_STRECH_TO_800_600 = 4;
	public final static int LOADMODE_FIT_TO_756_576 = 5;
	
	private int lastPicturesMaxSize = 15;
	
	private String inputPath;
	
	private Configuration configuration;
	/** List of Configuration*/
	private final Vector lastConfigurations;
	/** List of String (paths)*/
	private final Vector lastInputs;
	/** List of Library*/
	private static List libraries;
	/** Load mode */
	private int loadMode = LOADMODE_ORIGINAL;
	
	public LaunchModel(){
		lastInputs = new Vector();
		lastConfigurations = new Vector();
	}
	
	public synchronized void setLoadMode(int loadMode){
		this.loadMode = loadMode;
	}
	
	public synchronized void setRuleProperties(RuleProperties ruleProperties) {
		setConfiguration(new Configuration(
				getConfiguration().getRuleDescriptor(),ruleProperties));
	}
	
	public synchronized void setRuleDescriptor(RuleDescriptor ruleDescriptor) {
		setConfiguration(new Configuration(
				ruleDescriptor,ruleDescriptor.getDefaultProperties()));
	}

	public synchronized void setConfiguration(Configuration configuration) {
		this.configuration = configuration;
		setChanged();
		notifyObservers(configuration);
	}
	public synchronized void setConfigurationImage(Image image) {
		configuration.setImage(image);
		setChanged();
		notifyObservers(lastConfigurations);
	}
	
	public synchronized void notifyConfigurationLaunch() {
		if(!lastConfigurations.contains(configuration)){
			configuration = new Configuration(configuration.getRuleDescriptor(),configuration.getRuleProperties());
			lastConfigurations.insertElementAt(configuration, 0);
		}
		setChanged();
		notifyObservers(lastConfigurations);
	}

	public synchronized void setInputPath(String inputPath) {
		this.inputPath = inputPath;
		addToLastInputLocations(inputPath);
		setChanged();
		notifyObservers(inputPath);
	}

	public synchronized String getInputPath() {
		return inputPath;
	}

	
	private void addToLastInputLocations(String path){
		if(lastInputs.contains(path)){
			lastInputs.remove(path);
		}
		lastInputs.insertElementAt(path,0);
		if(lastInputs.size()>lastPicturesMaxSize){
			lastInputs.remove(lastInputs.size()-1);
		}
	}
	
	/** @param locationList list of strings*/
	public synchronized void addLastInputLocations(List locationList){
		for(int i = locationList.size()-1;i>0;i--){
			addToLastInputLocations((String)locationList.get(i));
		}
		if(!locationList.isEmpty()){
			setInputPath((String)locationList.get(0));
		}
		setChanged();
	}

	/** @return list of strings */
	public synchronized List getLastInputLocations() {
		return lastInputs;
	}
	
	
	/** @return list of Configuration */
	public synchronized List getLastConfigurations() {
		return lastConfigurations;
	}
	

	public synchronized int getLoadMode(){
		return loadMode;
	}
	
	public synchronized ComputationInput loadComputationInput(int displayWith, int displayHeight) {
		ImageData imageDataOrig = new ImageData(inputPath);
		final ImageData imageData;
		switch(loadMode){
		case LOADMODE_ORIGINAL: imageData = imageDataOrig;break;
		case LOADMODE_FIT_TO_SCREEN: imageData = ImageUtil.fit(imageDataOrig,displayWith,displayHeight);break;
		case LOADMODE_STRECH_TO_SCREEN: imageData = ImageUtil.strech(imageDataOrig,displayWith,displayHeight);break;
		case LOADMODE_FIT_TO_800_600: 
			if(imageDataOrig.width>imageDataOrig.height)imageData = ImageUtil.fit(imageDataOrig,800,600);
			else imageData = ImageUtil.fit(imageDataOrig,600,800);
			break;
		case LOADMODE_STRECH_TO_800_600: 
			if(imageDataOrig.width>imageDataOrig.height)imageData = ImageUtil.strech(imageDataOrig,800,600);
			else imageData = ImageUtil.strech(imageDataOrig,600,800);
			break;
		case LOADMODE_FIT_TO_756_576: 
			if(imageDataOrig.width>imageDataOrig.height)imageData = ImageUtil.fit(imageDataOrig,800,600);
			else imageData = ImageUtil.fit(imageDataOrig,576,756);
			break;
		default: imageData = imageDataOrig;break;
		}
		return new ComputationInput(inputPath,imageData);
	}

	public synchronized Configuration getConfiguration() {
		return configuration;
	}
	public synchronized RuleDescriptor getRuleDescriptor() {
		return getConfiguration().getRuleDescriptor();
	}

	public synchronized RuleProperties getRuleProperties() {
		return getConfiguration().getRuleProperties();
	}
	
	public static Library findLibrary(String name){
		for(int i=0; i<libraries.size();i++){
			Library library = (Library)libraries.get(i);
			if(library.getName().equals(name)){
				return library;
			}
		}
		return null;
	}
	
	public synchronized void setLibraries(List libraries) {
		LaunchModel.libraries = libraries;
	}
	public List getLibraries() {
		return libraries;
	}

	public static String getLoadModeName(int loadMode)
	{
		switch(loadMode){
		case LaunchModel.LOADMODE_ORIGINAL:
			return "Original";
		case LaunchModel.LOADMODE_FIT_TO_SCREEN:
			return "Fit to screen";
		case LaunchModel.LOADMODE_STRECH_TO_SCREEN:
			return "Strech to screen";
		case LaunchModel.LOADMODE_FIT_TO_800_600:
			return "Fit to 800x600";
		case LaunchModel.LOADMODE_STRECH_TO_800_600:
			return "Strech to 800x600";
		case LaunchModel.LOADMODE_FIT_TO_756_576:
			return "Fit to 756x576";
		default: return "Unknown";
		}
	}

}
