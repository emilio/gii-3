package behaviours.alt;

import agents.Alumn;
import jade.core.AID;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.UnreadableException;
import messages.GroupChangeRequestConfirmationMessage;
import messages.GroupChangeRequestDenegationMessage;
import messages.GroupChangeRequestMessage;
import messages.Message;
import messages.TeacherGroupChangeMessage;
import messages.TeacherGroupChangeRequestMessage;
import messages.TerminationConfirmationMessage;

public class AlumnBehaviour extends CyclicBehaviour {
    private static final long serialVersionUID = 6681845965215134904L;

    private final Alumn alumn;

    /** To avoid sending multiple times the same request */
    private int pendingRepliesFromAlumns;
    private boolean pendingReplyFromTeacher;
    private final int otherAlumns;

    public AlumnBehaviour(Alumn alumn) {
        super(alumn);
        this.alumn = alumn;
        this.pendingRepliesFromAlumns = 0;
        this.otherAlumns = alumn.findAgentsByType("alumn").length - 1;
        this.pendingReplyFromTeacher = false;
    }

    @Override
    public void action() {
        assert this.alumn.getCurrentAssignedGroup() != null;
        // Send a message to the other alumns if we aren't happy with our curent
        // group And we don't have pending replies, neither from the alumns nor
        // from the teacher
        if (!this.alumn.isAvailableForCurrentAssignedGroup() && this.pendingRepliesFromAlumns == 0
                && !this.pendingReplyFromTeacher) {
            this.pendingRepliesFromAlumns = this.otherAlumns;
            System.err.println("INFO: [" + this.myAgent.getAID()
                    + "] Requesting group changes to all alumns");
            this.alumn.sendMessageToType("alumn", new GroupChangeRequestMessage(
                    this.alumn.getCurrentAssignedGroup(), this.alumn.getAvailability()));
        } else {
            this.handleIncomingMessages();
        }
    }

    private void handleIncomingMessages() {
        final ACLMessage msg = this.myAgent.blockingReceive();
        final AID sender = msg.getSender();
        Message message = null;

        // Skip messages sent from myself
        if (sender.equals(this.alumn.getAID())) {
            return;
        }

        try {
            message = (Message) msg.getContentObject();
        } catch (final UnreadableException e) {
            System.err.println("WARNING: [" + this.myAgent.getAID()
                    + "] Error receiving message from " + sender);
            e.printStackTrace(System.err);
            return;
        }

        System.err.println("INFO: [" + this.myAgent.getAID() + "] ReceiveMessage ("
                + message.getType() + "). PendingReplies: " + this.pendingRepliesFromAlumns
                + "; FromTeacher: " + this.pendingReplyFromTeacher);

        assert this.pendingRepliesFromAlumns >= 0;

        switch (message.getType()) {
            case TERMINATION_REQUEST:
                this.alumn.sendMessage(sender, new TerminationConfirmationMessage());
                this.alumn.doDelete();
                return;
            case GROUP_CHANGE_REQUEST:
                final GroupChangeRequestMessage groupChangeMessage = (GroupChangeRequestMessage) message;

                // If our group doesn't interest the other agent, just leave
                if (!groupChangeMessage.getDesiredGroups()
                        .contains(this.alumn.getCurrentAssignedGroup())) {
                    System.err.println("INFO: Change not possible from " + this.alumn.getAID()
                            + " to " + sender);
                    this.alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(
                            groupChangeMessage.getGroup()));
                    return;
                }

                // If our situation doesn't get worse
                if (!this.alumn.isAvailableForCurrentAssignedGroup()
                        || this.alumn.getAvailability().contains(groupChangeMessage.getGroup())) {
                    System.err.println("INFO: Trying to confirm change from " + this.alumn.getAID()
                            + " to " + sender);
                    this.alumn.sendMessage(sender, new GroupChangeRequestConfirmationMessage(
                            groupChangeMessage.getGroup(), this.alumn.getCurrentAssignedGroup()));
                } else {
                    this.alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(
                            groupChangeMessage.getGroup()));
                }
                return;
            case GROUP_CHANGE_REQUEST_DENEGATION:
                final GroupChangeRequestDenegationMessage denegationMessage = (GroupChangeRequestDenegationMessage) message;
                System.err.println("INFO[" + this.alumn.getAID()
                        + "]: Received denegation message, ignoring...");

                if (denegationMessage.getDeniedGroup() != this.alumn.getCurrentAssignedGroup()) {
                    System.err.println("INFO: Received outdated denegation message, ignoring...");
                    return;
                }
                this.pendingRepliesFromAlumns -= 1;

                return;
            case GROUP_CHANGE_REQUEST_CONFIRMATION:
                final GroupChangeRequestConfirmationMessage confirmationMessage = (GroupChangeRequestConfirmationMessage) message;
                System.err.println("INFO[" + this.alumn.getAID()
                        + "] Received group change request confirmation from " + sender);

                if (confirmationMessage.getOldGroup() != this.alumn.getCurrentAssignedGroup()) {
                    System.err.println("INFO: Received outdated confirmation message, ignoring...");
                    return;
                }

                this.pendingRepliesFromAlumns -= 1;
                if (this.pendingReplyFromTeacher) {
                    System.err
                            .println("INFO: Received confirmation message while waiting for teacher, ignoring...");
                    return;
                }

                // Try to make the change with the teacher, note that it can
                // reject it if another request involving this or the previous
                // alumn was found.
                this.alumn.sendMessage(this.alumn.getTeacherService(),
                                       new TeacherGroupChangeRequestMessage(this.alumn.getAID(),
                                               sender, confirmationMessage.getOldGroup(),
                                               confirmationMessage.getNewGroup()));
                this.pendingReplyFromTeacher = true;
                return;
            case TEACHER_GROUP_CHANGE_REQUEST_DENEGATION:
                this.pendingReplyFromTeacher = false;
                this.pendingRepliesFromAlumns = 0;
                return;

            case TEACHER_GROUP_CHANGE:
                final TeacherGroupChangeMessage teacherGroupChangeMessage = (TeacherGroupChangeMessage) message;

                // If we're some of the two parts involved
                if (this.alumn.getAID().equals(teacherGroupChangeMessage.fromAlumn)) {
                    this.pendingReplyFromTeacher = false;
                    assert this.alumn.getCurrentAssignedGroup()
                            .equals(teacherGroupChangeMessage.fromGroup);
                    this.alumn.setCurrentAssignedGroup(teacherGroupChangeMessage.toGroup);
                } else if (this.alumn.getAID().equals(teacherGroupChangeMessage.toAlumn)) {
                    this.pendingReplyFromTeacher = false;
                    assert this.alumn.getCurrentAssignedGroup()
                            .equals(teacherGroupChangeMessage.toGroup);
                    this.alumn.setCurrentAssignedGroup(teacherGroupChangeMessage.fromGroup);
                    // If we're not and we're pending
                } else {
                    // TODO: unblock if blocked, asking both of them if some of
                    // the groups match
                }

                return;

            // TODO
            default:
                System.out.println("ERROR: Unexpected message " + message.getType()
                        + " received in AlumnBehavior. Sender: " + sender + "; Receiver: "
                        + this.alumn.getAID());
                return;
        }
    }
}
