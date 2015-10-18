package messages;

public class TerminationRequestMessage extends TeacherToAlumnMessage {
    public TerminationRequestMessage() {
        super(MessageType.TERMINATION_REQUEST);
    }
}
