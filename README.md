# packet-choker
Simple dll that chokes packets

Inject this dll into process you want to choke outgoing packets from to create fake network latency. Useful in online schools when teacher starts asking you questions you have no idea about. Hopefully they will leave you alone. Tested on Teams 1.4.00.2879 64bit and earlier. No incomming packets are touched so you will hear other people in call load and clear (if you have good net connection) but they will barely understand you.

## How it works?
Upon injecting dll hooks winsock api functions _send_ and _sendto_ from _WS2_32.DLL_
