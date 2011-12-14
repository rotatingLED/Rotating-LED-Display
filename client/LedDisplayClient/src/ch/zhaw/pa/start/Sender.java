package ch.zhaw.pa.start;

import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.net.UnknownHostException;

import ch.zhaw.pa.client.TcpClient;
import ch.zhaw.pa.model.Image;

public class Sender {

	private TcpClient client;

	public Sender() throws UnknownHostException, IOException {
		client = new TcpClient("127.0.0.1", 28900);
	}

	private void send() throws IOException, ClassNotFoundException {
		File infile = new File("/home/andreas/dwhelper/converted/");

		File[] files = infile.listFiles(new FilenameFilter() {

			@Override
			public boolean accept(File dir, String name) {
				return name.endsWith(".png.bin");
			}
		});

		for (int i = 0; i < files.length; i++) {
			System.out.println("->" + i + " / " + files.length);
			for (int x = 0; x < 100; x++) {
				System.out.println("  " + x);
				sendFile(files[i], i * 100 + x);
			}
		}

		System.out.println("fertig");

	}

	private void sendFile(File file, int sequence) throws IOException, ClassNotFoundException {
		FileInputStream in = new FileInputStream(file);
		Image img = Image.loadImage(in);
		in.close();

		client.sendImage(img, sequence);
	}

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		Sender s = new Sender();
		s.send();
	}
}
