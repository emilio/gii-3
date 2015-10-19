with Text_IO;

procedure LoopThroughEnumerate is
  type Week is (monday, tuesday, wednesday, thursday, friday, saturday, sunday);
  current: Week;
begin
  current := Week'First;

  loop
    Text_IO.Put_Line(Week'Image(current));

    exit when current = Week'last;
    current := Week'Succ(current);
  end loop;
end LoopThroughEnumerate;

