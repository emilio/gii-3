package agents;

import java.util.HashMap;

import behaviours.TeacherBehaviour;
import behaviours.TeacherWakerBehaviour;
import jade.core.AID;
import jade.core.behaviours.ParallelBehaviour;
import jade.core.behaviours.WakerBehaviour;
import jade.domain.FIPAException;

public class Teacher extends SimpleAgent {
    private static final long serialVersionUID = -8623682538983889708L;
    private TeacherBehaviour teacherBehaviour;
    private WakerBehaviour wakerBehaviour;
    private ParallelBehaviour parallelBehaviour;

    public HashMap<AID, Availability> getGroups() {
        return this.teacherBehaviour.getGroups();
    }

    @Override
    protected void setup() {
        try {
            this.simpleSetup("teacher");
        } catch (final FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        this.teacherBehaviour = new TeacherBehaviour(this);
        this.wakerBehaviour = new TeacherWakerBehaviour(this, 10000);

        this.parallelBehaviour = new ParallelBehaviour(this, ParallelBehaviour.WHEN_ANY);
        this.parallelBehaviour.addSubBehaviour(this.wakerBehaviour);
        this.parallelBehaviour.addSubBehaviour(this.teacherBehaviour);
        this.addBehaviour(this.parallelBehaviour);

        System.err.println("Teacher agent " + this.getAID() + " started");
    }
}
