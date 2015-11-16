
package ControllerPkg is
  protected type Controller is
    entry beginRead;
    entry endRead;
    entry beginWrite;
    entry endWrite;
  private
    readerCount: Integer := 0;
    writing: Boolean := false;
  end Controller;
end ControllerPkg;
