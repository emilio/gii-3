package messages;

public class TeacherGroupChangeRequestDenegationMessage extends TeacherToAlumnMessage {
    private static final long serialVersionUID = 354523242594204016L;

    public TeacherGroupChangeRequestDenegationMessage() {
        super(MessageType.TEACHER_GROUP_CHANGE_REQUEST_DENEGATION);
    }
}
