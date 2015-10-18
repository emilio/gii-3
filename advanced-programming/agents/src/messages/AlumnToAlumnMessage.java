package messages;

public class AlumnToAlumnMessage extends Message {
    public AlumnToAlumnMessage(MessageType type) {
        super(type);
        assert(type == MessageType.REQUEST_GROUP_CHANGE);
    }
}
