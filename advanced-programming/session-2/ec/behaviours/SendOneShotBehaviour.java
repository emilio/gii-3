package ec.behaviours;

import java.util.Scanner;

import jade.core.behaviours.OneShotBehaviour;
import jade.core.Agent;

import es.usal.pa.Utils;

public class SendOneShotBehaviour extends OneShotBehaviour {
    public SendOneShotBehaviour(Agent agent) {
        super(agent);
    }

    @Override
    public void action() {
        Scanner s = new Scanner(System.in);
        String content;

        while ( true ) {
            content = s.nextLine();
            Utils.enviarMensaje(myAgent, "chat", content);
        }
    }
}
