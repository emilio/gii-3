package behaviours;

import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;
import jade.core.behaviours.CyclicBehaviour;

import jade.core.AID;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.*;

import messages.*;

import agents.Alumn;

public class AlumnBehaviour extends CyclicBehaviour {
    private Alumn alumn;
    /** To avoid sending multiple times the same request */
    private int pendingRepliesFromAlumns;
    private boolean pendingReplyFromTeacher;
    private int otherAlumns;

    public AlumnBehaviour(Alumn alumn) {
        super(alumn);
        this.alumn = alumn;
        this.pendingRepliesFromAlumns = 0;
        this.otherAlumns = alumn.findAgentsByType("alumn").length - 1;
        this.pendingReplyFromTeacher = false;
    }

    @Override
    public void action() {
        // Send a message to the other alumns if we aren't happy with our curent group
        // And we don't have pending replies, neither from the alumns nor from the teacher
        if ( ! alumn.isAvailableForCurrentAssignedGroup() && this.pendingRepliesFromAlumns == 0 && !this.pendingReplyFromTeacher ) {
            this.pendingRepliesFromAlumns = this.otherAlumns;
            System.err.println("INFO: [" + myAgent.getAID() + "] Requesting group changes to all alumns");
            alumn.sendMessageToType("alumn", new GroupChangeRequestMessage(alumn.getCurrentAssignedGroup(), alumn.getAvailability()));
        } else {
            this.handleIncomingMessages();
        }
    }

    private void handleIncomingMessages() {
        ACLMessage msg = myAgent.blockingReceive();
        AID sender = msg.getSender();
        Message message = null;

        // Skip messages sent from myself
        if ( sender.equals(alumn.getAID()) )
            return;

        try {
            message = (Message) msg.getContentObject();
        } catch (UnreadableException e) {
            System.err.println("WARNING: [" + myAgent.getAID() + "] Error receiving message from " + sender);
            e.printStackTrace(System.err);
            return;
        }

        System.err.println("INFO: [" + myAgent.getAID() + "] ReceiveMessage (" + message.getType() + "). PendingReplies: " + this.pendingRepliesFromAlumns + "; FromTeacher: " + this.pendingReplyFromTeacher);

        assert(this.pendingRepliesFromAlumns >= 0);

        switch ( message.getType() ) {
            case TERMINATION_REQUEST:
                alumn.sendMessage(sender, new TerminationConfirmationMessage());
                alumn.doDelete();
                return;
            case GROUP_CHANGE_REQUEST:
                GroupChangeRequestMessage groupChangeMessage = (GroupChangeRequestMessage) message;

                // If our group doesn't interest the other agent, just leave
                if ( !groupChangeMessage.getDesiredGroups().contains(alumn.getCurrentAssignedGroup()) ) {
                    System.err.println("INFO: Change not possible from " + alumn.getAID() + " to " + sender);
                    alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(groupChangeMessage.getGroup()));
                    return;
                }

                // If our situation doesn't get worse
                if ( !alumn.isAvailableForCurrentAssignedGroup() || alumn.getAvailability().contains(groupChangeMessage.getGroup()) ) {
                    System.err.println("INFO: Trying to confirm change from " + alumn.getAID() + " to " + sender);
                    alumn.sendMessage(sender, new GroupChangeRequestConfirmationMessage(groupChangeMessage.getGroup(), alumn.getCurrentAssignedGroup()));
                } else {
                    alumn.sendMessage(sender, new GroupChangeRequestDenegationMessage(groupChangeMessage.getGroup()));
                }
                return;
            case GROUP_CHANGE_REQUEST_DENEGATION:
                GroupChangeRequestDenegationMessage denegationMessage = (GroupChangeRequestDenegationMessage) message;
                System.err.println("INFO[" + alumn.getAID() + "]: Received denegation message, ignoring...");

                if ( denegationMessage.getDeniedGroup() != alumn.getCurrentAssignedGroup() ) {
                    System.err.println("INFO: Received outdated denegation message, ignoring...");
                    return;
                }
                this.pendingRepliesFromAlumns -= 1;

                return;
            case GROUP_CHANGE_REQUEST_CONFIRMATION:
                GroupChangeRequestConfirmationMessage confirmationMessage = (GroupChangeRequestConfirmationMessage) message;
                System.err.println("INFO[" + alumn.getAID() + "] Received group change request confirmation from " + sender);

                if ( confirmationMessage.getOldGroup() != alumn.getCurrentAssignedGroup() ) {
                    System.err.println("INFO: Received outdated confirmation message, ignoring...");
                    return;
                }

                this.pendingRepliesFromAlumns -= 1;
                if ( this.pendingReplyFromTeacher ) {
                    System.err.println("INFO: Received confirmation message while waiting for teacher, ignoring...");
                    return;
                }

                // Try to make the change with the teacher, note that it can reject it if another request involving this or the previous alumn was found.
                alumn.sendMessage(alumn.getTeacherService(), new TeacherGroupChangeRequestMessage(alumn.getAID(), sender, confirmationMessage.getOldGroup(), confirmationMessage.getNewGroup()));
                this.pendingReplyFromTeacher = true;
                return;
            case TEACHER_GROUP_CHANGE_REQUEST_DENEGATION:
                this.pendingReplyFromTeacher = false;
                this.pendingRepliesFromAlumns = 0;
                return;

            case TEACHER_GROUP_CHANGE:
                this.pendingReplyFromTeacher = false;
                this.pendingRepliesFromAlumns = 0;

                TeacherGroupChangeMessage teacherGroupChangeMessage = (TeacherGroupChangeMessage) message;

                assert(alumn.getCurrentAssignedGroup().equals(teacherGroupChangeMessage.fromGroup));
                alumn.setCurrentAssignedGroup(teacherGroupChangeMessage.toGroup);
                return;

            // TODO
            default:
                System.out.println("ERROR: Unexpected message " + message.getType() + " received in AlumnBehavior. Sender: " + sender + "; Receiver: " + alumn.getAID());
                return;
        }
    }
}
