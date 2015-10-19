package messages;

import java.util.EnumSet;

import agents.Availability;

public class GroupChangeRequestMessage extends AlumnToAlumnMessage {
    private Availability group;
    private EnumSet<Availability> desiredGroups;

    public GroupChangeRequestMessage(Availability currentGroup, EnumSet<Availability> desiredGroups) {
        super(MessageType.GROUP_CHANGE_REQUEST);
        assert(!desiredGroups.contains(currentGroup));
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
