package net.sf.prenzl.ui.computation;

import java.util.Observable;
import java.util.Vector;

import net.sf.prenzl.adapter.Computation;
import net.sf.prenzl.launch.ComputationInput;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.ICountListener;
import net.sf.prenzl.util.ImageUtil;
import net.sf.prenzl.util.Log;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class ComputationUI extends Observable{
	public static final int DISPLAY_ORIGINAL = 0;
	public static final int DISPLAY_FIT = 1;
	public static final int DISPLAY_STRECH = 2;
	
	private Vector countListeners = new Vector();
	
	// Rule
	private Configuration configuration;
	private Computation computation;
	private RunnerThread runnerThread;

	// Data
	private ImageData imageData = null;
	private ImageData imageDataScaled = null;

	// Graphics and SWT
	private Composite parent;
	private DrawingContext dc;
	private Shell fullScreen = null;
	private boolean isFullScreen = false;
	private Point drawOrigin = null;
	private int displayMode = DISPLAY_ORIGINAL;
	
	private final MouseListener mouseListener;
	// History
	private final static int MAX_HISTORY_MEMORY = 100 * 1024 * 1024;// 100 MB
	private byte[][] history = null;
	private byte[] firstGeneration = null;
	
	private int cycleCount = 0;

	public ComputationUI(Composite parent, MouseListener mouseListener) {
		this.parent = parent;
		this.mouseListener = mouseListener;
		
		drawOrigin = new Point(0, 0);
		dc = new DrawingContext();
		resetDrawingContext(false);
	}

	private void drawImage() {
		if(dc.getLabel()==null)return;
		dc.getDisplay().syncExec(new Runnable(){
			public void run(){
				if (imageData != null) {
					int displayWidth = dc.getLabel().getParent().getBounds().width;
					int displayHeight = dc.getLabel().getParent().getBounds().height;

					if(displayMode == DISPLAY_ORIGINAL){
						imageDataScaled = imageData;
					}
					else if(displayMode == DISPLAY_FIT){
						if(displayWidth!=0 && displayHeight!=0) {
							imageDataScaled = ImageUtil.fit(imageData,displayWidth,displayHeight);
						}
						else{
							imageDataScaled = imageData;
						}
					}
					else if(displayMode == DISPLAY_STRECH){
						if(displayWidth!=0 && displayHeight!=0) {
							imageDataScaled = ImageUtil.strech(imageData,displayWidth,displayHeight);
						}
						else{
							imageDataScaled = imageData;
						}
					}
					else{
						throw new RuntimeException("Unknown display mode "+displayMode);
					}
					
					// Centers the label
					int locX = 0;
					int locY = 0;
					if(displayWidth!=0 && displayHeight!=0){
						if (displayWidth > imageDataScaled.width) {
							locX = (displayWidth - imageDataScaled.width) / 2;
						}
						if (displayHeight > imageDataScaled.height) {
							locY = (displayHeight - imageDataScaled.height) / 2;
						}
					}
					dc.getLabel().setLocation(locX, locY);

					
					// Draws the image
					Image imageT = new Image(dc.getDisplay(), imageDataScaled);
					int width = imageDataScaled.width;
					int height = imageDataScaled.height;

					int x = drawOrigin.x % width;
					if (x < 0)
						x = width + x;
					int y = drawOrigin.y % height;
					if (y < 0)
						y = height + y;

					synchronized (dc) {
						GC gc = dc.getGC();
						gc.setClipping(0, 0, width, height);
						gc.drawImage(imageT, x - width, y - height);// upper left
						gc.drawImage(imageT, x, y - height);// upper right
						gc.drawImage(imageT, x - width, y);// lower left
						gc.drawImage(imageT, x, y);// lower right
					}

					imageT.dispose();
				}
				
			}
		});
	}
/*
	private void center(Rectangle bounds){
		int locX = 0;
		int locY = 0;
		if(bounds!=null && imageData!=null){
			if (bounds.width > imageData.width) {
				locX = (bounds.width - imageData.width) / 2;
			}
			if (bounds.height > imageData.height) {
				locY = (bounds.height - imageData.height) / 2;
			}
		}
		dc.getLabel().setLocation(locX, locY);
	}
*/	
	public void setFocus() {
		dc.getLabel().setFocus();
	}

	private void paintBackground() {
		// Paints background
		GC gc = dc.getGC();
		gc.setForeground(dc.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		gc.fillRectangle(0, 0, dc.getDisplay().getClientArea().width,
				dc.getDisplay().getClientArea().height);
	}

	private synchronized void resetThreadImpl(){
		boolean wasRunning = isRunning();
		setRunning(false);
		if (runnerThread != null) {
			runnerThread.dispose();
		}
		// First clean previous state
		if (computation != null) {
			computation.cleanUp();
			computation = null;
		}

		runnerThread = new RunnerThread();
		runnerThread.start();
		if(wasRunning)setRunning(true);
	}

	/** @param computationInput should not be null*/ 
	private synchronized void resetImpl(ImageData data) {
		imageData = data;
		// Initializes history
		int dataLength = imageData.data.length;
		history = new byte[MAX_HISTORY_MEMORY / dataLength][];
		Log.verbose("Computation history size is "+history.length);
		
		// Clones first generation (history[0] will be empty for the first generations)
		firstGeneration = new byte[imageData.data.length];
		for(int i=0;i<imageData.data.length;i++){
			firstGeneration[i] = imageData.data[i];
		}
		// Initializes history array
		for(int i=0; i<history.length; i++){
			history[i] = new byte[imageData.data.length];
		}
		
		drawOrigin = new Point(0, 0);
		//center(dc.getLabel().getParent().getBounds());
		drawImage();

		cycleCount = 0;
	}
	
	public synchronized void reset(Configuration configuration) {
		this.configuration = configuration;
		resetThreadImpl();
		setChanged();
		notifyObservers();
	}
	
	public synchronized void reset(ComputationInput computationInput) {
		paintBackground();
		resetImpl(computationInput.getData());		
		notifyCycleCount(cycleCount);
		resetThreadImpl();		
		setChanged();
		notifyObservers();
	}
	public synchronized void resetFirstGeneration() {
		if(firstGeneration!=null){
			ImageData clone = (ImageData)imageData.clone();
			clone.data = firstGeneration;
			reset(new ComputationInput(clone));
		}
	}
	
	public synchronized void reset(Configuration configuration, ComputationInput computationInput) {
		this.configuration = configuration;		
		paintBackground();
		if(computationInput!=null){
			resetImpl(computationInput.getData());		
			notifyCycleCount(cycleCount);
			resetThreadImpl();		
			setChanged();
			notifyObservers();
		}
	}
	
	public void addCountListener(ICountListener countListener){
		countListeners.add(countListener);
	}
	
	public void removeCountListener(ICountListener countListener){
		countListeners.remove(countListener);
	}
	
	private void notifyCycleCount(int count){
		for(int i=0;i<countListeners.size();i++){
			((ICountListener)countListeners.get(i)).setCount(count);
		}
	}

/*
	public void setFullScreen(boolean doFullScreen){
		resetDrawingContext(doFullScreen);
	}
	*/
	private void resetDrawingContext(final boolean doFullScreen) {
//		dc.getDisplay().syncExec(new Runnable(){
//			public void run(){
//			}
//		});
		if (doFullScreen) {
			fullScreen = new Shell(dc.getLabel().getShell(), SWT.NO_TRIM);
			Rectangle dispBounds = dc.getDisplay().getBounds();
			fullScreen.setBounds(dispBounds);

			Label label = createDrawingLabel(fullScreen, fullScreen.getBounds());
			dc.setContext(label);
			//center(dispBounds);
			fullScreen.open();
		}
		else {
			Label label = createDrawingLabel(parent, parent.getClientArea());
			dc.setContext(label);
			if (fullScreen != null) {
				fullScreen.close();
				fullScreen.dispose();
				fullScreen = null;
			}
		}
	}

	public void dispose() {
		if (runnerThread != null) {
			runnerThread.dispose();
			synchronized (runnerThread) {
				try {
					runnerThread.wait(1000);
				}
				catch (InterruptedException e) {
				}
			}
		}
		if (computation != null)
			computation.cleanUp();
		if (dc != null)
			dc.dispose();
	}

	public void setRunning(boolean running) {
		if (runnerThread != null) {
			runnerThread.setRunning(running);
			setChanged();
			notifyObservers();
		}
	}

	private Label createDrawingLabel(Composite parent, Rectangle bounds) {
		Label label = new Label(parent, SWT.CENTER);
		parent.setBackground(parent.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		label.setBackground(label.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		label.addPaintListener(new PaintListener() {
			public void paintControl(PaintEvent e) {
				//center(dc.getLabel().getParent().getBounds());
				drawImage();
			}
		});
		LabelMouseListener lml = new LabelMouseListener();
		label.addMouseListener(lml);
		label.addMouseListener(mouseListener);
		label.addMouseMoveListener(lml);
		label.setBounds(bounds);
		return label;
	}
	
	private byte[] getData(){
		if(cycleCount<0){// should never happen (protected by GUI)
			throw new RuntimeException("Cycle count cannot be <0");
		}
		if(cycleCount==0){
			return firstGeneration;
		}
		else{
			return history[cycleCount%history.length];
		}
	}
	
	public void setToPreviousStep(){
		if(!runnerThread.isRunning()){
			cycleCount--;
			imageData.data = getData();
			drawImage();
			notifyCycleCount(cycleCount);
		}
	}
	
	public void setToNextStep(){
		if(!runnerThread.isRunning()){
			try {
				runnerThread.cycle();
			}
			catch (Exception e) {
				Log.error("Exception while running standalone cycle",e);
			}
		}
	}


	public boolean isRunning(){
		if(runnerThread==null)return false;
		return runnerThread.isRunning();
	}

	public int getCycleCount() {
		return cycleCount;
	}

	public DrawingContext getDc() {
		return dc;
	}

	public ImageData getImageDataScaled() {
		return imageDataScaled;
	}
	
	public synchronized int getDisplayMode() {
		return displayMode;
	}

	public synchronized void setDisplayMode(int displayMode) {
		this.displayMode = displayMode;
		paintBackground();
		drawImage();
		setChanged();
		notifyObservers();
	}

	private class RunnerThread extends ComputationThread {

		protected void computationStep() throws Exception {
			if(computation==null){
				computation = configuration.getLibrary().createComputation(
						configuration.getRuleName(), 
						imageData.width, imageData.height,
						imageData.data,null,
						configuration.getPropertiesAsString());
			}
			computation.compute();
		}

		protected void displayStep() throws Exception {
			cycleCount++;
			if(computation!=null){
				computation.getCurrent(getData());
				imageData.data = getData();
				
				if (!dc.getLabel().isDisposed()) {
					drawImage();
				}
			}
			notifyCycleCount(cycleCount);
		}

	}
	private class LabelMouseListener implements MouseMoveListener, MouseListener {
		private boolean isOrigMoveActive = false;
		private int relX = 0;
		private int relY = 0;

		public void mouseDoubleClick(MouseEvent e) {
			// interrupts the thread in order to prevent blocking
			boolean wasRunning = runnerThread.isRunning();
			if (wasRunning) runnerThread.setRunning(false);
			if (isFullScreen) {
				resetDrawingContext(false);
				isFullScreen = false;
			}
			else {
				resetDrawingContext(true);
				isFullScreen = true;
			}
			if (wasRunning)	runnerThread.setRunning(true);
		}

		public void mouseDown(MouseEvent e) {
			if (e.button == 1) {
				isOrigMoveActive = true;
				relX = drawOrigin.x - e.x;
				relY = drawOrigin.y - e.y;
				setOrigin(e);
			}
		}

		public void mouseUp(MouseEvent e) {
			if (e.button == 1) isOrigMoveActive = false;
		}

		public void mouseMove(MouseEvent e) {
			if (isOrigMoveActive) setOrigin(e);
		}

		private void setOrigin(MouseEvent e) {
			drawOrigin = new Point(e.x + relX, e.y + relY);
			if (!isRunning()) {
				drawImage();
			}
		}
	}
}
