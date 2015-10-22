package agents;

import java.util.EnumSet;

import behaviours.AlumnBehaviour;
import jade.core.AID;
import jade.domain.FIPAException;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;
import messages.FirstAssignationMessage;
import messages.FirstAssignationRequestMessage;
import messages.Message;
import messages.MessageType;

public abstract class Alumn extends SimpleAgent {
    private static final long serialVersionUID = 7370466273846708748L;

    public abstract EnumSet<Availability> getAvailability();

    // TODO: s/Availability/Group/g
    private Availability currentAssignedGroup;
    private AID teacherService;
    private AlumnBehaviour behaviour;

    public Availability getCurrentAssignedGroup() {
        return this.currentAssignedGroup;
    }

    public void setCurrentAssignedGroup(Availability group) {
        this.currentAssignedGroup = group;
    }

    public AID getTeacherService() {
        return this.teacherService;
    }

    public boolean isAvailableForCurrentAssignedGroup() {
        return this.getAvailability().contains(this.currentAssignedGroup);
    }

    @Override
    protected void setup() {
        try {
            this.simpleSetup("alumn");
        } catch (final FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        this.behaviour = new AlumnBehaviour(this);
        this.addBehaviour(this.behaviour);

        System.out.println("Alumn agent " + this.getAID() + " started");

        this.currentAssignedGroup = null;

        this.teacherService = this.getService("teacher");

        if (this.teacherService == null) {
            System.err.println("Teacher agent not found, aborting...");
            this.doDelete();
            return;
        }

        this.sendMessage(this.teacherService, new FirstAssignationRequestMessage());
        System.out.println("Requested first assignment to " + this.teacherService);

        final ACLMessage msg = this
                .blockingReceive(MessageTemplate.MatchSender(this.teacherService));

        try {
            final FirstAssignationMessage response = (FirstAssignationMessage) msg
                    .getContentObject();
            this.currentAssignedGroup = response.getGroup();
        } catch (final UnreadableException ex) {
            System.err.println("W.T.F");
            ex.printStackTrace(System.err);
        }

        System.out.println("Alumn " + this.getAID() + " created. Assigned: "
                + this.currentAssignedGroup);

        final ACLMessage initMsg = this
                .blockingReceive(MessageTemplate.MatchSender(this.teacherService));

        try {
            final Message initMessage = (Message) initMsg.getContentObject();
            assert initMessage.getType() == MessageType.INIT;
        } catch (UnreadableException ex) {
        }
    }
}
