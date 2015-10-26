package behaviours;

import java.util.EnumSet;

import agents.Availability;
import agents.Alumn;

import jade.core.AID;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.UnreadableException;

import messages.GroupChangeRequestConfirmationDenegationMessage;
import messages.GroupChangeRequestConfirmationMessage;
import messages.GroupChangeRequestDenegationMessage;
import messages.GroupChangeRequestMessage;
import messages.Message;
import messages.TeacherGroupChangeMessage;
import messages.TeacherGroupChangeRequestMessage;
import messages.TerminationConfirmationMessage;

public class AlumnBehaviour extends CyclicBehaviour {
    private static final long serialVersionUID = 6681845965215134904L;

    // We will ask for changes at most two times
    private static final int MAX_UNSUCCESFUL_REQUESTS = 2;

    private final Alumn alumn;

    /** To avoid sending multiple times the same request */
    private int pendingRepliesFromAlumns;
    private boolean pendingReplyFromTeacher;
    private boolean pendingGroupConfirmationReply;
    private final int otherAlumns;
    private int batchRequestsAlreadyDone;

    public AlumnBehaviour(Alumn alumn) {
        super(alumn);
        this.alumn = alumn;
        this.batchRequestsAlreadyDone = 0;
        this.pendingRepliesFromAlumns = 0;
        this.otherAlumns = alumn.findAgentsByType("alumn").length - 1;
        this.pendingReplyFromTeacher = false;
        this.pendingGroupConfirmationReply = false;
    }

    public boolean pendingReplies() {
        assert this.pendingRepliesFromAlumns >= 0 : this.alumn.getAID().toString();
        return this.pendingRepliesFromAlumns > 0 || this.pendingReplyFromTeacher
                || this.pendingGroupConfirmationReply;
    }

    @Override
    public void action() {
        assert this.alumn.getCurrentAssignedGroup() != null;
        // Send a message to the other alumns iff:
        // - We aren't happy with our curent group
        // - We don't have pending replies, neither from the alumns nor from the
        // teacher
        // - We haven't asked more than MAX_UNSUCCESSFUL_REQUEST times
        if (!this.alumn.isAvailableForCurrentAssignedGroup() && !this.pendingReplies()
                && this.batchRequestsAlreadyDone < MAX_UNSUCCESFUL_REQUESTS) {
            this.pendingRepliesFromAlumns += this.otherAlumns;
            this.batchRequestsAlreadyDone += 1;
            System.err.println("INFO: [" + this.myAgent.getLocalName()
                    + "] Requesting group changes to all alumns");
            // If we've previously requested changes and no-one
            // replied, we try to change the group independently
            if (this.batchRequestsAlreadyDone > 1) {
                EnumSet<Availability> desiredAvailability = Availability.ALL.clone();
                desiredAvailability.remove(this.alumn.getCurrentAssignedGroup());

                this.alumn.sendMessageToType("alumn", new GroupChangeRequestMessage(
                        this.alumn.getCurrentAssignedGroup(), desiredAvailability));
            // If not we just try to stick with our groups
            } else {
                this.alumn.sendMessageToType("alumn", new GroupChangeRequestMessage(
                        this.alumn.getCurrentAssignedGroup(), this.alumn.getAvailability()));
            }
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

        System.err.println("INFO: [" + this.myAgent.getLocalName() + "] ReceiveMessage ("
                + message.getType() + ", " + sender.getLocalName() + ")\n\tPendingReplies: "
                + this.pendingRepliesFromAlumns + "; FromTeacher: " + this.pendingReplyFromTeacher
                + "; RequestsAlreadyDone: " + this.batchRequestsAlreadyDone + "; CurrentGroup: "
                + this.alumn.getCurrentAssignedGroup());

        switch (message.getType()) {
            case TERMINATION_REQUEST:
                this.alumn.sendMessage(sender, new TerminationConfirmationMessage());
                this.alumn.doDelete();
                return;
            case GROUP_CHANGE_REQUEST:
                final GroupChangeRequestMessage groupChangeMessage = (GroupChangeRequestMessage) message;

                // If we are waiting for a change, or our group doesn't interest
                // the other alumn just deny the change
                if (this.pendingReplies() || !groupChangeMessage.getDesiredGroups()
                        .contains(this.alumn.getCurrentAssignedGroup())) {
                    this.alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(
                            groupChangeMessage.getGroup()));
                    return;
                }

                // If our situation doesn't get worse, accept it, else reject it
                if (!this.alumn.isAvailableForCurrentAssignedGroup()
                        || this.alumn.getAvailability().contains(groupChangeMessage.getGroup())) {
                    this.alumn.sendMessage(sender, new GroupChangeRequestConfirmationMessage(
                            groupChangeMessage.getGroup(), this.alumn.getCurrentAssignedGroup()));
                    this.pendingGroupConfirmationReply = true;
                } else {
                    this.alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(
                            groupChangeMessage.getGroup()));
                }
                return;
            // Our confirmation was rejected due to another confirmation
            // arriving before,
            // we just unflag ourselves
            case GROUP_CHANGE_REQUEST_CONFIRMATION_DENEGATION:
                this.pendingGroupConfirmationReply = false;
                return;
            case GROUP_CHANGE_REQUEST_DENEGATION:
                final GroupChangeRequestDenegationMessage denegationMessage = (GroupChangeRequestDenegationMessage) message;

                this.pendingRepliesFromAlumns -= 1;

                if (denegationMessage.getDeniedGroup() != this.alumn.getCurrentAssignedGroup()) {
                    System.err.println("INFO: [" + this.alumn.getLocalName()
                            + "] Received outdated denegation message, ignoring...");
                }

                return;
            case GROUP_CHANGE_REQUEST_CONFIRMATION:
                final GroupChangeRequestConfirmationMessage confirmationMessage = (GroupChangeRequestConfirmationMessage) message;

                this.pendingRepliesFromAlumns -= 1;

                if (confirmationMessage.getOldGroup() != this.alumn.getCurrentAssignedGroup()) {
                    System.err.println("INFO: [" + this.alumn.getLocalName()
                            + "] Received outdated confirmation message, ignoring...");
                    this.alumn.sendMessage(sender,
                                           new GroupChangeRequestConfirmationDenegationMessage());
                    return;
                }

                if (this.pendingReplyFromTeacher) {
                    this.alumn.sendMessage(sender,
                                           new GroupChangeRequestConfirmationDenegationMessage());
                    return;
                }

                // Make the change with the teacher
                this.alumn.sendMessage(this.alumn.getTeacherService(),
                                       new TeacherGroupChangeRequestMessage(this.alumn.getAID(),
                                               sender, confirmationMessage.getOldGroup(),
                                               confirmationMessage.getNewGroup()));
                this.pendingReplyFromTeacher = true;
                return;

            case TEACHER_GROUP_CHANGE:
                final TeacherGroupChangeMessage teacherGroupChangeMessage = (TeacherGroupChangeMessage) message;

                // if the sender isn't the teacher and we're implicated in the
                // change, it's the forwarded message from the other alumn we
                // should ignore
                if (!sender.equals(this.alumn.getTeacherService())
                        && (this.alumn.getAID().equals(teacherGroupChangeMessage.fromAlumn)
                                || this.alumn.getAID().equals(teacherGroupChangeMessage.toAlumn))) {
                    return;
                }

                // If we're some of the two parts involved
                if (this.alumn.getAID().equals(teacherGroupChangeMessage.fromAlumn)) {
                    // Since our group has changed, we might want to ask
                    // everyone again
                    this.batchRequestsAlreadyDone = 0;
                    this.pendingReplyFromTeacher = false;

                    assert this.alumn.getCurrentAssignedGroup()
                            .equals(teacherGroupChangeMessage.fromGroup);

                    this.alumn.setCurrentAssignedGroup(teacherGroupChangeMessage.toGroup);

                    // Forward the message for the other alumns, just in case
                    // they're interested
                    this.alumn.sendMessageToType("alumn", teacherGroupChangeMessage);
                } else if (this.alumn.getAID().equals(teacherGroupChangeMessage.toAlumn)) {
                    // Since our group has changed, we might want to ask
                    // everyone again
                    this.batchRequestsAlreadyDone = 0;
                    this.pendingReplyFromTeacher = false;

                    assert this.alumn.getCurrentAssignedGroup()
                            .equals(teacherGroupChangeMessage.toGroup);

                    this.alumn.setCurrentAssignedGroup(teacherGroupChangeMessage.fromGroup);

                    // Forward the message for the other alumns, just in case
                    // they're interested
                    this.alumn.sendMessageToType("alumn", teacherGroupChangeMessage);
                } else {
                    // If we aren't happy with our group and not waiting for
                    // replies, check if the new groups interest us
                    if (!this.alumn.isAvailableForCurrentAssignedGroup()
                            && !this.pendingReplies()) {
                        // If some of the new groups interest us, send a request
                        // for the new holder of the group
                        if (this.alumn.getAvailability()
                                .contains(teacherGroupChangeMessage.fromGroup)) {
                            this.alumn.sendMessage(teacherGroupChangeMessage.toAlumn,
                                                   new GroupChangeRequestMessage(
                                                           this.alumn.getCurrentAssignedGroup(),
                                                           this.alumn.getAvailability()));
                            this.pendingRepliesFromAlumns += 1;
                        }

                        if (this.alumn.getAvailability()
                                .contains(teacherGroupChangeMessage.toGroup)) {
                            this.alumn.sendMessage(teacherGroupChangeMessage.fromAlumn,
                                                   new GroupChangeRequestMessage(
                                                           this.alumn.getCurrentAssignedGroup(),
                                                           this.alumn.getAvailability()));
                            this.pendingRepliesFromAlumns += 1;
                        }
                    }
                }

                return;
            default:
                System.out.println("ERROR: Unexpected message " + message.getType()
                        + " received in AlumnBehavior. Sender: " + sender + "; Receiver: "
                        + this.alumn.getAID());
                return;
        }
    }
}
