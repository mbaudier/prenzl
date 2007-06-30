package net.sf.prenzl.ui.video;

import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.DirectColorModel;
import java.awt.image.IndexColorModel;
import java.awt.image.WritableRaster;
import java.io.ByteArrayOutputStream;
import java.util.Vector;

import net.sf.prenzl.PrenzlPlugin;
import net.sf.prenzl.util.Log;

import org.eclipse.jface.dialogs.IInputValidator;
import org.eclipse.jface.dialogs.InputDialog;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.ImageLoader;
import org.eclipse.swt.graphics.PaletteData;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGEncodeParam;
import com.sun.image.codec.jpeg.JPEGImageEncoder;

public class Recorder {
	private final Vector frames;

	private final FramesToMovie imagesToMovie;

	private final String moviePath;

	private final String quality;

	private final int frameRate;

	private boolean ended = false;

	public Recorder(String moviePath, String quality, int frameRate) {
		this.moviePath = moviePath;
		this.quality = quality.toLowerCase();
		this.frameRate = frameRate;

		frames = new Vector();
		imagesToMovie = new FramesToMovie();
	}

	public synchronized byte[] getNextFrame() {
		while (!isDataAvailable() && !isEnded()) {
			try {
				wait(100);
			} catch (Exception e) {
			}
		}

		if (isEnded()) {
			return null;
		}
		byte[] res = (byte[]) frames.get(0);
		frames.remove(0);
		return res;
	}

	public void record(int width, int height) {

		imagesToMovie.doIt(width, height, frameRate, Recorder.this,
				FramesToMovie.createMediaLocator("file://" + moviePath));
	}

	public boolean isDataAvailable() {
		return frames.size() != 0;
	}

	/**
	 * Block until file writing is done.
	 */
	public boolean waitForFileDone() {
		return imagesToMovie.waitForFileDone();
	}

	public void recordImage(ImageData imageData) {
		try {

			// byte[] arr = new byte[imageDataScaled.data.length];
			// System.arraycopy(imageDataScaled.data, 0, arr, 0, arr.length);

			ByteArrayOutputStream out = new ByteArrayOutputStream();

			// Check if is a number
			boolean isQualityLevel = false;
			float qual = 1f;
			try {
				qual = Float.parseFloat(quality);
				if (qual >= 0 && qual <= 1) {
					isQualityLevel = true;
				}
			} catch (Exception e) {
			}

			if (!isQualityLevel) {
				if (quality.equals("high")) {
					qual = 1f;
					isQualityLevel = true;
				}
			}

			if (isQualityLevel) {
				BufferedImage img = convertToAWT(imageData);
				JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(out);
				JPEGEncodeParam param = encoder.getDefaultJPEGEncodeParam(img);
				param.setQuality(qual, true);
				encoder.encode(img, param);
			} else if (quality.equals("fast")) {// swt
				ImageLoader imgl = new ImageLoader();
				imgl.data = new ImageData[] { imageData };
				imgl.save(out, SWT.IMAGE_JPEG);
			} else {
				throw new RuntimeException("Unknown quality level");
			}
			byte[] arr = out.toByteArray();
			out.close();

			frames.add(arr);
			Log.info("Recorded image ");

			/*
			 * String countStr = null; if(cCount<10){ countStr =
			 * "00000"+cCount; } else if(cCount<100){ countStr = "0000"+cCount; }
			 * else if(cCount<1000){ countStr = "000"+cCount; } else if(cCount<10000){
			 * countStr = "00"+cCount; } else if(cCount<100000){ countStr =
			 * "0"+cCount; } else{ countStr = Integer.toString(cCount); }
			 * 
			 * File file = new File(dumpDir.getPath() + File.separator + "img-" +
			 * countStr + ".jpg"); FileOutputStream out = new
			 * FileOutputStream(file); imgl.save(out, SWT.IMAGE_JPEG);
			 * out.close(); Log.info("Dumped " + file);
			 */
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void cleanUp() {
		if (imagesToMovie != null) {
			imagesToMovie.cleanUp();
		}
	}

	public synchronized boolean isEnded() {
		return ended && frames.size() == 0;
	}

	public synchronized void setEnded(boolean ended) {
		this.ended = ended;
		notifyAll();
	}

	public static Recorder askForMovieLocation() {
		Shell shell = PrenzlPlugin.getDefaultWorkbench()
				.getActiveWorkbenchWindow().getShell();
		FileDialog fileDialog = new FileDialog(shell, SWT.SAVE);
		fileDialog.setFilterExtensions(new String[] { "*.mov" });
		final String path;
		if ((path = fileDialog.open()) != null) {
			IInputValidator validator = new IInputValidator() {

				public String isValid(String newText) {
					String[] params = parseParams(newText);
					String quality = params[0];
					String frameRate = params[1];
					if (frameRate != null) {
						try {
							Integer.parseInt(frameRate);
						} catch (Exception e) {
							return e.getMessage();
						}
					}
					
					if (quality.equals("fast")) {
						return null;
					} else if (quality.equals("high")) {
						return null;
					} else {
						try {
							Float.parseFloat(quality);
							return null;
						} catch (Exception e) {
							return e.getMessage();
						}
					}
				}

			};

			InputDialog dialog = new InputDialog(
					shell,
					"Video quality",
					"Please enter video quality: 'fast', 'high' or a rate between 0.0 (low) and 1.0 (high)",
					"fast", validator);
			if (dialog.open() == Window.OK) {
				String[] params = parseParams(dialog.getValue());
				String quality = params[0];
				String frameRate = params[1];
				return new Recorder(path, quality, frameRate!=null?Integer.parseInt(frameRate):10);
			}

		}
		return null;
	}

	private static String[] parseParams(String newText) {
		int comaIndex = newText.indexOf(',');

		String quality = null;
		String frameRate = null;
		if (comaIndex > 0 && comaIndex+1<newText.length()) {
			quality = newText.substring(0, comaIndex).toLowerCase();
			frameRate = newText.substring(comaIndex+1);
		} else {
			quality = newText.toLowerCase();
		}

		return new String[] { quality, frameRate };
	}

	static BufferedImage convertToAWT(ImageData data) {
		ColorModel colorModel = null;
		PaletteData palette = data.palette;
		if (palette.isDirect) {
			colorModel = new DirectColorModel(data.depth, palette.redMask,
					palette.greenMask, palette.blueMask);
			BufferedImage bufferedImage = new BufferedImage(colorModel,
					colorModel.createCompatibleWritableRaster(data.width,
							data.height), false, null);
			WritableRaster raster = bufferedImage.getRaster();
			int[] pixelArray = new int[3];
			for (int y = 0; y < data.height; y++) {
				for (int x = 0; x < data.width; x++) {
					int pixel = data.getPixel(x, y);
					RGB rgb = palette.getRGB(pixel);
					pixelArray[0] = rgb.red;
					pixelArray[1] = rgb.green;
					pixelArray[2] = rgb.blue;
					raster.setPixels(x, y, 1, 1, pixelArray);
				}
			}
			return bufferedImage;
		} else {
			RGB[] rgbs = palette.getRGBs();
			byte[] red = new byte[rgbs.length];
			byte[] green = new byte[rgbs.length];
			byte[] blue = new byte[rgbs.length];
			for (int i = 0; i < rgbs.length; i++) {
				RGB rgb = rgbs[i];
				red[i] = (byte) rgb.red;
				green[i] = (byte) rgb.green;
				blue[i] = (byte) rgb.blue;
			}
			if (data.transparentPixel != -1) {
				colorModel = new IndexColorModel(data.depth, rgbs.length, red,
						green, blue, data.transparentPixel);
			} else {
				colorModel = new IndexColorModel(data.depth, rgbs.length, red,
						green, blue);
			}
			BufferedImage bufferedImage = new BufferedImage(colorModel,
					colorModel.createCompatibleWritableRaster(data.width,
							data.height), false, null);
			WritableRaster raster = bufferedImage.getRaster();
			int[] pixelArray = new int[1];
			for (int y = 0; y < data.height; y++) {
				for (int x = 0; x < data.width; x++) {
					int pixel = data.getPixel(x, y);
					pixelArray[0] = pixel;
					raster.setPixel(x, y, pixelArray);
				}
			}
			return bufferedImage;
		}
	}

}
