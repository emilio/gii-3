with Text_IO;

procedure StringToInt is
  var: String(1..50);
  len: Natural;
  result: Integer;
  package Integer_IO is new Text_IO.Integer_IO(Integer);
begin
  Text_IO.Put("Gimme a number: ");
  Text_IO.Get_Line(var, len);
  result := 2 * Integer'Value(var(1..len));
  Text_IO.Put("Here is your number doubled: ");
  Text_IO.Put_line(Integer'Image(result));
end StringToInt;
