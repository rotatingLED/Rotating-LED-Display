package ch.zhaw.pa.imageconverter;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

import ch.zhaw.pa.model.Image;

public class ImageReader {

	public static Image read(InputStream in) throws IOException {
		return read(ImageIO.read(in));
	}

	public static Image read(java.awt.Image read) {
		Image img = new Image(read.getWidth(null), read.getHeight(null));
		BufferedImage bimg = new BufferedImage(read.getWidth(null), read.getHeight(null), BufferedImage.TYPE_3BYTE_BGR);

		Graphics2D g = bimg.createGraphics();
		
		g.drawImage(read, 0, 0, null);
		
		g.dispose();

		img.setDataFrom8Bit(((DataBufferByte) bimg.getRaster().getDataBuffer()).getData());

		return img;
	}

}
