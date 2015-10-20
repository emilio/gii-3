package messages;

import agents.Availability;

public class GroupChangeRequestConfirmationMessage extends AlumnToAlumnMessage {
    private static final long serialVersionUID = 9109289653803158533L;
    private final Availability oldGroup;
    private final Availability newGroup;

    public GroupChangeRequestConfirmationMessage(Availability oldGroup, Availability newGroup) {
        super(MessageType.GROUP_CHANGE_REQUEST_CONFIRMATION);
        assert!oldGroup.equals(newGroup);
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
