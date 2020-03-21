# Socket programming

## Contents
- [1. Host byte storage structure judgment](#env)
- [2. UDP socket programming](#env1)
- [3. SOCK_STREAM programming](#env2)
- [4. TCP Public Scanner](#env3)

##<span id="env">1. Host byte storage structure judgment</span>
Based on the concepts of big-endian and little-endian, write host_endian.c to determine the byte storage structure of the test machine.
Double words are stored in memory in the big-endian mode, where the high byte stores the low address and the low byte stores the high address, whereas in the little-endian the opposite is in contrast.
- Screenshot of running results:
<img src="https://pic.downk.cc/item/5e758d989d7d586a549eb3c8.jpg"/>

##<span id="env1">2.UDP socket programming</span>
Compared with TCP socket programming, UDP socket programming does not need to listen and accept connections on the server side. The address information is specified in the send and receive functions, and the parameters of socket () are also different. The basic process is as follows
<img src='https://pic.downk.cc/item/5e758e279d7d586a549f4bf9.jpg'/>

- you can use the **man** instruction to view the main function parameters
- Screenshot of running results:
<img src='https://pic.downk.cc/item/5e758e8d9d7d586a549fbd5b.jpg' />
<img src='https://pic.downk.cc/item/5e758ed69d7d586a54a00b3d.jpg' />

##<span id="env2">3. SOCK_STREAM programming</span>
Use stream socket to implement a simple remote control system, which can be terminated with quit, can receive and respond to client commands, and the server can receive multiple connections from the client.

- Screenshot of running results:
<img src='https://pic.downk.cc/item/5e75989e9d7d586a54a91b01.jpg'/>
<img src='https://pic.downk.cc/item/5e7598cf9d7d586a54a93ba4.jpg'/>

##<span id="env3">4. TCP Public Scanner</span>

- First receive the IP address and port range to be scanned. If the input parameters are not four, return to the correct usage and exit.
- Set the loop from the initial port to the end port. Use the **connect()** function to establish a connection for each port number in the range. If the return value is not less than 0, the connection is successfully established, and the socket is closed to return the ret value.
- Use the **htons** function to convert the port number from host byte order to server byte order as a parameter to the **getservbyport** function. If the **getservbyport** function call is successful, it returns a pointer to the servant structure, otherwise it returns a null pointer.
- Output the attributes of the **servant** structure and proceed to the next cycle.
- Screenshot of running results:
<img src='https://pic.downk.cc/item/5e759af79d7d586a54ab3773.jpg' />
