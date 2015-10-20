package messages;

public class AlumnToTeacherMessage extends Message {
    public AlumnToTeacherMessage(MessageType type) {
        super(type);
        assert(type == MessageType.FIRST_ASSIGNATION_REQUEST ||
               type == MessageType.TERMINATION_CONFIRMATION ||
               type == MessageType.TEACHER_GROUP_CHANGE_REQUEST);
    }
}
