package agents;

import java.util.EnumSet;

public class MondayAlumn extends Alumn {
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.MONDAY);
    }
}
