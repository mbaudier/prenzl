/* Created on 30 janv. 2006 */
package net.sf.prenzl.launch;

import java.util.List;
import java.util.Observable;
import java.util.Vector;

import org.eclipse.swt.graphics.Image;

import net.sf.prenzl.adapter.Library;

public class LaunchModel extends Observable{
	
	private int lastPicturesMaxSize = 15;
	
	private ComputationInput input;
//	private RuleDescriptor ruleDescriptor;
//	private RuleProperties ruleProperties;

	private Configuration configuration;
	/** List of Configuration*/
	private final Vector lastConfigurations;
	/** List of ComputationInput*/
	private final Vector lastInputs;
	/** List of Library*/
	private static List libraries;
	
	public LaunchModel(){
		lastInputs = new Vector();
		lastConfigurations = new Vector();
	}
	
	public synchronized boolean isReadyToLaunch(){
		return lastConfigurations.size()!=0 && input!=null;
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

	public synchronized void setComputationInput(ComputationInput input) {
		this.input = input;
		if(lastInputs.contains(input)){
			lastInputs.remove(input);
		}
		lastInputs.insertElementAt(input,0);
		if(lastInputs.size()>lastPicturesMaxSize){
			lastInputs.remove(lastInputs.size()-1);
		}
		
		setChanged();
		notifyObservers(input);
	}

	
	/** @param locationList list of strings*/
	public synchronized void addLastInputLocations(List locationList){
		for(int i = locationList.size()-1;i>=0;i--){
			setComputationInput(new ComputationInput((String)locationList.get(i)));
		}
		if(!lastInputs.isEmpty()){
			input = ((ComputationInput)lastInputs.get(0));
		}
		setChanged();
	}

	/** @return list of strings */
	public synchronized List getLastInputLocations() {
		Vector res = new Vector();
		for(int i=0;i<lastInputs.size();i++){
			res.add(((ComputationInput)lastInputs.get(i)).getLocation());
		}
		return res;
	}
	
	
	/** @return list of Configuration */
	public synchronized List getLastConfigurations() {
//		Vector res = new Vector();
//		for(int i=0;i<lastConfigurations.size();i++){
//			res.add(((Configuration)lastConfigurations.get(i)));
//		}
//		return res;
		return lastConfigurations;
	}
	

	public synchronized ComputationInput getComputationInput() {
		return input;
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
	
	public static synchronized void setLibraries(List libraries) {
		LaunchModel.libraries = libraries;
	}
	public static List getLibraries() {
		return libraries;
	}


}
