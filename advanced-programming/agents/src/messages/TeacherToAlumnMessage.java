package messages;

public class TeacherToAlumnMessage extends Message {
    public TeacherToAlumnMessage(MessageType type) {
        super(type);
        assert(type == MessageType.FIRST_ASSIGNATION ||
               type == MessageType.TERMINATION_REQUEST);
    }
}
