with Text_IO;

procedure WeekDays is
  type WeekDay is (monday, tuesday, wednesday, thursday, friday, saturday, sunday);
  subtype WorkingDay is WeekDay range monday..friday;

  package WeekDay_IO is new Text_IO.Enumeration_IO(WeekDay);
  weekday_var: WeekDay;
begin
  Text_IO.Put("Give me a weekday (monday..sunday): ");
  WeekDay_IO.Get(weekday_var);

  Text_IO.Put("The WeekDay is: ");
  WeekDay_IO.Put(weekday_var);
end WeekDays;
