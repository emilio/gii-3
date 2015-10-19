with Text_IO;

procedure EvenNumbers is
  current: Integer := 0;
  sum: Integer := 0;
begin
  loop
    exit when current > 100;
    sum := sum + current;
    current := current + 2;
  end loop;

  Text_IO.Put("Here's the sum of the even numbers until 100: ");
  Text_IO.Put_Line(Integer'Image(sum));
end EvenNumbers;
