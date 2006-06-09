package net.sf.prenzl;

import net.sf.prenzl.ui.views.ConfigurationsView;
import net.sf.prenzl.ui.views.Display2dView;
import net.sf.prenzl.ui.views.PropertiesView;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

public class ConfigPerspective implements IPerspectiveFactory {
	public static final String ID = PrenzlPlugin.ID+".configperspective";


	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(false);
		
		layout.addStandaloneView(Display2dView.ID,  true, IPageLayout.RIGHT, 0.3f, editorArea);
		layout.addStandaloneView(ConfigurationsView.ID,  true, IPageLayout.RIGHT, 0.7f, Display2dView.ID);
		layout.addStandaloneView(PropertiesView.ID,  true, IPageLayout.BOTTOM, 0.6f, ConfigurationsView.ID);
	}

}
