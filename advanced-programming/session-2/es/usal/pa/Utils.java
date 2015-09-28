/**
 * This file is provided by USAL, slightly modified
 * (basically s/ontologia/null-ontology/g)
 */
package es.usal.pa;

import jade.content.lang.sl.SLCodec;
import jade.core.AID;
import jade.core.Agent;
import jade.domain.DFService;
import jade.domain.FIPAException;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.Envelope;
import jade.domain.FIPAAgentManagement.SearchConstraints;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.lang.acl.ACLMessage;

import java.io.IOException;
import java.io.Serializable;
import java.util.Iterator;

public class Utils
{

	/**
	 * Permite buscar a todos los agentes que implementa un servicio de un tipo dado
	 * @param agent Agente con el que se realiza la búsqueda
	 * @param tipo  Tipo de servidio buscado
	 * @return Listado de agentes que proporciona el servicio
	 */
    protected static DFAgentDescription [] buscarAgentes(Agent agent, String tipo)
    {
        //indico las características el tipo de servicio que quiero encontrar
        DFAgentDescription template=new DFAgentDescription();
        ServiceDescription templateSd=new ServiceDescription();
        templateSd.setType(tipo); //como define el tipo el agente coordinador tamiben podriamos buscar por nombre
        template.addServices(templateSd);

        SearchConstraints sc = new SearchConstraints();
        sc.setMaxResults(Long.MAX_VALUE);
        try
        {
            DFAgentDescription [] results = DFService.search(agent, template, sc);
            return results;
        }
        catch(FIPAException e)
        {
            //JOptionPane.showMessageDialog(null, "Agente "+getLocalName()+": "+e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        	e.printStackTrace();
        }

        return null;
    }


    /**
     * Envía un objeto desde el agent eindicado a un agent eque proporciona un servicio del tipo dado
     * @param agent Agente desde el que se va a enviar el servicio
     * @param tipo Tipo del servicio buscado
     * @param objeto Mensaje a Enviar
     */
    public static void enviarMensaje(Agent agent, String tipo, Object objeto)
    {
        DFAgentDescription[] dfd;
        dfd=buscarAgentes(agent, tipo);

        try
        {
            if(dfd!=null)
            {
            	ACLMessage aclMessage = new ACLMessage(ACLMessage.REQUEST);

                for ( DFAgentDescription agentDesc : dfd )
	        		aclMessage.addReceiver(agentDesc.getName());

                aclMessage.setOntology("null-ontology");
                //el lenguaje que se define para el servicio
                aclMessage.setLanguage(new SLCodec().getName());
                //el mensaje se transmita en XML
                aclMessage.setEnvelope(new Envelope());
				//cambio la codificacion de la carta
				aclMessage.getEnvelope().setPayloadEncoding("ISO8859_1");
                //aclMessage.getEnvelope().setAclRepresentation(FIPANames.ACLCodec.XML);
        		aclMessage.setContentObject((Serializable)objeto);
        		agent.send(aclMessage);
            }
        }
        catch(IOException e)
        {
            //JOptionPane.showMessageDialog(null, "Agente "+getLocalName()+": "+e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        }
    }

    /**
     * Permite buscar los agents que dan un servicio de un determinado tipo. Devuelve el primero de ellos.
     * @param agent Agentes desde el que se realiza la búsqueda
     * @param tipo Tipo de servicio buscado
     * @return Primer agente que proporciona el servicio
     */
    protected static DFAgentDescription buscarAgente(Agent agent, String tipo)
    {
        //indico las características el tipo de servicio que quiero encontrar
        DFAgentDescription template=new DFAgentDescription();
        ServiceDescription templateSd=new ServiceDescription();
        templateSd.setType(tipo); //como define el tipo el agente coordinador tamiben podriamos buscar por nombre
        template.addServices(templateSd);

        SearchConstraints sc = new SearchConstraints();
        sc.setMaxResults(new Long(1));

        try
        {
            DFAgentDescription [] results = DFService.search(agent, template, sc);
            if (results.length > 0)
            {
                //System.out.println("Agente "+agent.getLocalName()+" encontro los siguientes agentes");
                for (int i = 0; i < results.length; ++i)
                {
                    DFAgentDescription dfd = results[i];
                    AID provider = dfd.getName();

                    //un mismo agente puede proporcionar varios servicios, solo estamos interasados en "tipo"
                    Iterator it = dfd.getAllServices();
                    while (it.hasNext())
                    {
                        ServiceDescription sd = (ServiceDescription) it.next();
                        if (sd.getType().equals(tipo))
                        {
                            System.out.println("- Servicio \""+sd.getName()+"\" proporcionado por el agente "+provider.getName());

                            return dfd;
                        }
                    }
                }
            }
            else
            {
                //JOptionPane.showMessageDialog(null, "Agente "+getLocalName()+" no encontro ningun servicio buscador", "Error", JOptionPane.INFORMATION_MESSAGE);
            }
        }
        catch(FIPAException e)
        {
            //JOptionPane.showMessageDialog(null, "Agente "+getLocalName()+": "+e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        	e.printStackTrace();
        }

        return null;
    }
}
