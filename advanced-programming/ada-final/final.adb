with Ada.Text_IO;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Numerics.Discrete_Random;
with Nuclear_Central;

procedure Final is
  -- Some shared constants here
  CENTRAL_MAX_PRODUCTION: constant Integer := 30;
  CENTRAL_MIN_PRODUCTION: constant Integer := 0;
  CITY_MAX_REQUIREMENTS: constant Integer := 90;
  CITY_MIN_REQUIREMENTS: constant Integer := 15;

  -- This is the atomic var used to update the city requirements:
  -- The `CityUpdater` task sets it, and the main task reads it.
  requirements: Integer := 35;
  pragma Volatile(requirements);
  pragma Atomic(requirements);

  -- The `CityUpdater` task represents a city, and updates the requirements
  -- once per second.
  task type CityUpdater is
  end CityUpdater;

  task body CityUpdater is
    subtype RngRange is Integer range -3..3;
    package Random is new Ada.Numerics.Discrete_Random(RngRange);
    seed: Random.Generator;
    current_operation: Ada.Real_Time.Time;
    current_requirements: Integer;
  begin
    Random.reset(seed);

    current_operation := Ada.Real_Time.Clock;
    loop
      current_requirements := requirements;
      current_requirements := current_requirements + Random.random(seed);
      if current_requirements > CITY_MAX_REQUIREMENTS then
        current_requirements := CITY_MAX_REQUIREMENTS;
      end if;
      if current_requirements < CITY_MIN_REQUIREMENTS then
        current_requirements := CITY_MIN_REQUIREMENTS;
      end if;
      requirements := current_requirements;
      current_operation := current_operation + Ada.Real_Time.Seconds(1);
      delay until current_operation;
    end loop;
  end CityUpdater;

  centrals: Array(1..3) of Nuclear_Central.Nuclear_Central;
  centrals_used: Array(1..3) of Boolean;
  current_values: Array(1..3) of Integer;
  city: CityUpdater;
  city_requirements: Integer;

  min_value: Integer;
  min_value_index: Integer;
  max_value: Integer;
  max_value_index: Integer;

  production: Integer;
  expected_production: Integer;

  current_operation: Ada.Real_Time.Time;
  ratio: Float;
begin
  current_operation := Ada.Real_Time.Clock;

  for i in centrals'Range loop
    centrals(i).start;
  end loop;

  loop
    city_requirements := requirements;

    -- Read the current values
    for i in centrals'Range loop
      centrals(i).read_value(current_values(i));
    end loop;

    -- Sum the current values to get the actual production
    production := 0;
    for i in current_values'Range loop
      production := production + current_values(i);
    end loop;

    -- Calculate the current production / requirements ratio
    ratio := Float(production - city_requirements) / Float(city_requirements);
    if ratio < -0.05 then
      Ada.Text_IO.Put("PELIGRO BAJADA");
    elsif ratio > 0.05 then
      Ada.Text_IO.Put("PELIGRO SOBRECARGA");
    else
      Ada.Text_IO.Put("ESTABLE");
    end if;

    Ada.Text_IO.Put_Line(" consumo:" & city_requirements'Img & " producción:" & production'Img);

    for i in centrals_used'Range loop
      centrals_used(i) := false;
    end loop;

    -- TODO: This could probably be more efficient (and elegant) with a sorted
    -- dequeue, but doing that deserves a big time investment so...
    expected_production := production;
    for i in current_values'Range loop
      if expected_production < city_requirements then
        min_value := CENTRAL_MAX_PRODUCTION;
        min_value_index := -1;
        for j in current_values'Range loop
          if current_values(j) <= min_value and centrals_used(j) = false then
            min_value_index := j;
            min_value := current_values(j);
          end if;
        end loop;

        if min_value_index /= -1 then
          Ada.Text_IO.Put_Line("SUBO " & min_value_index'Img);
          expected_production := expected_production + 1;
          centrals(min_value_index).increment;
          centrals_used(min_value_index) := true;
        else
          Ada.Text_IO.Put_Line("Central to increment not found. This is an error.");
        end if;
      elsif expected_production > city_requirements then
        max_value := CENTRAL_MIN_PRODUCTION;
        max_value_index := -1;

        for j in current_values'Range loop
          if current_values(j) >= max_value and centrals_used(j) = false then
            max_value_index := j;
            max_value := current_values(j);
          end if;
        end loop;

        if max_value_index /= -1 then
          Ada.Text_IO.Put_Line("BAJO " & max_value_index'Img);
          expected_production := expected_production - 1;
          centrals(max_value_index).decrement;
          centrals_used(max_value_index) := true;
        else
          Ada.Text_IO.Put_Line("Central to decrement not found. This is an error.");
        end if;
      end if;
    end loop;

    for i in centrals_used'Range loop
      if centrals_used(i) = false then
        centrals(i).maintain;
      end if;
    end loop;

    current_operation := current_operation + Ada.Real_Time.Milliseconds(300);
    delay until current_operation;
  end loop;
end Final;
