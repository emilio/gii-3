package messages;

import agents.Availability;

public class FirstAssignationMessage extends TeacherToAlumnMessage {
    private static final long serialVersionUID = 8775855277633710105L;
    private final Availability group;

    public FirstAssignationMessage(Availability availability) {
        super(MessageType.FIRST_ASSIGNATION);
        this.group = availability;
    }

    public Availability getGroup() {
        return this.group;
    }
}
