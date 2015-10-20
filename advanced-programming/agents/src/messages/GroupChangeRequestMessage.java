package messages;

import java.util.EnumSet;

import agents.Availability;

public class GroupChangeRequestMessage extends AlumnToAlumnMessage {
    private static final long serialVersionUID = 4816331921277682167L;
    private final Availability group;
    private final EnumSet<Availability> desiredGroups;

    public GroupChangeRequestMessage(Availability currentGroup,
            EnumSet<Availability> desiredGroups) {
        super(MessageType.GROUP_CHANGE_REQUEST);
        assert!desiredGroups.contains(currentGroup);
        this.group = currentGroup;
        this.desiredGroups = desiredGroups;
    }

    public Availability getGroup() {
        return this.group;
    }

    public EnumSet<Availability> getDesiredGroups() {
        return this.desiredGroups;
    }
}
