package agents;

import behaviours.TeacherBehaviour;
import jade.domain.FIPAException;

public class Teacher extends SimpleAgent {
    private TeacherBehaviour behaviour;

    protected void setup() {
        try {
            this.simpleSetup("teacher");
        } catch (FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        this.behaviour = new TeacherBehaviour(this);
        this.addBehaviour(this.behaviour);

        System.out.println("Teacher agent " + this.getAID() + " started");
    }
}
