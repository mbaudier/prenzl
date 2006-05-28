package net.sf.prenzl.ui;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import net.sf.prenzl.Log;
import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.adapter.Computation;
import net.sf.prenzl.adapter.Library;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.ImageLoader;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Shell;

public class ComputationUI {
	private Computation computation;

	private RunnerThread runnerThread;

	private Composite parent;

	private DrawingContext dc;

	private ImageData imageData = null;

	private Shell fullScreen = null;

	private boolean isFullScreen = false;

	private Point drawOrigin = null;
	
	private final static int MAX_HISTORY_MEMORY = 100 * 1024 * 1024;// 100 MB
	private int historySize = 100;
	private int nextHistoryIndex = 0;
	private byte[][] history = null;

	public ComputationUI(Composite parent) {
		this.parent = parent;
		drawOrigin = new Point(0, 0);
		dc = new DrawingContext();
		resetDrawingContext(false);
	}

	private void drawImage() {
		if (imageData != null) {
			Image imageT = new Image(dc.getDisplay(), imageData);
			int width = imageData.width;
			int height = imageData.height;

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

	public void setFocus() {
		dc.getLabel().setFocus();
	}

	public void paintBackground() {
		// Paints background
		GC gc = dc.getGC();
		gc.setForeground(dc.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		gc.fillRectangle(0, 0, dc.getDisplay().getClientArea().width,
				dc.getDisplay().getClientArea().height);
	}

	public void reset(Library library, String ruleName, String firstGenPicPath) {
		paintBackground();

		// First clean previous state
		if (computation != null) {
			computation.cleanUp();
			computation = null;
		}

		if (firstGenPicPath != null && library != null) {
			try {
				imageData = new ImageData(firstGenPicPath);
				
				// Initializes history
				int dataLength = imageData.data.length;
				historySize = MAX_HISTORY_MEMORY / dataLength;
				Log.verbose("Computation history size is "+historySize);
				history = new byte[historySize][];
				history[0] = imageData.data;
				nextHistoryIndex = 0;
				for(int i=1; i<historySize; i++){
					history[i] = new byte[imageData.data.length];
				}
				
				computation = library.createComputation(ruleName, imageData.width, imageData.height,
						imageData.data);
				
				drawOrigin = new Point(0, 0);
				drawImage();

				Shell shell = parent.getShell();
				Rectangle shellOrigBounds = shell.getBounds();
				Rectangle parentBounds = parent.getClientArea();
				int dx = parentBounds.width>0?
						shellOrigBounds.width - dc.getLabel().getBounds().width
						:20;
				int dy = parentBounds.height>0?
						shellOrigBounds.height - dc.getLabel().getBounds().height
						:55;
				shell.setBounds(shellOrigBounds.x,shellOrigBounds.y,imageData.width+dx,imageData.height+dy);
				
				if (runnerThread != null) {
					runnerThread.dispose();
				}
				runnerThread = new RunnerThread();
				runnerThread.start();
			}
			catch (Exception e) {
				Log.error("Could not initialize computation",e);
			}
		}
	}

	private void resetDrawingContext(boolean doFullScreen) {
		if (doFullScreen) {
			fullScreen = new Shell(dc.getLabel().getShell(), SWT.NO_TRIM);
			Rectangle dispBounds = dc.getDisplay().getBounds();
			fullScreen.setBounds(dispBounds);

			Label label = createDrawingLabel(fullScreen, fullScreen.getBounds());
			int locX = 0;
			int locY = 0;
			if (dispBounds.width > imageData.width) {
				locX = (dispBounds.width - imageData.width) / 2;
			}
			if (dispBounds.height > imageData.height) {
				locY = (dispBounds.height - imageData.height) / 2;
			}
			label.setLocation(locX, locY);
			dc.setContext(label);
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
		}
	}

	private Label createDrawingLabel(Composite parent, Rectangle bounds) {
		Label label = new Label(parent, SWT.CENTER);
		parent.setBackground(parent.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		label.setBackground(label.getDisplay().getSystemColor(SWT.COLOR_BLACK));
		label.addPaintListener(new PaintListener() {
			public void paintControl(PaintEvent e) {
				drawImage();
			}
		});
		LabelMouseListener lml = new LabelMouseListener();
		label.addMouseListener(lml);
		label.addMouseMoveListener(lml);
		label.setBounds(bounds);
		return label;
	}
	
	public void setToPreviousStep(){
		if(!runnerThread.isRunning()){
			nextHistoryIndex = nextHistoryIndex!=0 ? nextHistoryIndex-1 : historySize-1;
			imageData.data = history[nextHistoryIndex-1>=0?nextHistoryIndex-1:historySize-1];
			drawImage();
			runnerThread.decrementCount();
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
	
	private class RunnerThread extends ComputationThread {

		protected void computationStep() throws Exception {
			computation.compute();
		}

		protected void displayStep() throws Exception {
			computation.getCurrent(history[nextHistoryIndex]);
			imageData.data = history[nextHistoryIndex];
			nextHistoryIndex++;
			if(nextHistoryIndex==historySize){
				nextHistoryIndex = 0;
			}
			
			if (!dc.getLabel().isDisposed()) {
				drawImage();
			}
		}

	}
	private class LabelMouseListener implements MouseMoveListener, MouseListener {
		private boolean isOrigMoveActive = false;

		private int relX = 0;

		private int relY = 0;

		public void mouseDoubleClick(MouseEvent e) {
			// interrupts the thread in order to prevent blocking
			boolean wasRunning = runnerThread.isRunning();
			if (wasRunning)
				runnerThread.setRunning(false);
			if (isFullScreen) {
				resetDrawingContext(false);
				isFullScreen = false;
			}
			else {
				resetDrawingContext(true);
				isFullScreen = true;
			}
			if (wasRunning)
				runnerThread.setRunning(true);
		}

		public void mouseDown(MouseEvent e) {
			if (e.button == 1) {
				isOrigMoveActive = true;
				relX = drawOrigin.x - e.x;
				relY = drawOrigin.y - e.y;
				setOrigin(e);
			}
			else if (e.button == 2 || e.button == 3) {
				if (!PrenzlPlugin.getRunModel().isRunning()) {
					Menu menu = createMenu(new Point(e.x, e.y));
					menu.setVisible(!menu.isVisible());
				}
			}
		}

		public void mouseUp(MouseEvent e) {
			if (e.button == 1)
				isOrigMoveActive = false;
		}

		public void mouseMove(MouseEvent e) {
			if (isOrigMoveActive)
				setOrigin(e);
		}

		private void setOrigin(MouseEvent e) {
			drawOrigin = new Point(e.x + relX, e.y + relY);
			if (!PrenzlPlugin.getRunModel().isRunning()) {
				drawImage();
			}
		}

		private Menu createMenu(Point ptArg) {
			Point pt = dc.getLabel().toDisplay(ptArg);
			Menu menu = new Menu(dc.getLabel().getShell(), SWT.POP_UP);
			createSaveMenuItem(menu, "Save display as JPEG...", true, SWT.IMAGE_JPEG);
			createSaveMenuItem(menu, "Save display as BMP...", true, SWT.IMAGE_BMP);
			new MenuItem(menu, SWT.SEPARATOR);
			createSaveMenuItem(menu, "Save image as JPEG...", false, SWT.IMAGE_JPEG);
			createSaveMenuItem(menu, "Save image as BMP...", false, SWT.IMAGE_BMP);
			menu.setLocation(pt.x, pt.y);
			return menu;
		}

		private void createSaveMenuItem(Menu menu, String text, final boolean saveDisplay,
				final int type) {
			MenuItem mi = new MenuItem(menu, SWT.PUSH);
			mi.setText(text);
			mi.addSelectionListener(new SelectionListener() {
				public void widgetDefaultSelected(SelectionEvent e) {
				}

				public void widgetSelected(SelectionEvent e) {
					saveAsPicture(saveDisplay, type);
				}
			});
		}

		private void saveAsPicture(boolean saveDisplay, final int type) {
			ImageData imgd = null;
			Label label = dc.getLabel();
			if (saveDisplay) {
				int width = Math.min(imageData.width, label.getBounds().width);
				int height = Math.min(imageData.height, label.getBounds().height);
				System.out.println("width=" + width + ", height=" + height);
				Image image = new Image(label.getDisplay(), width, height);
				dc.getGC().copyArea(image, 0, 0);
				imgd = image.getImageData();
			}
			else {
				imgd = imageData;
			}

			FileDialog fileDialog = new FileDialog(label.getShell(), SWT.SAVE);
			final String path;
			if ((path = fileDialog.open()) != null) {
				final ImageLoader imgl = new ImageLoader();
				ImageData[] arr = {imgd};
				imgl.data = arr;
				label.getDisplay().asyncExec(new Runnable() {
					public void run() {
						try {
							OutputStream out = new FileOutputStream(path);
							imgl.save(out, type);
							out.close();
						}
						catch (IOException e1) {
							Log.error("Could not save file " + path, e1);
						}
					}
				});
			}
		}

	}

}
