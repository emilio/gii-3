with Ada.Text_IO;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Numerics.Discrete_Random;

procedure Final is
  -- The `Monitor` task is responsible of reading the NuclearCentral
  -- values and manage them

  -- The `NuclearCentral` task represents a nuclear central that can
  -- increment, decrement or keep the energy production
  task type NuclearCentral is
    entry start(the_id: in Integer);
    entry read_value(value: out Integer);
    entry increment;
    entry decrement;
    entry stop;
  end NuclearCentral;

  task body NuclearCentral is
    current_production: Integer;
    id: Integer;
    last_operation_time: Time := Clock;
    current_operation_time: Time;
  begin
    current_production := 0;

    -- First wait for the start message
    select
      accept start(the_id: in Integer) do
        Ada.Text_IO.put_line("Start: " & the_id'Img);
        id := the_id;
      end start;
    end select;

    -- Now enter in the main loop
    loop
      -- Ada.Text_IO.put("Nuclear Central" & id'Img & ": ");
      select
        accept read_value(value: out Integer) do
          -- Artificial delay required by the statement
          delay 0.05;
          value := current_production;
        end read_value;
        or
        accept increment;
          current_operation_time := Ada.Real_Time.Clock;
          -- This operation can be done just once per second
          if current_operation_time - last_operation_time >= Ada.Real_Time.Seconds(1) then
            last_operation_time := current_operation_time;
            current_production := current_production + 1;
          end if;
        or
        accept decrement;
          current_operation_time := Ada.Real_Time.Clock;
          -- This operation can be done just once per second
          if current_operation_time - last_operation_time >= Ada.Real_Time.Seconds(1) then
            current_operation_time := Ada.Real_Time.Clock;
            current_production := current_production - 1;
          end if;
        or
        accept stop;
          Ada.Text_IO.put_line("Stop");
          exit;
      end select;
    end loop;
  end NuclearCentral;

  subtype RngRange is Integer range -3..3;
  package Random is new Ada.Numerics.Discrete_Random(RngRange);
  seed: Random.Generator;

  city_requirements: Integer;
  centrals: Array(1..3) of NuclearCentral;
  current_values: Array(1..3) of Integer;
  iterations: Integer;
  production: Integer;
  expected_production: Integer;
  ratio: Float;
begin
  iterations := 0;
  city_requirements := 45;
  Random.reset(seed);

  for i in centrals'Range loop
    centrals(i).start(i);
  end loop;

  loop
    -- Update the city requirements if appropiate
    if iterations rem 3 = 0 then
      city_requirements := city_requirements + Random.random(seed);
      if city_requirements > 90 then
        city_requirements := 90;
      end if;
      if city_requirements < 15 then
        city_requirements := 15;
      end if;
      -- Ada.Text_IO.put_line("Updating city requirements: " & city_requirements'Img);
    end if;

    -- Read the current values
    for i in centrals'Range loop
      centrals(i).read_value(current_values(i));
      -- Ada.Text_IO.put_line("Current value for " & i'Img & ": " & current_values(i)'Img);
    end loop;

    -- Sum the current values to get the actual production
    production := 0;
    for i in current_values'Range loop
      production := production + current_values(i);
    end loop;

    -- Calculate the current production / requirements ratio
    ratio := Float(production - city_requirements) / Float(city_requirements);

    -- Ada.Text_IO.put_line("Ratio: " & ratio'Img);
    if ratio < -0.05 then
      Ada.Text_IO.put_line("PELIGRO BAJADA consumo:" & city_requirements'Img & " producción " & production'Img);
    elsif ratio > 0.05 then
      Ada.Text_IO.put_line("PELIGRO SOBRECARGA consumo:" & city_requirements'Img & " producción " & production'Img);
    else
      Ada.Text_IO.put_line("ESTABLE consumo:" & city_requirements'Img & " producción " & production'Img);
    end if;

    -- Calculate the appropiate movements to get the things working
    -- TODO: This could be better distributed, this "overcharges" the first
    -- centrals
    expected_production := production;
    for i in current_values'Range loop
      if expected_production < city_requirements then
        Ada.Text_IO.put_line("SUBO " & i'Img);
        expected_production := expected_production + 1;
        centrals(i).increment;
      elsif expected_production > city_requirements and ratio > 0.05 then
        Ada.Text_IO.put_line("BAJO " & i'Img);
        expected_production := expected_production - 1;
        centrals(i).decrement;
      end if;
    end loop;

    iterations := iterations + 1;
    delay 0.3;
  end loop;
end Final;
