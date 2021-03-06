package ch.zhaw.pa.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;

import ch.zhaw.pa.model.Image;
import ch.zhaw.pa.util.GzUtil;

public class TcpClient {
	private Socket socket;
	private PrintStream out;
	private BufferedReader in;

	public TcpClient(String server, int port) throws UnknownHostException, IOException {
		this.socket = new Socket(server, port);
		this.out = new PrintStream(socket.getOutputStream());
		this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
	}

	public void sendImage(Image img, int sequence, boolean compress) throws IOException {
		sendMessageType(MessageType.IMAGE);
		sendInt(sequence);

		byte[] data = img.getData();

		sendInt(compress ? 1 : 0);

		if (compress) {
			// size
			sendInt(data.length);

			out.write(data);
		} else {
			byte[] compressed = GzUtil.compress(data);
			
			// size
			sendInt(compressed.length);

			out.write(compressed);
		}

//		String ret = readLine();
//		if (!"OK".equals(ret)) {
//			throw new IOException("Recived message error \"" + ret + "\"");
//		}
	}

	private String readLine() throws IOException {
		return this.in.readLine();
	}

	private void sendMessageType(MessageType type) {
		sendInt(type.getType());
	}

	private void sendInt(int value) {
		out.print(value);
		out.print(' ');
	}
}
