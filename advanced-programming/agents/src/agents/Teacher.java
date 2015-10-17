package agents;

import jade.domain.FIPAException;

public class Teacher extends SimpleAgent {
    protected void setup() {
        try {
            this.simpleSetup("teacher");
        } catch (FIPAException ex) {
            System.err.println("Agent " + this.getAID() + " setup failed!");
            ex.printStackTrace(System.err);
        }

        System.out.println("Teacher agent " + this.getAID() + " started");
    }
}
