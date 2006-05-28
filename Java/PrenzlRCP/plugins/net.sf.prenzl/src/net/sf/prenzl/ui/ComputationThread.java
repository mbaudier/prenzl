package net.sf.prenzl.ui;

import net.sf.prenzl.Log;
import net.sf.prenzl.PrenzlPlugin;

abstract class ComputationThread extends Thread {
	private boolean valid = true;
	private boolean running = false;

	protected abstract void computationStep() throws Exception;
	protected abstract void displayStep() throws Exception;

	private double load = 0.85d;

	private long computeDurationSum = 0;
	private long displayDurationSum = 0;
	private int cycleCount = 0;

	public void run() {
		try {
			long beginComputation = System.currentTimeMillis();
			cycleCount = 0;
			// TODO: find a cleaner way to propagate the cycle count
			PrenzlPlugin.getRunModel().setCount(cycleCount);

			while (valid) {
				synchronized (this) {
					while (!running && valid) {
						try {
							wait(100);
						}
						catch (InterruptedException e) {
						}
					}
				}
				if (!valid) {
					break;
				}

				double timeToSleep = cycle();
				if (timeToSleep > 0) {
					synchronized (this) {
						wait((long) timeToSleep);
					}
				}

			}

			synchronized (this) {
				notifyAll();
			}

			if (cycleCount != 0) {
				long duration = System.currentTimeMillis() - beginComputation;
				logStatReport(duration, cycleCount, computeDurationSum, displayDurationSum);
			}
		}
		catch (Exception e) {
			dispose();
			Log.error("Unexpected exception killed the cycle thread", e);
		}

	}

	public double cycle() throws Exception{
		long beginCompute = System.currentTimeMillis();
		// ***** COMPUTATION ***** //
		computationStep();
		// *********************** //
		long computeDuration = System.currentTimeMillis() - beginCompute;
		computeDurationSum = computeDurationSum + computeDuration;

		long beginDisplay = System.currentTimeMillis();
		// ***** DISPLAY ***** //
		displayStep();
		// ******************* //
		// TODO: find a cleaner way to propagate the cycle count
		PrenzlPlugin.getRunModel().setCount(cycleCount);
		Thread.yield();// lets graphical threads takes the hand, before sleeping anyhow
		long displayDuration = System.currentTimeMillis() - beginDisplay;
		cycleCount++;
		displayDurationSum = displayDurationSum + displayDuration;


		double cycleDuration = (double) (computeDuration + displayDuration);
		return cycleDuration / load	- cycleDuration;
	}
	
	public void decrementCount(){
		cycleCount--;
		// TODO: find a cleaner way to propagate the cycle count
		PrenzlPlugin.getRunModel().setCount(cycleCount);
	}
	
	public synchronized void setRunning(boolean running) {
		this.running = running;
		notifyAll();
	}
	public synchronized boolean isRunning() {
		return running;
	}
	public synchronized void dispose() {
		this.valid = false;
		notifyAll();
	}

	private void logStatReport(long duration, int cycleCount, long computeDurationSum,
			long paintDurationSum) {
		long avgCycleDuration = duration / cycleCount;
		long avgComputeDuration = (computeDurationSum) / cycleCount;
		long avgPaintDuration = (paintDurationSum) / cycleCount;
		StringBuffer buf = new StringBuffer("");
		buf.append("Computation stats: duration=" + duration + "ms cycleCount=" + cycleCount);
		buf.append(" avgCycle=" + avgCycleDuration + "ms");
		buf.append(" avgCompute=" + avgComputeDuration + "ms(" + (avgComputeDuration * 100)
				/ avgCycleDuration + "%)");
		buf.append(" avgDipslay=" + avgPaintDuration + "ms(" + (avgPaintDuration * 100)
				/ avgCycleDuration + "%)");
		Log.verbose(buf);
	}
}
