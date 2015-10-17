// TODO: Move this to another package
package agents;

import java.util.EnumSet;

public enum Availability {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURDSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY;

    public static final EnumSet<Availability> ALL = EnumSet.allOf(Availability.class);
    public static final EnumSet<Availability> NONE = EnumSet.noneOf(Availability.class);
}
