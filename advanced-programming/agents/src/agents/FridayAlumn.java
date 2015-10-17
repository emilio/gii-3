package agents;

import java.util.EnumSet;

public class FridayAlumn extends Alumn {
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.FRIDAY);
    }
}
