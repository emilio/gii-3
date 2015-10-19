package messages;

// TODO: Unify the names of the messages. Choose between
// * TerminationRequest...
// * RequestTermination...
public enum MessageType {
    TERMINATION_REQUEST,
    TERMINATION_CONFIRMATION,
    FIRST_ASSIGNATION_REQUEST,
    FIRST_ASSIGNATION,
    GROUP_CHANGE_REQUEST,
    GROUP_CHANGE_REQUEST_CONFIRMATION;
}
