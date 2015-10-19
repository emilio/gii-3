with Text_IO;

procedure RangeExample is
  type Note is new Integer range 0..10;
  package Note_IO is new Text_IO.Integer_IO(Note);

  note_var: Note;
begin
  Text_IO.Put("Give me a note (0..10): ");
  Note_IO.Get(note_var);

  Text_IO.Put("The note is: ");
  Note_IO.Put(note_var);
end RangeExample;
