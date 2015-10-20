package messages;

import agents.Availability;


public final class TeacherGroupChangeMessage extends TeacherToAlumnMessage {
    public final Availability fromGroup;
    public final Availability toGroup;

    public TeacherGroupChangeMessage(Availability fromGroup, Availability toGroup) {
        super(MessageType.TEACHER_GROUP_CHANGE);
        this.fromGroup = fromGroup;
        this.toGroup = toGroup;
    }
}
