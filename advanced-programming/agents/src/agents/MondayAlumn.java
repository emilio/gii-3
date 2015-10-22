package agents;

import java.util.EnumSet;

public class MondayAlumn extends Alumn {
    private static final long serialVersionUID = -198415545845646389L;

    @Override
    public EnumSet<Availability> getAvailability() {
        return EnumSet.of(Availability.MONDAY);
    }
}
