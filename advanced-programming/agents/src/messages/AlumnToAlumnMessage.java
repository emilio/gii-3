package messages;

public class AlumnToAlumnMessage extends Message {
    public AlumnToAlumnMessage(MessageType type) {
        super(type);
        assert(type == MessageType.GROUP_CHANGE_REQUEST ||
               type == MessageType.GROUP_CHANGE_REQUEST_CONFIRMATION ||
               type == MessageType.GROUP_CHANGE_REQUEST_DENEGATION);
    }
}
