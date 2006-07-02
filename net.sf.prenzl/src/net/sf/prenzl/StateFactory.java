package net.sf.prenzl;

import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.ui.IElementFactory;
import org.eclipse.ui.IMemento;

public class StateFactory implements IElementFactory {
	public static final String ID = PrenzlPlugin.ID+".stateFactory";

	public IAdaptable createElement(IMemento memento) {
		return new State(memento);
	}
}
