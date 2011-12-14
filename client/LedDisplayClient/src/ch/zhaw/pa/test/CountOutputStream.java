package ch.zhaw.pa.test;

import java.io.IOException;
import java.io.OutputStream;

public class CountOutputStream extends OutputStream {
	private int count = 0;

	@Override
	public void write(int b) throws IOException {
		count++;
	}

	public int getCount() {
		return count;
	}
}
