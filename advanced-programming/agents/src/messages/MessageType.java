package messages;

// TODO: Unify the names of the messages. Choose between
// * TerminationRequest...
// * RequestTermination...
public enum MessageType {
    TERMINATION_REQUEST,
    TERMINATION_CONFIRMATION,
    REQUEST_FIRST_ASSIGNATION,
    FIRST_ASSIGNATION,
    REQUEST_GROUP_CHANGE;
}
