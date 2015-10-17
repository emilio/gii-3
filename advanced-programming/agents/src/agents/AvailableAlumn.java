package agents;

import java.util.EnumSet;

public class AvailableAlumn extends Alumn {
    public EnumSet<Availability> getAvailability() {
        return Availability.ALL;
    }
}
