import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.domain.FIPAException;
import jade.content.lang.sl.SLCodec;
import jade.domain.DFService;

import ec.behaviours.MessagingParallelBehaviour;

public class Agent extends jade.core.Agent {
    private MessagingParallelBehaviour behaviour;

    protected void setup() {
        DFAgentDescription dfd = new DFAgentDescription();
        dfd.setName(this.getAID());

        ServiceDescription sd = new ServiceDescription();
        sd.setName("chat");
        sd.setType("chat");
        sd.addOntologies("null-ontology");
        sd.addLanguages(new SLCodec().getName());

        dfd.addServices(sd);

        try {
            DFService.register(this, dfd);
        } catch (FIPAException e) {
            System.err.println("Fuck it");
        }

        this.behaviour = new MessagingParallelBehaviour(this);
        this.addBehaviour(this.behaviour);

        System.out.println("Hey from agent " + this.getAID() + "!");
    }
}
