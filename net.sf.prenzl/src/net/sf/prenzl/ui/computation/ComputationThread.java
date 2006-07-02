package net.sf.prenzl.ui.computation;

import net.sf.prenzl.util.Log;

abstract class ComputationThread extends Thread {
	private boolean valid = true;
	private boolean running = false;

	protected abstract void computationStep() throws Exception;
	protected abstract void displayStep() throws Exception;

	private double load = 0.85d;

	private long computeDurationSum = 0;
	private long displayDurationSum = 0;

	public void run() {
		try {
			long beginComputation = System.currentTimeMillis();
			// loopCount is used only for performance measures.
			// it can be different from cycleCount in ComputationUIU since cycleCount 
			// takes next/previousStep into account
			int loopCount = 0; 
			
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
				loopCount++;
				if (timeToSleep > 0) {
					synchronized (this) {
						wait((long) timeToSleep);
					}
				}
			}

			synchronized (this) {
				notifyAll();
			}

			if (loopCount != 0) {
				long duration = System.currentTimeMillis() - beginComputation;
				logStatReport(duration, loopCount, computeDurationSum, displayDurationSum);
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
		Thread.yield();// lets graphical threads takes the hand, before sleeping anyhow
		long displayDuration = System.currentTimeMillis() - beginDisplay;
		displayDurationSum = displayDurationSum + displayDuration;


		double cycleDuration = (double) (computeDuration + displayDuration);
		return cycleDuration / load	- cycleDuration;
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

	private static void logStatReport(long duration, int cycleCount, long computeDurationSum,
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
