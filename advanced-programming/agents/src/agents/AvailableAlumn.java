package agents;

import java.util.EnumSet;

public class AvailableAlumn extends Alumn {
    private static final long serialVersionUID = 8252983783275128890L;

    @Override
    public EnumSet<Availability> getAvailability() {
        return Availability.ALL;
    }
}
