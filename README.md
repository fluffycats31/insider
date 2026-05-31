# insider

Hooking tcpip's networking functions to intercept and control all incoming/outgoing ipv4 traffic on a windows system while running inside the kernel.

## Information

If the tcpip protocol block does have a OpenQueue pointer then we assume the adapter is disconnected. There are four hooks that are placed in total, first two are to detect when the network adapter is connected/disconnected. The second are for when ipv4 traffic is incoming/outgoing. OpenQueue pointer is only present when the adpater is connected, we wait until it is connected which our hook then is called we let it finish then setup our the networking hook. If OpenQueue is present we place our networking hooks without issue.

## Acknowledgements

- https://www.security.com/threat-intelligence/daxin-malware-espionage-analysis
