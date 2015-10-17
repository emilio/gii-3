package agents;

import java.io.Serializable;
import java.io.IOException;

import jade.content.lang.sl.SLCodec;
import jade.core.Agent;
import jade.core.AID;
import jade.domain.DFService;
import jade.domain.FIPAException;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.Envelope;
import jade.domain.FIPAAgentManagement.SearchConstraints;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.lang.acl.ACLMessage;

public class SimpleAgent extends Agent {
    public static final String DEFAULT_ONTOLOGY = "default-ontology";

    public void simpleSetup(String serviceType) throws FIPAException {
        DFAgentDescription agentDescription = new DFAgentDescription();
        agentDescription.setName(this.getAID());

        ServiceDescription serviceDescription = new ServiceDescription();
        serviceDescription.setName(this.getLocalName());
        serviceDescription.setType(serviceType);

        agentDescription.addServices(serviceDescription);

        DFService.register(this, agentDescription);
    }

    public DFAgentDescription[] findAgents(String type,
                                           String name,
                                           long maxResults) {
        DFAgentDescription template = new DFAgentDescription();

        if ( type != null || name != null ) {
            ServiceDescription serviceDescription = new ServiceDescription();

            if ( type != null )
                serviceDescription.setType(type);

            if ( name != null )
                serviceDescription.setName(name);

            template.addServices(serviceDescription);
        }

        SearchConstraints constraints = new SearchConstraints();
        constraints.setMaxResults(maxResults);

        try {
            return DFService.search(this, template, constraints);
        } catch(FIPAException e) {
            e.printStackTrace();
            return null;
        }
    }

    public DFAgentDescription[] findAgents(String type, String name) {
        return findAgents(type, name, Long.MAX_VALUE);
    }

    public DFAgentDescription[] findAgents() {
        return findAgents(null, null, Long.MAX_VALUE);
    }

    public DFAgentDescription findAgent(String type,
                                        String name) {
        DFAgentDescription[] results = findAgents(type, name, 1l);

        if ( results == null || results.length == 0 )
            return null;

        return results[0];
    }

    public AID getService(String serviceType) {
        DFAgentDescription maybeAgent = findAgentByType(serviceType);

        if ( maybeAgent == null )
            return null;

        return maybeAgent.getName();
    }

    public DFAgentDescription findAgentByName(String name) {
        return findAgent(null, name);
    }

    public DFAgentDescription findAgentByType(String type) {
        return findAgent(type, null);
    }

    public DFAgentDescription[] findAgentsByType(String type) {
        return findAgents(type, null);
    }

    public DFAgentDescription[] findAgentsByName(String name) {
        return findAgents(null, name);
    }

    public boolean sendMessage(DFAgentDescription[] receivers,
                               Serializable contents) {
        AID[] agents = new AID[receivers.length];

        for (int i = 0; i < receivers.length; ++i)
            agents[i] = receivers[i] == null ? null : receivers[i].getName();

        return sendMessage(agents, contents);
    }

    public boolean sendMessage(AID receiver, Serializable contents) {
        return sendMessage(new AID[] { receiver }, contents);
    }

    public boolean sendMessage(AID[] receivers,
                               Serializable contents) {
        if ( receivers == null || contents == null )
            return false;

        ACLMessage aclMessage = new ACLMessage(ACLMessage.REQUEST);

        for ( AID agent : receivers )
            if ( agent != null )
                aclMessage.addReceiver(agent);

        // aclMessage.setOntology(ontology);
        // aclMessage.setLanguage(new SLCodec().getName());
        aclMessage.setEnvelope(new Envelope());
        // aclMessage.getEnvelope().setPayloadEncoding("ISO8859_1");
        // aclMessage.getEnvelope().setAclRepresentation(FIPANames.ACLCodec.XML);

        try {
            aclMessage.setContentObject(contents);
        } catch (IOException e) {
            return false;
        }

        this.send(aclMessage);
        return true;
    }

    public boolean sendMessageToType(String type, Serializable contents) {
        return sendMessage(findAgentsByType(type), contents);
    }
}
