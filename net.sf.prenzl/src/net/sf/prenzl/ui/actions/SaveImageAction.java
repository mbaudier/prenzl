package net.sf.prenzl.ui.actions;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.computation.ComputationUI;
import net.sf.prenzl.util.Log;

import org.eclipse.jface.action.Action;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.ImageLoader;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Label;

public class SaveImageAction extends Action implements Observer {
	private final boolean saveDisplay;
	private final int type;

	private final ComputationUI computationUI;
	
	public SaveImageAction(ComputationUI computationUI, boolean saveDisplay, int type) {
		this.computationUI = computationUI;
		this.saveDisplay = saveDisplay;
		this.type = type;
		setText("Save "+(saveDisplay?"display":"image")+" as "+typeExtension(type));
		setImageDescriptor(SharedImages.getExtensionIMGD(typeExtension(type)));
		computationUI.addObserver(this);
	}

	private static String typeExtension(int type){
		if(type == SWT.IMAGE_JPEG){
			return "jpg";
		}
		else if(type == SWT.IMAGE_BMP)
		{
			return "bmp";
		}
		else{
			throw new RuntimeException("Unknown image type "+type);
		}
	}
	
	public void run() {
		saveAsPicture();
	}

	public void update(Observable o, Object arg) {
		if(o == computationUI){
			setEnabled(!computationUI.isRunning());
		}
		
	}

	private void saveAsPicture() {
		ImageData imgd = null;
		Label label = computationUI.getDc().getLabel();
		if (saveDisplay) {
			int width = Math.min(computationUI.getImageData().width, label.getBounds().width);
			int height = Math.min(computationUI.getImageData().height, label.getBounds().height);
			System.out.println("width=" + width + ", height=" + height);
			Image image = new Image(label.getDisplay(), width, height);
			computationUI.getDc().getGC().copyArea(image, 0, 0);
			imgd = image.getImageData();
		}
		else {
			imgd = computationUI.getImageData();
		}

		FileDialog fileDialog = new FileDialog(label.getShell(), SWT.SAVE);
		fileDialog.setFilterExtensions(new String[]{"*."+typeExtension(type)});
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
