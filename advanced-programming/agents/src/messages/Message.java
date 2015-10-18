package messages;

public class Message implements java.io.Serializable {
    private MessageType type;

    public Message(MessageType type) {
        this.type = type;
    }

    public MessageType getType() {
        return this.type;
    }
}
