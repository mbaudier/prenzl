package net.sf.prenzl.util;

import org.eclipse.swt.graphics.ImageData;

public class ImageUtil {

	public static ImageData fit(ImageData imageData, int displayWidth, int displayHeight){
		float scale = Math.min(
				((float)displayWidth)/imageData.width,
				((float)displayHeight)/imageData.height);
		return imageData.scaledTo(
					(int)(imageData.width*scale),
					(int)(imageData.height*scale));
	}
	
	public static ImageData strech(ImageData imageData, int displayWidth, int displayHeight){
		return imageData.scaledTo(
				displayWidth,
				displayHeight);
	}

}
