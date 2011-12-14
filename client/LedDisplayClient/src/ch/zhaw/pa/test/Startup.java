package ch.zhaw.pa.test;

import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;

import javax.imageio.ImageIO;

import ch.zhaw.pa.imageconverter.ImageReader;
import ch.zhaw.pa.model.Image;

public class Startup {
	private static DecimalFormat f = new DecimalFormat("#0.00");

	public static String readableFileSize(long size) {
		if (size <= 0) {
			return "0";
		}
		final String[] units = new String[] { "B", "KB", "MB", "GB", "TB" };
		int digitGroups = (int) (Math.log10(size) / Math.log10(1024));
		return new DecimalFormat("#,##0.#").format(size / Math.pow(1024, digitGroups)) + " " + units[digitGroups];
	}

	public static void main(String[] args) throws IOException {
		ImageReader reader = new ImageReader();

		for (int i = 1; i <= 10; i++) {
			CountOutputStream outWithoutCompression = new CountOutputStream();
			CountOutputStream outWithCompression = new CountOutputStream();

			Image img = reader.read(Startup.class.getResourceAsStream("test" + i + ".png"));

			ImageIO.write(img.toBufferedImage(), "PNG", new File("/tmp/pa-test" + i + ".png"));

			// BeagleClient client = new BeagleClient();
			// client.connect("localhost", 50009);
			//

			int wo = outWithoutCompression.getCount();
			int w = outWithCompression.getCount();

			double factor = 1.0 * wo / w ;

			System.out.println("Bild " + i + " MSG Originalgrösse: " + readableFileSize(wo) + ", Compressed: " + readableFileSize(w) + ", " + 
			
					f.format(factor)
					
					
					+ " mal grösser ohne Kompression");
		}
		System.out.println("fertig");

	}
}
