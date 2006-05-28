package net.sf.prenzl;

import org.eclipse.ui.application.IWorkbenchConfigurer;
import org.eclipse.ui.application.IWorkbenchWindowConfigurer;
import org.eclipse.ui.application.WorkbenchAdvisor;
import org.eclipse.ui.application.WorkbenchWindowAdvisor;

public class ApplicationWorkbenchAdvisor extends WorkbenchAdvisor {

	public WorkbenchWindowAdvisor createWorkbenchWindowAdvisor(
			IWorkbenchWindowConfigurer configurer) {
		return new ApplicationWorkbenchWindowAdvisor(configurer);
	}

	public String getInitialWindowPerspectiveId() {
		return Perspective.ID;
	}

	public void initialize(IWorkbenchConfigurer configurer) {
		super.initialize(configurer);
	}

	public void postStartup() {
		super.postStartup();
	}

	public void preStartup() {
		super.preStartup();
//		PrenzlPlugin.getDefault().preStartup();
	}

}
