package agents;

import java.util.EnumSet;

public class SemiFlexibleAlumn extends Alumn {
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.MONDAY,
                          Availability.FRIDAY);
    }
}

