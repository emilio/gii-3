package agents;

import java.util.EnumSet;

public class FridayAlumn extends Alumn {
    private static final long serialVersionUID = -3818816184148432238L;

    @Override
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.FRIDAY);
    }
}
