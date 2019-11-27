# ACI_labs

Duvidas: 

Switch:
  - How does a switch know where a MAC address is if it doesnt have the destination MAC address in its table (flooding is the answer to this, but what if there are more switches in between the two hosts trying to communicate)
  	R: a switch does not have a MAC address. When Host A tries to communicate with Host B, and the its MAC address is unknown, before sending any DATA,  it must go throught a protocol, ARP (address resolution protocol) which allows the Host A to broadcast a message to receive a Host B response with the destination MAC address.
 
  - When a MAC address is unknown but are in different networks, how does a sender find a destination node.
  	R: ARP protocol.

  - Even if it does know the destination MAC address but the node belongs to a different network, how does it find the destination node (shouldn't it have the gateway MAC address?);
  	R: Only has a DG MAC address in the case that between hosts there is a router.

  - Is it because the mast is periodic that there is no need to configure a watchdog?
  	
  
  - Pratica: diagrama temporal, quando o Ciclo de Rede começa, como a memoria partilhada vai buscar os inputs (que instantes) e envia os outputs também?
  	Depende do modelo que estivermos a falar, mas garantidamente dentro do ciclo de rede o RTU envia os inputs que recebeu para a Carta de memoria do PLC, a carta de memoria do PLC envia tb os seus outputs a serem escritos no RTU.

  - Diagrama professor, necessidade do ethernet ter o PLC connectado a essa rede também?
  	

  - Objetos (structs) caso de nao existirem sub indices que valor surge no inicio?
  	No inicio indicaria logo 0, indicando que não é uma estrutura.
  
  - Objetos: quando enviamos a informaçao para um dispositivo, esta ja tem que estar bem estruturada de maneira ao dispoisitivo saber que secção dos dados vai onde? (trama = frame?)
  
  
  - o que define se guardamos no %M ou %MW (no PLC)? tamanho das variáveis no RTU? (no nosso trabalho os I eram %M ?)
  
  
  - pq é que quando intorduzimos T(MinSI) as leituras no PROFIBUS passam a ser Cyclicos? (Isto não implica periodico certo?, (isocrono = periodico?) )
  	O PROFIBUS comunica utilizando tanto o token passing como o Master Slave ao mesmo tempo. 
  
  - TCR é um ciclo medido na perspetiva da rede?
  	Depende do modelo.
  
  - TCR = SYNC*N? 
  	Sim.

  - ciclico, aciclico, isocrono, periodico.i

  - Pq é que o CAN nao pode usar as tecnologias mais recented do physical layer: e.g 10GbaseT  - ciclico, aciclico, isocrono, periodico.i

  - Pq é que o CAN nao pode usar as tecnologias mais recented do physical layer: e.g 10GbaseT?

  - Sincrono e asincrono varia dependendo do conteto? (envio de data, envio de pacotes)

  - How does the maximum cable length change when speed changes?

  - 4 entradas digitais, ocupam 4 words/1 word/4 registos?

  - CAN: quando colisão não ocorre comparando COB-ID, o que a sucede?
