package ch.zhaw.pa.model;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;

public class Image {
	private int width;
	private int height;

	private byte[] data;

	public Image(int width, int height) {
		setSize(width, height);
	}

	public void setSize(int width, int height) {
		this.width = width;
		this.height = height;

		int size = this.width * this.height * 3;
		if (size % 2 == 1) {
			size += 1;
		}

		// 4 bit data
		size /= 2;

		this.data = new byte[size];
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public byte[] getData() {
		return data;
	}

	/**
	 * A new buffered image is created each time this function is called
	 * 
	 * @return The new created buffered Image
	 */
	public BufferedImage toBufferedImage() {
		BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_3BYTE_BGR);

		byte[] data = ((DataBufferByte) img.getRaster().getDataBuffer()).getData();

		int j = 0;
		for (int i = 0; i < this.data.length; i++) {
			int b = this.data[i];

			data[j++] = (byte) (b & 0xf0);
			data[j++] = (byte) ((b & 0x0f) << 4);
		}

		return img;
	}

	public void setDataFrom8Bit(byte[] data8bit) {
		if (data8bit.length != this.data.length * 2) {
			throw new IllegalArgumentException("Image size don't match");
		}

		int j = 0;
		for (int i = 0; i < this.data.length; i++) {
			int b = data8bit[j++] & 0xf0;
			b |= data8bit[j++] >> 4;

			this.data[i] = (byte) b;
		}
	}
	
	public void setData(byte[] data) {
		if (data.length != this.data.length) {
			throw new IllegalArgumentException("Image size don't match");
		}
		this.data = data;
	}
	
	public static void saveImage(OutputStream out, Image img) throws IOException {
		ObjectOutputStream oout = new ObjectOutputStream(out);
		oout.writeInt(img.width);
		oout.writeInt(img.height);
		oout.writeObject(img.data);
	}
	
	public static Image loadImage(InputStream in) throws IOException, ClassNotFoundException {
		ObjectInputStream oin = new ObjectInputStream(in);
		Image img = new Image(oin.readInt(), oin.readInt());
		img.setData((byte[]) oin.readObject());
		
		return img;
	}
}
