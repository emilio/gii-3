package messages;

public class Message implements java.io.Serializable {
    private static final long serialVersionUID = 4348233472781482752L;
    private final MessageType type;

    public Message(MessageType type) {
        this.type = type;
    }

    public MessageType getType() {
        return this.type;
    }
}
