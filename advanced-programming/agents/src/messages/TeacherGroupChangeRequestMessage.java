package messages;

import jade.core.AID;
import agents.Availability;

public class TeacherGroupChangeRequestMessage extends AlumnToTeacherMessage {
    public final AID fromAlumn;
    public final AID toAlumn;
    public final Availability fromGroup;
    public final Availability toGroup;

    public TeacherGroupChangeRequestMessage(AID fromAlumn, AID toAlumn, Availability fromGroup, Availability toGroup) {
        super(MessageType.TEACHER_GROUP_CHANGE_REQUEST);
        this.fromAlumn = fromAlumn;
        this.toAlumn = toAlumn;
        this.fromGroup = fromGroup;
        this.toGroup = toGroup;
    }
}
