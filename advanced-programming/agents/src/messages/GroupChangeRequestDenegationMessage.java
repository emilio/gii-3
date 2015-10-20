package messages;

import agents.Availability;

public class GroupChangeRequestDenegationMessage extends AlumnToAlumnMessage {
    private Availability deniedGroup;

    public GroupChangeRequestDenegationMessage(Availability deniedGroup) {
        super(MessageType.GROUP_CHANGE_REQUEST_DENEGATION);
        this.deniedGroup = deniedGroup;
    }

    public Availability getDeniedGroup() {
        return this.deniedGroup;
    }
}
