package messages;

import java.util.EnumSet;

import agents.Availability;

public class RequestGroupChangeMessage extends AlumnToAlumnMessage {
    public RequestGroupChangeMessage(Availability currentGroup, EnumSet<Availability> desiredGroups) {
        super(MessageType.REQUEST_GROUP_CHANGE);
        assert(!desiredGroups.contains(currentGroup));
    }
}
