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
        System.out.println("SendOneShotBehaviour");

        while ( true ) {
            content = s.nextLine();
            System.out.println("Read message");
            Utils.enviarMensaje(myAgent, "chat", content);
        }
    }
}
