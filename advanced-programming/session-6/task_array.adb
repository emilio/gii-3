with Text_IO;

procedure Task_Array is
  Counter: Integer;

  task type Child_Task;

  task body Child_Task is
  begin
    for i in 1..100000000 loop
      Counter := Counter + 1;
    end loop;
  end Child_Task;

  type Child_Array is Array(1..3) of Child_Task;

  procedure Run_Tasks is
    Tasks: Child_Array;
  begin
    null;
  end Run_Tasks;
begin
  Run_Tasks;
  Text_IO.Put_Line(Counter'Img);
end Task_Array;
