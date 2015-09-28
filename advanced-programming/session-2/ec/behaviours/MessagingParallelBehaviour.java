package ec.behaviours;

import jade.core.behaviours.ParallelBehaviour;
import jade.core.behaviours.CyclicBehaviour;
import jade.core.behaviours.ThreadedBehaviourFactory;

import jade.core.Agent;

public class MessagingParallelBehaviour extends ParallelBehaviour {
    private SendOneShotBehaviour sendBehaviour;
    private PrintCyclicBehaviour printBehaviour;

    public MessagingParallelBehaviour(Agent agent) {
        super();

        this.sendBehaviour = new SendOneShotBehaviour(agent);
        this.printBehaviour = new PrintCyclicBehaviour(agent);

        ThreadedBehaviourFactory tbf = new ThreadedBehaviourFactory();
        this.addSubBehaviour(tbf.wrap(this.printBehaviour));
        this.addSubBehaviour(tbf.wrap(this.sendBehaviour));
    }
}
