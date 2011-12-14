package ch.zhaw.pa.client;

public enum MessageType {
	IMAGE(1);

	private int type;

	private MessageType(int type) {
		this.type = type;
	}

	public int getType() {
		return type;
	}
}
