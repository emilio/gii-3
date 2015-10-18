package messages;

public class AlumnToTeacherMessage extends Message {
    public AlumnToTeacherMessage(MessageType type) {
        super(type);
        assert(type == MessageType.REQUEST_FIRST_ASSIGNATION);
    }
}
