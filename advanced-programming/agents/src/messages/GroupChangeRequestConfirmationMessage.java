package messages;

import agents.Availability;

public class GroupChangeRequestConfirmationMessage extends AlumnToAlumnMessage {
    private Availability oldGroup;
    private Availability newGroup;

    public GroupChangeRequestConfirmationMessage(Availability oldGroup, Availability newGroup) {
        super(MessageType.GROUP_CHANGE_REQUEST_CONFIRMATION);
        assert(!oldGroup.equals(newGroup));
        this.oldGroup = oldGroup;
        this.newGroup = newGroup;
    }

    public Availability getOldGroup() {
        return this.oldGroup;
    }

    public Availability getNewGroup() {
        return this.newGroup;
    }
}
