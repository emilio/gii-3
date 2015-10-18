package agents;

import java.util.EnumSet;

import jade.core.AID;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;
// import jade.core.behaviours.CyclicBehaviour;
import jade.domain.FIPAException;

import messages.*;

abstract class Alumn extends SimpleAgent {
    public abstract EnumSet<Availability> getAvailability();
    private Availability currentAssignedGroup;
    private AID teacherService;

    protected void setup() {
        try {
            this.simpleSetup("alumn");
        } catch (FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        System.out.println("Alumn agent " + this.getAID() + " started");

        this.currentAssignedGroup = null;

        this.teacherService = this.getService("teacher");

        if ( this.teacherService == null ) {
            System.err.println("Teacher agent not found, aborting...");
            this.doDelete();
            return;
        }

        this.sendMessage(this.teacherService, new RequestFirstAssignationMessage());
        System.out.println("Requested first assignment to " + this.teacherService);

        ACLMessage msg = this.blockingReceive(
                             MessageTemplate.and(
                                 MessageTemplate.MatchPerformative(ACLMessage.REQUEST),
                                 MessageTemplate.MatchSender(this.teacherService)));

        try {
            FirstAssignationMessage response = (FirstAssignationMessage) msg.getContentObject();
            this.currentAssignedGroup = response.getGroup();
        } catch (UnreadableException ex) {
            System.err.println("W.T.F");
            ex.printStackTrace(System.err);
        }

        System.out.println("Alumn " + this.getAID() + " created. Assigned: " + this.currentAssignedGroup);
    }
}
