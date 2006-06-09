package net.sf.prenzl.launch;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;

import net.sf.prenzl.util.Log;

public class RuleProperties {
	final Properties properties;

	public RuleProperties() {
		properties = new Properties();
	}

	public RuleProperties(String serialedProperties) {
		this();
		parse(serialedProperties);
	}

	public RuleProperties(RuleProperties ruleProperties) {
		this();
		Iterator it = ruleProperties.properties.entrySet().iterator();
		while (it.hasNext()) {
			Map.Entry entry = (Map.Entry) it.next();
			properties.setProperty(entry.getKey().toString(), entry.getValue().toString());
		}
	}

	public void setProperty(Object key, Object value){
		properties.setProperty(key.toString(),value.toString());
	}
	
	public String getString(String key){
		return properties.getProperty(key);
	}
	
	public int getInt(String key){
		return Integer.parseInt(properties.getProperty(key));
	}
	
	public Map.Entry[] getOrderedEntries(){
		//TreeMap treeMap = new TreeMap(properties);
		Map.Entry[] entries = new Map.Entry[properties.size()];
		Iterator itEntries = properties.entrySet().iterator();
		int i = 0;
		while (itEntries.hasNext()) {
			entries[i] = (Map.Entry) itEntries.next();
			i++;
		}
		return entries;
	}
	
	public String serialize(){
		try {
			ByteArrayOutputStream out = new ByteArrayOutputStream();		
			properties.store(out,null);
			String res = out.toString();
			out.close();
			return res;
		} catch (IOException e) {
			String msg = "Could not serialize properties";
			Log.error(msg,e);
			throw new RuntimeException(msg,e);
		}
	}
	
	public void parse(InputStream in){
		try {
			properties.clear();
			properties.load(in);
		} catch (IOException e) {
			Log.error("Could not parse properties",e);
		}
	}
	
	public void parse(String str){
		try {
			InputStream in = new ByteArrayInputStream(str.getBytes());
			parse(in);
			in.close();
		} catch (IOException e) {
			Log.error("Could not interpret String as InputStream",e);
		}
	}
}
