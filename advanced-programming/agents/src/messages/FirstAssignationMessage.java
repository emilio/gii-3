package messages;

import agents.Availability;

public class FirstAssignationMessage extends TeacherToAlumnMessage {
    private Availability group;

    public FirstAssignationMessage(Availability availability) {
        super(MessageType.FIRST_ASSIGNATION);
        this.group = availability;
    }

    public Availability getGroup() {
        return this.group;
    }
}
