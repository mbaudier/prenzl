package net.sf.prenzl.ui.actions;

import java.util.Observable;
import java.util.Observer;

import net.sf.prenzl.SharedImages;
import net.sf.prenzl.ui.computation.ComputationUI;
import net.sf.prenzl.ui.video.Recorder;

import org.eclipse.jface.action.Action;

public class RecordAction extends Action implements Observer {
	private final ComputationUI computationUI;

	public RecordAction(ComputationUI computationUI) {
		this.computationUI = computationUI;
		this.computationUI.addObserver(this);
		update(this.computationUI, null);
	}

	public RecordAction(ComputationUI computationUI, String text) {
		this(computationUI);
		setText(text);
	}

	public void run() {
		if (computationUI.isRecording()) {
			computationUI.closeRecorder();
		} else {
			Recorder recorder = Recorder.askForMovieLocation();
			computationUI.setRecorder(recorder);

		}
	}

	public void update(Observable o, Object arg) {
		if (o == computationUI) {
			if (!computationUI.isRecording()) {
				setToolTipText("Record");
				if (getText() != null) {
					setText("Record");
				}
				setImageDescriptor(SharedImages.IMGD_MOVIE_OFF);
			} else {
				setToolTipText("Stop recording");
				if (getText() != null) {
					setText("Stop");
				}
				setImageDescriptor(SharedImages.IMGD_MOVIE_ON);
			}

		}
	}

}
