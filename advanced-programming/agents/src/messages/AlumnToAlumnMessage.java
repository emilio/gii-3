package messages;

public class AlumnToAlumnMessage extends Message {
    private static final long serialVersionUID = 2732307084667046717L;

    public AlumnToAlumnMessage(MessageType type) {
        super(type);
        assert type == MessageType.GROUP_CHANGE_REQUEST
                || type == MessageType.GROUP_CHANGE_REQUEST_CONFIRMATION
                || type == MessageType.GROUP_CHANGE_REQUEST_DENEGATION;
    }
}
