package agents;

import behaviours.TeacherBehaviour;
import jade.domain.FIPAException;
import jade.core.AID;
import jade.lang.acl.MessageTemplate;
import jade.core.behaviours.ParallelBehaviour;
import jade.core.behaviours.WakerBehaviour;

import java.util.Map;

import messages.*;

public class Teacher extends SimpleAgent {
    private TeacherBehaviour teacherBehaviour;
    private WakerBehaviour wakerBehaviour;
    private ParallelBehaviour parallelBehaviour;

    protected void setup() {
        try {
            this.simpleSetup("teacher");
        } catch (FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        this.teacherBehaviour = new TeacherBehaviour(this);
        this.wakerBehaviour = new WakerBehaviour(this, 10000) {
            protected void onWake() {
                System.err.println("Done!!!1111!!!");
                Teacher teacher = (Teacher) myAgent;
                int pendingReplies = teacher.findAgentsByType("alumn").length;

                teacher.sendMessageToType("alumn", new TerminationRequestMessage());

                while ( pendingReplies-- > 0 )
                    teacher.blockingReceive(MessageTemplate.MatchAll());

                System.err.println("\n\n\n========================================");
                System.err.println("Ended");
                System.err.println("========================================\n\n");

                for (Map.Entry<AID, Availability> entry : teacher.teacherBehaviour.getGroups().entrySet()) {
                    AID agent = entry.getKey();
                    Availability group = entry.getValue();
                    System.err.println(" * " + agent.getLocalName() + ": " + group);
                }

                teacher.doDelete();
            }
        };

        this.parallelBehaviour = new ParallelBehaviour(this, ParallelBehaviour.WHEN_ANY);
        parallelBehaviour.addSubBehaviour(this.wakerBehaviour);
        parallelBehaviour.addSubBehaviour(this.teacherBehaviour);
        this.addBehaviour(parallelBehaviour);

        System.err.println("Teacher agent " + this.getAID() + " started");
    }
}
