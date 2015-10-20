package messages;

public class TerminationConfirmationMessage extends AlumnToTeacherMessage {
    private static final long serialVersionUID = -4652838112446728718L;

    public TerminationConfirmationMessage() {
        super(MessageType.TERMINATION_CONFIRMATION);
    }
}
