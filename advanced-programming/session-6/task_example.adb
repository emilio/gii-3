with Text_IO;

procedure Task_Example is
  task type Child_Task;
  task body Child_Task is
  begin
    Text_IO.Put_Line("Child");
  end Child_Task;

  a, b: Child_Task;
begin
  Text_IO.Put_Line("Main");
end Task_Example;
