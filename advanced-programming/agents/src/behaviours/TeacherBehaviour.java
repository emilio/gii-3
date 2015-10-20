package behaviours;

import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl.UnreadableException;
import jade.core.behaviours.CyclicBehaviour;

import jade.core.AID;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.*;

import messages.*;

import agents.Availability;
import agents.Teacher;

/**
 * This is the main teacher behaviour.
 * The behaviour encapsulates the teacher logic, storing the availability
 * days of the alumns.
 */
public class TeacherBehaviour extends CyclicBehaviour {
    private static long MAX_ALUMNS_PER_GROUP = 5;
    private static EnumSet<Availability> AVAILABLE_GROUPS = EnumSet.of(Availability.MONDAY, Availability.TUESDAY, Availability.THURDSDAY, Availability.FRIDAY);

    /**
     * This is the list of registered alumns and the group they're assigned to.
     */
    private HashMap<AID, Availability> groups;

    private Teacher teacher;

    private Random random;

    public HashMap<AID, Availability> getGroups() {
        return new HashMap<AID, Availability>(this.groups);
    }

    public TeacherBehaviour(Teacher agent) {
        super(agent);
        this.groups = new HashMap<>();
        this.teacher = agent;
        this.random = new Random();
    }

    private Availability firstAssignation(AID alumn) {
        if ( groups.containsKey(alumn) ) {
            System.err.println("WARN: Requested first assignation for already registered alumn " + alumn);
            return groups.get(alumn);
        }

        // TODO: This could be more optimized, for example, having the availabilityCount map cached

        // Get the count of the current availabilities
        Map<Availability, Long> availabilityCount = groups.values().stream()
                                                                   .collect(Collectors.groupingBy(a -> a, Collectors.counting()));

        // Get the current available groups
        List<Availability> availableGroups = AVAILABLE_GROUPS.stream()
                                                            .filter(a -> availabilityCount.getOrDefault(a, 0l) < MAX_ALUMNS_PER_GROUP)
                                                            .collect(Collectors.toList());

        // Pick a random one
        Availability result = availableGroups.get(this.random.nextInt(availableGroups.size()));
        groups.put(alumn, result);

        return result;
    }

    @Override
    public void action() {
        // We have to use a timeout to allow the WakerBehaviour to take place
        ACLMessage msg = myAgent.blockingReceive(100);

        if ( msg == null )
            return;

        AID sender = msg.getSender();
        Message message = null;

        try {
            message = (Message) msg.getContentObject();
        } catch (UnreadableException e) {
            System.err.println("[" + myAgent.getAID() + "] Error receiving message from " + sender);
            e.printStackTrace(System.err);
            return;
        }

        System.err.println("INFO: [" + myAgent.getAID() + "] ReceiveMessage (" + message.getType() + ")");

        switch ( message.getType() ) {
            case FIRST_ASSIGNATION_REQUEST:
                // TODO: This should be an INFORM message, with reply
                teacher.sendMessage(sender, new FirstAssignationMessage(this.firstAssignation(sender)));
                return;
            case TEACHER_GROUP_CHANGE_REQUEST:
                TeacherGroupChangeRequestMessage requestMessage = (TeacherGroupChangeRequestMessage) message;

                assert(requestMessage.fromAlumn.equals(sender));

                // Ensure those alumns haven't been changed previously
                if ( groups.get(requestMessage.fromAlumn) == requestMessage.fromGroup &&
                     groups.get(requestMessage.toAlumn) == requestMessage.toGroup) {
                    groups.put(requestMessage.fromAlumn, requestMessage.toGroup);
                    groups.put(requestMessage.toAlumn, requestMessage.fromGroup);
                    teacher.sendMessage(requestMessage.fromAlumn, new TeacherGroupChangeMessage(requestMessage.fromGroup, requestMessage.toGroup));
                    teacher.sendMessage(requestMessage.toAlumn, new TeacherGroupChangeMessage(requestMessage.toGroup, requestMessage.fromGroup));
                } else {
                    teacher.sendMessage(requestMessage.fromAlumn, new TeacherGroupChangeRequestDenegationMessage());
                }
                return;
            default:
                System.err.println("ERROR: Unexpected message of type " + message.getType() + " received in TeacherBehaviour. Sender: " + sender + "; Receiver: " + teacher.getAID());
                return;
        }
    }
}
