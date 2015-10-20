package messages;

import agents.Availability;

public final class TeacherGroupChangeMessage extends TeacherToAlumnMessage {
    private static final long serialVersionUID = -4186883935138393549L;
    public final Availability fromGroup;
    public final Availability toGroup;

    public TeacherGroupChangeMessage(Availability fromGroup, Availability toGroup) {
        super(MessageType.TEACHER_GROUP_CHANGE);
        this.fromGroup = fromGroup;
        this.toGroup = toGroup;
    }
}
