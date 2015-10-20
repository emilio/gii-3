package messages;

public class TerminationRequestMessage extends TeacherToAlumnMessage {
    private static final long serialVersionUID = -2455197631666403456L;

    public TerminationRequestMessage() {
        super(MessageType.TERMINATION_REQUEST);
    }
}
