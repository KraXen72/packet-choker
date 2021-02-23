# packet-choker
Simple dll that chokes packets

Inject this dll into process you want to choke outgoing packets from to create fake network latency. Useful in online schools when teacher starts asking you questions you have no idea about. Hopefully they will leave you alone. Tested on Teams 1.4.00.2879 64bit and earlier. No incomming packets are touched so you will hear other people in call load and clear (if you have good net connection) but they will barely understand you.

## How it works?
Upon injecting dll hooks winsock api functions _send_ and _sendto_ from _WS2_32.DLL_. In hook it chokes 5 out of 6 outgoing packets by simply returning packet length and making target process thinking each packet was successfully sent but _lost somewhere in net cable_ otherwise it calls original micro$oft api. The 'lag switch' is just a check if directory D:/t exists because im too lazy to make something more userfriendly lmao. TODO: gui with button
