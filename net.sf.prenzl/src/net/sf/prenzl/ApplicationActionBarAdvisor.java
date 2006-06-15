package net.sf.prenzl;

import net.sf.prenzl.ui.toolbars.launch.ComputationContributionItem;
import net.sf.prenzl.ui.toolbars.launch.PictureContributionItem;

import org.eclipse.jface.action.ICoolBarManager;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.action.ToolBarContributionItem;
import org.eclipse.jface.action.ToolBarManager;
import org.eclipse.swt.SWT;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.actions.ActionFactory;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;
import org.eclipse.ui.application.ActionBarAdvisor;
import org.eclipse.ui.application.IActionBarConfigurer;

/**
 * An action bar advisor is responsible for creating, adding, and disposing of
 * the actions added to a workbench window. Each window will be populated with
 * new actions.
 */
public class ApplicationActionBarAdvisor extends ActionBarAdvisor {

	// Actions - important to allocate these only in makeActions, and then use
	// them
	// in the fill methods. This ensures that the actions aren't recreated
	// when fillActionBars is called with FILL_PROXY.
	private IWorkbenchAction exitAction;
	
	public ApplicationActionBarAdvisor(IActionBarConfigurer configurer) {
		super(configurer);
	}

	protected void makeActions(final IWorkbenchWindow window) {
		// Creates the actions and registers them.
		// Registering is needed to ensure that key bindings work.
		// The corresponding commands keybindings are defined in the plugin.xml
		// file.
		// Registering also provides automatic disposal of the actions when
		// the window is closed.

		exitAction = ActionFactory.QUIT.create(window);
		register(exitAction);
		
	}

	protected void fillMenuBar(IMenuManager menuBar) {
//		MenuManager fileMenu = new MenuManager("&File",IWorkbenchActionConstants.M_FILE);
//		menuBar.add(fileMenu);
//		fileMenu.add(exitAction);
//		
//		menuBar.add(new MenuManager("Edit"));
//		menuBar.add(new MenuManager("View"));
//		menuBar.add(new MenuManager("Configurations"));
//		menuBar.add(new MenuManager("Movie"));
//		menuBar.add(new MenuManager("?"));
	}

    protected void fillCoolBar(ICoolBarManager coolBarManager) {
    	//coolBarManager.add(new PictureContributionItem());
    	
        IToolBarManager toolBarManager = new ToolBarManager(SWT.FLAT | SWT.RIGHT);
        coolBarManager.add(new ToolBarContributionItem(toolBarManager, "main"));
        coolBarManager.setLockLayout(false);
//        toolBarManager.add(runAction);
//        toolBarManager.add(previousStepAction);
//        toolBarManager.add(nextStepAction);
        toolBarManager.add(new ComputationContributionItem());
        toolBarManager.add(new PictureContributionItem());
        //toolBarManager.add(new CountContributionItem());
        
     }

}
