package messages;

import agents.Availability;
import jade.core.AID;

public class TeacherGroupChangeRequestMessage extends AlumnToTeacherMessage {
    private static final long serialVersionUID = 8028432203626871805L;
    public final AID fromAlumn;
    public final AID toAlumn;
    public final Availability fromGroup;
    public final Availability toGroup;

    public TeacherGroupChangeRequestMessage(AID fromAlumn, AID toAlumn, Availability fromGroup,
            Availability toGroup) {
        super(MessageType.TEACHER_GROUP_CHANGE_REQUEST);
        this.fromAlumn = fromAlumn;
        this.toAlumn = toAlumn;
        this.fromGroup = fromGroup;
        this.toGroup = toGroup;
    }
}
