package net.sf.prenzl;

import net.sf.prenzl.ui.views.Display2dView;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

public class Perspective implements IPerspectiveFactory {
	public static final String ID = PrenzlPlugin.ID+".runperspective";


	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		
		layout.addStandaloneView(Display2dView.ID,  false, IPageLayout.LEFT, 1.0f, editorArea);
	}

}
