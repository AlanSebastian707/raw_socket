# raw_socket
Algorithm: Implementation of Raw Socket for Packet Capture
1. Start the program.
2. Create a raw socket using socket() with AF_PACKET, SOCK_RAW and ETH_P_ALL.
3. Check whether socket creation is successful.
4. Enter an infinite loop to capture packets.
5. Receive packets using recvfrom().
6. Check the received packet size.
7. Extract the Ethernet header.
8. Display source MAC, destination MAC and protocol type.
9. Check whether the packet contains IPv4.
10. Extract the IP header.
11. Display source IP, destination IP, TTL and protocol.
12. Calculate the IP header length.
13. Identify the transport protocol:
    - TCP: Extract TCP header and display source and destination ports.
    - UDP: Extract UDP header and display source and destination ports.
    - ICMP: Extract ICMP header and display type and code.
    - Others: Display other packet.
14. Repeat packet capturing.
15. Close the socket and stop the program.
Easy way to memorise
Remember this sequence:
CREATE → RECEIVE → ETHERNET → IP → PROTOCOL → EXTRACT → DISPLAY → REPEAT
Layer	Information extracted
Ethernet	Source MAC   Destination MAC
IP	       Source IP   Destination IP, TTL
TCP	       Source Port Destination Port
UDP	       Source Port Destination Port
ICMP	     Type       Code
