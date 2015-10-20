package agents;

import java.util.EnumSet;

public class SemiFlexibleAlumn extends Alumn {
    private static final long serialVersionUID = 7149694510285044255L;

    @Override
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.MONDAY, Availability.FRIDAY);
    }
}
