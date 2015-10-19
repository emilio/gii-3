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
    private boolean changeRequestSent;

    public AlumnBehaviour(Alumn alumn) {
        super(alumn);
        this.changeRequestSent = false;
        this.alumn = alumn;
    }

    @Override
    public void action() {
        // Send a message to the other alumns if we aren't happy with our curent group
        if ( ! alumn.isAvailableForCurrentAssignedGroup() && ! changeRequestSent ) {
            alumn.sendMessageToType("alumn", new GroupChangeRequestMessage(alumn.getCurrentAssignedGroup(), alumn.getAvailability()));
            changeRequestSent = true;
        } else {
            this.handleIncomingMessages();
        }
    }

    private void handleIncomingMessages() {
        ACLMessage msg = myAgent.blockingReceive(MessageTemplate.MatchPerformative(ACLMessage.REQUEST));
        AID sender = msg.getSender();
        Message message = null;

        // Skip messages sent from myself
        if ( sender.equals(alumn.getAID()) )
            return;

        try {
            message = (Message) msg.getContentObject();
        } catch (UnreadableException e) {
            System.err.println("[" + myAgent.getAID() + "] Error receiving message from " + sender);
            e.printStackTrace(System.err);
            return;
        }

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
                    return;
                }

                // If our situation doesn't get worse
                if ( !alumn.isAvailableForCurrentAssignedGroup() || alumn.getAvailability().contains(groupChangeMessage.getGroup()) ) {
                    System.err.println("INFO: Trying to confirm change from " + alumn.getAID() + " to " + sender);
                    alumn.sendMessage(sender, new GroupChangeRequestConfirmationMessage(groupChangeMessage.getGroup(), alumn.getCurrentAssignedGroup()));
                }

            // TODO
            default:
                System.out.println("ERROR: Unexpected message " + message.getType() + " received in AlumnBehavior. Sender: " + sender + "; Receiver: " + alumn.getAID());
                return;
        }
    }
}
