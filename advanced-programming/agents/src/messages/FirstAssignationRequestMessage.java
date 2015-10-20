package messages;

public class FirstAssignationRequestMessage extends AlumnToTeacherMessage {
    private static final long serialVersionUID = 1442096393848706432L;

    public FirstAssignationRequestMessage() {
        super(MessageType.FIRST_ASSIGNATION_REQUEST);
    }
}
