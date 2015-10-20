package messages;

// TODO: Unify the names of the messages. Choose between
// * TerminationRequest...
// * RequestTermination...
public enum MessageType {
    TERMINATION_REQUEST,
    TERMINATION_CONFIRMATION,
    FIRST_ASSIGNATION_REQUEST,
    FIRST_ASSIGNATION,
    INIT,
    GROUP_CHANGE_REQUEST,
    GROUP_CHANGE_REQUEST_CONFIRMATION,
    GROUP_CHANGE_REQUEST_DENEGATION,
    TEACHER_GROUP_CHANGE_REQUEST,
    TEACHER_GROUP_CHANGE, // This message is sent to both alumns
    TEACHER_GROUP_CHANGE_REQUEST_DENEGATION;
}
