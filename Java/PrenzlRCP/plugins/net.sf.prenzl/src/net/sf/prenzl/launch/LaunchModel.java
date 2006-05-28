/* Created on 30 janv. 2006 */
package net.sf.prenzl.launch;

import java.util.List;
import java.util.Observable;
import java.util.Vector;

import net.sf.prenzl.adapter.Library;

public class LaunchModel extends Observable{
	
	private int lastPicturesMaxSize = 15;
	
	private String picturePath = null;
	private Library library = null;
	private String ruleName = null;

	private final Vector lastPictures;
	private final Vector libraries;
	
	public LaunchModel(List libraries){
		lastPictures = new Vector();
		this.libraries = new Vector(libraries);
	}
	
	public synchronized boolean isReadyToLaunch(){
		return library!=null && ruleName!=null && picturePath!=null;
	}
	
	public synchronized void addLastPictureFiles(List list){
		for(int i = list.size()-1;i>=0;i--){
			setPicturePath((String)list.get(i));
		}
		if(!lastPictures.isEmpty()){
			picturePath = (String)lastPictures.get(0);
		}
		setChanged();
	}

	public synchronized String getRuleName() {
		return ruleName;
	}

	public synchronized void setComputation(Library library,String ruleName) {
		this.library = library;
		this.ruleName = ruleName;
		setChanged();
		notifyObservers();
	}

	public synchronized Library getLibrary() {
		return library;
	}

	public synchronized String getPicturePath() {
		return picturePath;
	}

	public synchronized void setPicturePath(String picturePath) {
		this.picturePath = picturePath;
		if(lastPictures.contains(picturePath)){
			lastPictures.remove(picturePath);
		}
		lastPictures.insertElementAt(picturePath,0);
		if(lastPictures.size()>lastPicturesMaxSize){
			lastPictures.remove(lastPictures.size()-1);
		}
		
		setChanged();
		notifyObservers();
	}

	public synchronized List getLastPictures() {
		return new Vector(lastPictures);
	}
	
	public List getLibraries() {
		return libraries;
	}
}
