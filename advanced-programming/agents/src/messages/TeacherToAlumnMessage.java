package messages;

public class TeacherToAlumnMessage extends Message {
    private static final long serialVersionUID = -4521733008797925758L;

    public TeacherToAlumnMessage(MessageType type) {
        super(type);
        assert type == MessageType.FIRST_ASSIGNATION || type == MessageType.TERMINATION_REQUEST
                || type == MessageType.TEACHER_GROUP_CHANGE
                || type == MessageType.TEACHER_GROUP_CHANGE_REQUEST_DENEGATION;
    }
}
