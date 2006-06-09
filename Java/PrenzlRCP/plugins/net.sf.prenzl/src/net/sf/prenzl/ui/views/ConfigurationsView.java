package net.sf.prenzl.ui.views;

import java.util.List;
import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.SharedImages;
import net.sf.prenzl.launch.Configuration;
import net.sf.prenzl.launch.LaunchModel;
import net.sf.prenzl.ui.actions.LoadConfigurationAction;
import net.sf.prenzl.ui.actions.SaveConfigurationAction;

import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.viewers.ICellModifier;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.ui.part.ViewPart;

public class ConfigurationsView extends ViewPart implements Observer{
	public static final String ID = PrenzlPlugin.ID+".configurationsView";
	
	private final static String[] COLPROPS = {"Name"}; 
//	private final static int COLINDEX_ICON = 0;
	private final static int COLINDEX_NAME = 0;
	
	private TableViewer tableViewer;
//	private TableColumn columnIcon;
	private TableColumn columnName;

	
	public ConfigurationsView(){
	}
	
	/** Creates the viewer and initialize it.*/
	public void createPartControl(Composite parent) {
		tableViewer = new TableViewer(parent,SWT.BORDER | SWT.FULL_SELECTION | SWT.V_SCROLL);
//		tableViewer.getTable().setHeaderVisible(true);
//		tableViewer.getTable().setLinesVisible(true);

//		columnIcon = new TableColumn(tableViewer.getTable(),SWT.LEFT);
//		columnIcon.setText(COLPROPS[COLINDEX_ICON]);
		columnName = new TableColumn(tableViewer.getTable(),SWT.LEFT);
		columnName.setText(COLPROPS[COLINDEX_NAME]);

		tableViewer.setColumnProperties(COLPROPS);
		tableViewer.setContentProvider(new IStructuredContentProvider(){
			public void dispose() {}
			public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {}
			public Object[] getElements(Object inputElement) {
				return ((List)inputElement).toArray();
			}			
		});
		ConfigurationValueManager valueManager = new ConfigurationValueManager();
		tableViewer.setLabelProvider(valueManager);
//		tableViewer.setCellEditors(new CellEditor[]{new TextCellEditor(tableViewer.getTable())});
//		tableViewer.setCellModifier(valueManager);

		tableViewer.addSelectionChangedListener(new ISelectionChangedListener(){
			public void selectionChanged(SelectionChangedEvent event) {
				Configuration configuration = (Configuration)((IStructuredSelection)event.getSelection()).getFirstElement();
				if(configuration!=null){
					PrenzlPlugin.getLaunchModel().setConfiguration(configuration);
				}
			}
		});
		
		SaveConfigurationAction saveConfigurationAction = new SaveConfigurationAction();
		LoadConfigurationAction loadConfigurationAction = new LoadConfigurationAction();
		IToolBarManager toolBarManager= getViewSite().getActionBars().getToolBarManager();
    toolBarManager.add(loadConfigurationAction);
    toolBarManager.add(saveConfigurationAction);

    update(null,PrenzlPlugin.getLaunchModel().getConfiguration());
		PrenzlPlugin.getLaunchModel().addObserver(this);
	}

	public void update(Observable o, Object arg) {
		if(o instanceof LaunchModel || o == null){
			if(arg instanceof List){
				ISelection selection = tableViewer.getSelection();
				tableViewer.setInput(PrenzlPlugin.getLaunchModel().getLastConfigurations());
				columnName.pack();
				tableViewer.refresh();
				tableViewer.setSelection(selection);
			}
		}
	}

	/** Passing the focus request to the viewer's control.*/
	public void setFocus() {
		tableViewer.getTable().setFocus();
	}

	public void dispose() {
		tableViewer.getTable().dispose();
		super.dispose();
	}
	
	private class ConfigurationValueManager extends LabelProvider implements ITableLabelProvider,ICellModifier{

		public String getColumnText(Object element, int columnIndex) {
			return getCellValue(element,COLPROPS[columnIndex]);
		}
		public Image getColumnImage(Object element, int columnIndex) {
			if(columnIndex==COLINDEX_NAME) {
				Image img = ((Configuration)element).getImage();
				return img!=null?img:SharedImages.IMG_RULE;
			}
			else return null;
		}

		public boolean canModify(Object element, String property) {
			return property.equals(COLPROPS[COLINDEX_NAME]);
		}

		public Object getValue(Object element, String property) {
			return getCellValue(element,property);
		}

		public void modify(Object element, String property, Object value) {
//			Configuration configuration = (Configuration)((TableItem)element).getData();
//			configuration.setName(value.toString());
//			tableViewer.refresh(configuration);
		}
		
		private String getCellValue(Object element, String colProp){
			if(colProp.equals(COLPROPS[COLINDEX_NAME])){
				return ((Configuration)element).getName();
			}
			return null;
		}
	}
	
}