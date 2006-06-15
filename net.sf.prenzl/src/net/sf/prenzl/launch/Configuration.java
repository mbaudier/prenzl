package net.sf.prenzl.launch;

import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;

import net.sf.prenzl.adapter.Library;

import org.eclipse.swt.graphics.Image;

public class Configuration {
	public final static SimpleDateFormat sdf = new SimpleDateFormat("hh:mm:ss, dd/MM/yy");
	
	public final static String PROP_PRENZL_RULE_NAME = "prenzl.rule.name";
	public final static String PROP_PRENZL_RULE_LIBRARY = "prenzl.rule.library";
	public final static String PROP_PRENZL_CFG_NAME = "prenzl.configuration.name";
	public final static String PROP_PRENZL_CFG_DATE = "prenzl.configuration.date";
	
	private String name;
	private final Date date;
	private Image image;
	private RuleDescriptor ruleDescriptor; 
	private final RuleProperties ruleProperties; 

	public Configuration(RuleDescriptor ruleDescriptor, RuleProperties properties) {
		this(ruleDescriptor,properties,new Date());
	}

	private Configuration(RuleDescriptor ruleDescriptor, RuleProperties properties, Date date) {
		this(ruleDescriptor,properties,date,ruleDescriptor.getRuleName()+" at "+sdf.format(date));
	}
	private Configuration(RuleDescriptor ruleDescriptor, RuleProperties properties, Date date, String name) {
		this.ruleDescriptor = ruleDescriptor;
		this.ruleProperties = properties;
		this.name = name;
		this.date = date;
	}

	public Configuration(InputStream in) throws Exception{
		Properties propTemp = new Properties();
		propTemp.load(in);
		ruleDescriptor = new RuleDescriptor(
				LaunchModel.findLibrary(
					propTemp.getProperty(PROP_PRENZL_RULE_LIBRARY)), 
				propTemp.getProperty(PROP_PRENZL_RULE_NAME));
		
		Iterator it = propTemp.entrySet().iterator();
		ruleProperties = new RuleProperties();
		while (it.hasNext()) {
			Map.Entry entry = (Map.Entry) it.next();
			if(!(entry.getKey().equals(PROP_PRENZL_RULE_NAME)
					||entry.getKey().equals(PROP_PRENZL_RULE_LIBRARY)
					||entry.getKey().equals(PROP_PRENZL_CFG_NAME)
					||entry.getKey().equals(PROP_PRENZL_CFG_DATE)
				)){
				ruleProperties.setProperty(entry.getKey(), entry.getValue());
			}
		}
		
		name = propTemp.getProperty(PROP_PRENZL_CFG_NAME);
		date = sdf.parse(propTemp.getProperty(PROP_PRENZL_CFG_DATE));
	}
	public Library getLibrary() {
		return ruleDescriptor.getLibrary();
	}

	public String getRuleName() {
		return ruleDescriptor.getRuleName();
	}

	public RuleProperties getRuleProperties() {
		return ruleProperties;
	}

	/** @return null, if properties not set*/
	public String getPropertiesAsString() {
		return ruleProperties!=null?ruleProperties.serialize():null;
	}
	
	public synchronized void store(OutputStream out) throws Exception{
		Properties propTemp = new Properties();
		Iterator it = ruleProperties.properties.entrySet().iterator();
		while (it.hasNext()) {
			Map.Entry entry = (Map.Entry) it.next();
			propTemp.setProperty(entry.getKey().toString(), entry.getValue().toString());
		}
		propTemp.setProperty(PROP_PRENZL_RULE_NAME, ruleDescriptor.getRuleName());
		propTemp.setProperty(PROP_PRENZL_RULE_LIBRARY, ruleDescriptor.getLibrary().getName());
		propTemp.setProperty(PROP_PRENZL_CFG_NAME, getName());
		propTemp.setProperty(PROP_PRENZL_CFG_DATE, sdf.format(getDate()));
		propTemp.store(out, null);
	}


	public synchronized void setName(String name) {
		this.name = name;
	}

	public String getName(){
//		if(name==null){
//			return ruleDescriptor.getRuleName() + sdf.format(date);
//		}
		return name;
	}
	public String toString(){
		return getName();
	}

	public RuleDescriptor getRuleDescriptor() {
		return ruleDescriptor;
	}

	public synchronized Date getDate() {
		return date;
	}
	
	public boolean equals(Object o){
		if(o instanceof Configuration){
			Configuration other = (Configuration)o;
			return other.date.equals(date) && other.ruleDescriptor.equals(ruleDescriptor);
		}
		return false;
	}

	public synchronized Image getImage() {
		return image;
	}

	synchronized void setImage(Image image) {
		this.image = image;
	}
	
	
	
}
