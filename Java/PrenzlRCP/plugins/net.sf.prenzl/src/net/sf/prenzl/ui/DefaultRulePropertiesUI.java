package net.sf.prenzl.ui;

import java.util.Map;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.launch.RuleProperties;
import net.sf.prenzl.util.Log;

import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.ICellModifier;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerSorter;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;

public class DefaultRulePropertiesUI extends RulePropertiesUI {
	private final static String[] COLPROPS = {"Key","Value"}; 
	private final static int COLINDEX_KEY = 0;
	private final static int COLINDEX_VALUE = 1;
	
	private final TableViewer tableViewer;
	private final TableColumn columnKey;
	private final TableColumn columnValue;
	
	public DefaultRulePropertiesUI(Composite parent) {
		tableViewer = new TableViewer(parent,SWT.BORDER | SWT.FULL_SELECTION | SWT.V_SCROLL);
//		tableViewer.getTable().setHeaderVisible(true);
//		tableViewer.getTable().setLinesVisible(true);

		columnKey = new TableColumn(tableViewer.getTable(),SWT.LEFT);
		columnKey.setText(COLPROPS[COLINDEX_KEY]);
		columnValue = new TableColumn(tableViewer.getTable(),SWT.LEFT);
		columnValue.setText(COLPROPS[COLINDEX_VALUE]);

		tableViewer.setColumnProperties(COLPROPS);
		tableViewer.setContentProvider(new DRPContentProvider());
		DRPValueManager valueManager = new DRPValueManager();
		tableViewer.setLabelProvider(valueManager);
		tableViewer.setCellEditors(new CellEditor[]{null, new TextCellEditor(tableViewer.getTable())});
		tableViewer.setCellModifier(valueManager);
		
		tableViewer.setSorter(new ViewerSorter(){
			public int compare(Viewer viewer, Object e1, Object e2) {
				if(e1 instanceof Map.Entry && e2 instanceof Map.Entry){
					return ((Map.Entry)e1).getKey().toString().compareTo(
							((Map.Entry)e2).getKey().toString());
				}
				return 0;
			}			
		});
	}

	public void setProperties(RuleProperties properties) {
		tableViewer.setInput(properties);
		columnKey.pack();
		columnValue.pack();
	}

//	public RuleProperties getProperties() {
//		return (RuleProperties)tableViewer.getInput();
//	}

	public void dispose() {
		if(tableViewer!=null)tableViewer.getTable().dispose();
	}

	public void setFocus() {
		if(tableViewer!=null)tableViewer.getTable().setFocus();
	}

	private class DRPContentProvider implements IStructuredContentProvider{
		public void dispose() {}
		public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {}

		public Object[] getElements(Object inputElement) {
			return ((RuleProperties)inputElement).getOrderedEntries();
		}
		
	}
	private class DRPValueManager extends LabelProvider implements ITableLabelProvider,ICellModifier{

		public String getColumnText(Object element, int columnIndex) {
			return getCellValue(element,COLPROPS[columnIndex]).toString();
		}
		public Image getColumnImage(Object element, int columnIndex) {return null;}

		public boolean canModify(Object element, String property) {
			return property.equals(COLPROPS[COLINDEX_VALUE]);
		}

		public Object getValue(Object element, String property) {
			return getCellValue(element,property);
		}

		public void modify(Object element, String property, Object value) {
			Map.Entry entry = (Map.Entry)((TableItem)element).getData();
			Object oldValue = entry.getValue();
			if(Log.isTrace())Log.trace("key="+entry.getKey()+", value="+value);
			RuleProperties rp = new RuleProperties(PrenzlPlugin.getLaunchModel().getRuleProperties());
			rp.setProperty(entry.getKey(),value);
			tableViewer.refresh();
			columnValue.pack();
			if(!oldValue.equals(value)){
				PrenzlPlugin.getLaunchModel().setRuleProperties(rp);
			}
		}
		
		private Object getCellValue(Object element, String colProp){
			Map.Entry entry = (Map.Entry)element;
			if(colProp.equals(COLPROPS[COLINDEX_KEY])){
				return entry.getKey();
			}
			else if(colProp.equals(COLPROPS[COLINDEX_VALUE])){
				return entry.getValue();
			}
			throw new RuntimeException("Unknow column property "+colProp);
		}
	}
}
