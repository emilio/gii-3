with Ada.Real_Time; use Ada.Real_Time;
with Ada.Real_Time.Timing_Events;
with Ada.Text_IO;

package Nuclear_Central is
  SENSOR_JITTER: constant Duration := Ada.Real_Time.To_Duration(Ada.Real_Time.Milliseconds(50));
  CONTROL_JITTER: constant Duration := Ada.Real_Time.To_Duration(Ada.Real_Time.Milliseconds(100));

  protected type Nuclear_Central is
    entry read_value(value: out Integer);
    entry increment;
    entry decrement;
    entry maintain;
    entry start;
  private
    current_production: Integer := 15;
    increment_pending: Boolean := false;
    decrement_pending: Boolean := false;
    started: Boolean := false;
    Increment_Event: Timing_Events.Timing_Event;
    Decrement_Event: Timing_Events.Timing_Event;
    Monitoring_Event: Timing_Events.Timing_Event;
  end Nuclear_Central;
end Nuclear_Central;
