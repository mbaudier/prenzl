package net.sf.prenzl.ui.actions;

import java.io.FileOutputStream;
import java.io.OutputStream;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.util.Log;

import org.eclipse.jface.action.Action;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;

public class SaveConfigurationAction extends Action{
	
	public SaveConfigurationAction() {
		setImageDescriptor(SharedImages.IMGD_SAVE);
	}

	public void run() {
		FileDialog fileDialog = new FileDialog(PrenzlPlugin.getDefault().getWorkbench().getActiveWorkbenchWindow().getShell(), SWT.SAVE);
		fileDialog.setFilterExtensions(new String[]{"*.properties"});
		final String path;
		if ((path = fileDialog.open()) != null) {
			try {
				OutputStream out = new FileOutputStream(path);			
				PrenzlPlugin.getLaunchModel().getConfiguration().store(out);
				out.close();
			}
			catch (Exception e1) {
				Log.error("Could not save file " + path, e1);
			}
		}
	}

}