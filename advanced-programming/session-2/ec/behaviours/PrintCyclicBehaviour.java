package ec.behaviours;

import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;
import jade.core.behaviours.CyclicBehaviour;

import jade.core.Agent;

public class PrintCyclicBehaviour extends CyclicBehaviour {
    public PrintCyclicBehaviour(Agent agent) {
        super(agent);
    }

    @Override
    public void action() {
        ACLMessage msg = myAgent.blockingReceive(
                            MessageTemplate.and(
                                MessageTemplate.MatchPerformative(ACLMessage.REQUEST),
                                MessageTemplate.MatchOntology("null-ontology")
                            ));

        try {
            System. out.println("recv: "+ (String)msg.getContentObject());
        } catch (UnreadableException e) {
            e.printStackTrace();
        }
    }
}
