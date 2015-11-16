with Text_IO;

package body ControllerPkg is
  protected body Controller is
    entry beginRead
      when writing = false is
    begin
      Text_IO.Put_Line("beginRead");
      -- Text_IO.Put_Line("beginRead: " &beginRead'Count'Img);
      -- Text_IO.Put_Line("beginWrite: " &beginWrite'Count'Img);
      readerCount := readerCount + 1;
    end beginRead;

    entry endRead
      when writing = false is
    begin
      Text_IO.Put_Line("endRead");
      readerCount := readerCount - 1;
    end endRead;

    entry beginWrite
      when writing = false and readerCount = 0 is
    begin
      Text_IO.Put_Line("beginWrite");
      -- Text_IO.Put_Line("beginRead: " &beginRead'Count'Img);
      -- Text_IO.Put_Line("beginWrite: " &beginWrite'Count'Img);
      writing := true;
    end beginWrite;

    entry endWrite
      when writing = true is
    begin
      Text_IO.Put_Line("endWrite");
      writing := false;
    end endWrite;
  end Controller;
end ControllerPkg;
