package net.sf.prenzl;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.StringTokenizer;

public class Log {
	public static String prefix = "_ ";
	public final static SimpleDateFormat sdfLog = 
		new SimpleDateFormat("yyyy/MM/dd hh:mm:ss");
	
	public final static int LOG_OFF = 0;
	public final static int LOG_ERROR = 10;
	public final static int LOG_WARN = 20;
	public final static int LOG_INFO = 30;
	public final static int LOG_VERBOSE = 40;
	public final static int LOG_DEBUG = 50;
	public final static int LOG_TRACE = 60;

	private static int logLevel = LOG_DEBUG;
	
	public synchronized static int getLogLevel(){
		return logLevel;
	}
	
	public synchronized static void setLogLevel(int logLevel){
		Log.logLevel = logLevel;
	}
	
	public static boolean isDebug(){
		return getLogLevel() >= LOG_DEBUG;
	}
	
	public static boolean isTrace(){
		return getLogLevel() >= LOG_TRACE;
	}
	
	public static void error(Object msg, Exception e){
		log(LOG_ERROR, msg, e);
	}
	
	public static void warn(Object msg, Exception e){
		log(LOG_WARN, msg, e);
	}
	
	public static void info(Object msg){
		log(LOG_INFO, msg, null);
	}
	
	public static void verbose(Object msg){
		log(LOG_VERBOSE, msg, null);
	}
	
	public static void debug(Object msg){
		log(LOG_DEBUG, msg, null);
	}
	
	public static void trace(Object msg){
		log(LOG_TRACE, msg, null);
	}
	
	
	private static void log(int level, Object msg, Throwable e){
		if(getLogLevel()>=level){
			StringBuffer buf = new StringBuffer("");
			buf.append(prefix);
			String levelStr = "<unknown>";
			switch(level){
				case LOG_ERROR: levelStr   = "ERROR"; break;
				case LOG_WARN: levelStr    = "WARN "; break;
				case LOG_INFO: levelStr    = "INFO "; break;
				case LOG_VERBOSE: levelStr = "INFO+"; break;
				case LOG_DEBUG: levelStr   = "DEBUG"; break;
				case LOG_TRACE: levelStr   = "TRACE"; break;
			}
			buf.append(levelStr);
			buf.append(" ");
			buf.append(sdfLog.format(new Date()));
			buf.append(" ");
			String location = null;
			try{
				throw new Exception("");
			}
			catch (Exception fakeE){
				StringWriter strOut = new StringWriter();
				fakeE.printStackTrace(new PrintWriter(strOut));
				String stack = strOut.toString();
				location = stack;
				try {
					StringTokenizer st = new StringTokenizer(stack,"\n\t");
					st.nextToken();// message
					st.nextToken();// call of print
					st.nextToken();// call of info, debug, error, etc.
					location = st.nextToken();// actual line were the log was called
				} catch (Exception e1) {
					// dirty safety, since previous code is unsafe 
					// and a logging should be robust
					location = "<unknown location>";
				}
			}
			buf.append(location);
			buf.append('\n');
			buf.append(msg);
			buf.append('\n');
			if(e != null){
				StringWriter strOut = new StringWriter();
				e.printStackTrace(new PrintWriter(strOut));
				buf.append(strOut.toString());
			}
			
			if(level<=LOG_WARN){
				System.err.print(buf);
			}
			else {
				System.out.print(buf);
			}
		}
	}

}
