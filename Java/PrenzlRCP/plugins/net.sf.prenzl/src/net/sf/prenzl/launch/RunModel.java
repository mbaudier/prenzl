/* Created on 1 févr. 2006 */
package net.sf.prenzl.launch;

import java.util.Observable;

public class RunModel extends Observable {
	private boolean running = false;
	private long count = 0;
	private ICountListener countListener = null;
	
	public long getCount() {
		return count;
	}
	
	public void setCount(long count) {
		this.count = count;
		if(countListener!=null) countListener.setCount(count);
	}
	
	public boolean isRunning() {
		return running;
	}
	public void setRunning(boolean running) {
		this.running = running;
		setChanged();
		notifyObservers();
	}
	
	public void registerCountListener(ICountListener countListener){
		this.countListener = countListener;
	}
}
