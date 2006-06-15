package net.sf.prenzl.ui.actions;

import java.io.FileInputStream;
import java.io.InputStream;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.util.Log;

import org.eclipse.jface.action.Action;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;

public class LoadConfigurationAction extends Action{
	
	public LoadConfigurationAction() {
		setImageDescriptor(SharedImages.IMGD_OPEN);
	}

	public void run() {
		FileDialog fileDialog = new FileDialog(PrenzlPlugin.getDefault().getWorkbench().getActiveWorkbenchWindow().getShell(), SWT.OPEN);
		fileDialog.setFilterExtensions(new String[]{"*.properties"});
		final String path;
		if ((path = fileDialog.open()) != null) {
			try {
				InputStream in = new FileInputStream(path);		
				Configuration configuration = new Configuration(in);
				PrenzlPlugin.getLaunchModel().setConfiguration(configuration);
				in.close();
			}
			catch (Exception e1) {
				Log.error("Could not save file " + path, e1);
			}
		}
	}

}