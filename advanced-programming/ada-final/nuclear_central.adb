package body Nuclear_Central is
  protected body Nuclear_Central is
    procedure Monitoring_Alert_Callback
      (event: in out Timing_Events.Timing_Event) is
    begin
      Ada.Text_IO.Put_Line("ALERTA MONITORIZACIÓN DE ENERGÍA");
    end Monitoring_Alert_Callback;

    procedure Reset_Monitoring_Callback is
    begin
      Timing_Events.Set_Handler(
                             Monitoring_Event,
                             Ada.Real_Time.Seconds(2),
                             Monitoring_Alert_Callback'Access);
    end Reset_Monitoring_Callback;

    procedure Increment_Callback
      (event: in out Timing_Events.Timing_Event) is
    begin
      current_production := current_production + 1;
      increment_pending := false;
    end Increment_Callback;

    procedure Decrement_Callback
      (event: in out Timing_Events.Timing_Event) is
    begin
      current_production := current_production - 1;
      decrement_pending := false;
    end Decrement_Callback;

    entry start when started = false is
    begin
      Reset_Monitoring_Callback;
      started := true;
    end start;

    entry read_value(value: out Integer) when started is
    begin
      -- Artificial delay required by the statement
      delay SENSOR_JITTER;
      value := current_production;
    end read_value;

    entry increment when started is
      cancelled: Boolean;
    begin
      Reset_Monitoring_Callback;
      if increment_pending = false then
        -- Artificial delay required
        delay CONTROL_JITTER;

        if decrement_pending then
          Timing_Events.Cancel_Handler(Decrement_Event, cancelled);
          if cancelled = false then
            Ada.Text_IO.Put_Line("Decrement cancellation failed, decrement aborted");
            return;
          end if;
          decrement_pending := false;
        end if;

        Timing_Events.Set_Handler(
                               Increment_Event,
                               Ada.Real_Time.Seconds(1),
                               Increment_Callback'Access);
        increment_pending := true;
      end if;
    end increment;

    entry decrement when started is
      cancelled: Boolean;
    begin
      Reset_Monitoring_Callback;
      if decrement_pending = false then
        -- Artificial delay required
        delay CONTROL_JITTER;

        if increment_pending then
          Timing_Events.Cancel_Handler(Increment_Event, cancelled);
          if cancelled = false then
            Ada.Text_IO.Put_Line("Increment cancellation failed, decrement aborted");
            return;
          end if;
          increment_pending := false;
        end if;

        Timing_Events.Set_Handler(
                               Decrement_Event,
                               Ada.Real_Time.Seconds(1),
                               Decrement_Callback'Access);
        decrement_pending := true;
      end if;
    end decrement;

    entry maintain when started is
      cancelled: Boolean;
    begin
      Reset_Monitoring_Callback;

      if decrement_pending then
        Timing_Events.Cancel_Handler(Decrement_Event, cancelled);
        if cancelled = false then
          Ada.Text_IO.Put_Line("Decrement cancellation failed");
          return;
        end if;
        decrement_pending := false;
      end if;

      if increment_pending then
        Timing_Events.Cancel_Handler(Increment_Event, cancelled);
        if cancelled = false then
          Ada.Text_IO.Put_Line("Increment cancellation failed");
          return;
        end if;
        increment_pending := false;
      end if;
    end maintain;
  end Nuclear_Central;
end Nuclear_Central;
