package ch.zhaw.pa.test;

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;

import javax.imageio.ImageIO;

import ch.zhaw.pa.imageconverter.ImageReader;

public class ConvertImages {

	private static ImageObserver observer = new ImageObserver() {

		@Override
		public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) {
			return false;
		}
	};

	public static void main(String[] args) throws IOException {
		File infile = new File("/home/andreas/dwhelper/images");
		String outpath = "/home/andreas/dwhelper/converted/";

		File[] files = infile.listFiles(new FilenameFilter() {

			@Override
			public boolean accept(File dir, String name) {
				return name.endsWith(".png");
			}
		});

		for (int i = 0; i < files.length; i++) {
			System.out.println("->" + i + " / " + files.length);
			convertFile(files[i], outpath);
		}

		// convertFile(new File("/home/andreas/dwhelper/images/00271.png"),
		// outpath);

	}

	private static void convertFile(File f, String outpath) throws IOException {
		BufferedImage src = ImageIO.read(f);
		BufferedImage img = new BufferedImage(1000, 128, BufferedImage.TYPE_INT_RGB);
		Graphics2D g = img.createGraphics();

		g.drawImage(src, 0, 0, 1000, 128, 400, 500, 1400, 628, observer);

		g.dispose();

		ch.zhaw.pa.model.Image img12bit = ImageReader.read(img);

		// ImageIO.write(img, "PNG", new File(outpath+f.getName()));

		FileOutputStream out = new FileOutputStream(outpath + f.getName() + ".bin");
		ch.zhaw.pa.model.Image.saveImage(out, img12bit);
		out.close();

	}

}
