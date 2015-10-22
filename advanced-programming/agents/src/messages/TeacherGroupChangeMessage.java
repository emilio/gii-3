package messages;

import agents.Availability;

import jade.core.AID;

public final class TeacherGroupChangeMessage extends TeacherToAlumnMessage {
    private static final long serialVersionUID = -4186883935138393549L;

    public final AID fromAlumn;
    public final AID toAlumn;
    public final Availability fromGroup;
    public final Availability toGroup;

    public TeacherGroupChangeMessage(AID fromAlumn, AID toAlumn, Availability fromGroup,
            Availability toGroup) {
        super(MessageType.TEACHER_GROUP_CHANGE);
        this.fromAlumn = fromAlumn;
        this.toAlumn = toAlumn;
        this.fromGroup = fromGroup;
        this.toGroup = toGroup;
    }
}
