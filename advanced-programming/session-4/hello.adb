with Text_IO;

procedure Hello is
  var:Natural;

  package Natural_IO is new Text_IO.Integer_IO(Natural);

begin
  Natural_IO.Get(var);
  Natural_IO.Put(var);
end Hello;
