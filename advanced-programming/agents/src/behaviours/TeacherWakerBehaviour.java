package behaviours;

import java.util.Map;

import agents.Availability;
import agents.Teacher;
import jade.content.lang.Codec;
import jade.content.lang.Codec.CodecException;
import jade.content.lang.sl.SLCodec;
import jade.content.onto.Ontology;
import jade.content.onto.OntologyException;
import jade.content.onto.basic.Action;
import jade.core.AID;
import jade.core.behaviours.WakerBehaviour;
import jade.domain.JADEAgentManagement.JADEManagementOntology;
import jade.domain.JADEAgentManagement.ShutdownPlatform;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;

import messages.Message;
import messages.MessageType;
import messages.TerminationRequestMessage;

public class TeacherWakerBehaviour extends WakerBehaviour {
    private static final long serialVersionUID = -1905233731001836139L;
    private final Teacher teacher;

    public TeacherWakerBehaviour(Teacher teacher, long timeout) {
        super(teacher, timeout);
        this.teacher = teacher;
    }

    @Override
    protected void onWake() {
        int pendingReplies = this.teacher.findAgentsByType("alumn").length;
        this.teacher.sendMessageToType("alumn", new TerminationRequestMessage());

        while (pendingReplies-- > 0) {
            ACLMessage aclMessage = this.teacher.blockingReceive();
            try {
                Message msg = (Message) aclMessage.getContentObject();
                assert msg.getType() == MessageType.TERMINATION_CONFIRMATION;
            } catch (UnreadableException ex) {
            }
        }

        System.err.println("\n\n\n========================================");
        System.err.println("Ended");
        System.err.println("========================================\n\n");

        for (final Map.Entry<AID, Availability> entry : this.teacher.getGroups().entrySet()) {
            final AID agent = entry.getKey();
            final Availability group = entry.getValue();
            System.err.println(" * " + agent.getLocalName() + ": " + group);
        }

        final Codec codec = new SLCodec();
        final Ontology jmo = JADEManagementOntology.getInstance();
        this.teacher.getContentManager().registerLanguage(codec);
        this.teacher.getContentManager().registerOntology(jmo);
        final ACLMessage msg = new ACLMessage(ACLMessage.REQUEST);
        msg.addReceiver(this.teacher.getAMS());
        msg.setLanguage(codec.getName());
        msg.setOntology(jmo.getName());

        try {
            this.teacher.getContentManager()
                    .fillContent(msg, new Action(this.teacher.getAID(), new ShutdownPlatform()));
            this.teacher.send(msg);
        } catch (OntologyException | CodecException ex) {
            System.err.println("ERROR: Couldn't shutdown platform");
            ex.printStackTrace(System.err);
        }

        this.teacher.doDelete();
    }
}
