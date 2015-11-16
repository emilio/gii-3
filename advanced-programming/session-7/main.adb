with Text_IO;
with controllerpkg;
use controllerpkg;

procedure main is
  task type Reader;
  task type Writer;
  m_controller: Controller;

  task body Reader is
  begin
    m_controller.beginRead;
    Text_IO.Put_Line("Reading");
    delay 3.0;
    m_controller.endRead;
  end Reader;

  task body Writer is
  begin
    m_controller.beginWrite;
    Text_IO.Put_Line("Writing");
    m_controller.endWrite;
  end Writer;

  type Reader_Array is Array(1..10) of Reader;
  type Writer_Array is Array(1..10) of Writer;

  procedure Run is
    readers: Reader_Array;
    writers: Writer_Array;
  begin
    null;
  end Run;
begin
  Run;
  Text_IO.Put_Line("End");
end main;
