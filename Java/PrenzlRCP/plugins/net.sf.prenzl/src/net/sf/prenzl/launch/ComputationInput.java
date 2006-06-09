package net.sf.prenzl.launch;

import org.eclipse.swt.graphics.ImageData;

public class ComputationInput {
	private String filePath;
	private ImageData imageData;
	
	public ComputationInput(String filePath)
	{
		this.filePath = filePath;
	}
	public ComputationInput(ImageData imageData)
	{
		this.imageData = imageData;
	}
	
	public ImageData getData(){
		if(imageData == null){
			imageData = new ImageData(filePath);
		}
		return imageData;
	}
	
	public String getLocation(){
		return filePath!=null?filePath:"INTERNAL";
	}

}
