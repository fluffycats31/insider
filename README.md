# insider

Hooking tcpip's networking functions to intercept and control all incoming/outgoing ipv4 traffic on a windows system while running inside the kernel.

## Information

If the tcpip protocol block does have a OpenQueue pointer then we assume the adapter is disconnected. There are four hooks that are placed in total, first two are to detect when the network adapter is connected/disconnected. The second are for when ipv4 traffic is incoming/outgoing. OpenQueue pointer is only present when the adpater is connected, we wait until it is connected which our hook then is called we let it finish then setup our the networking hook. If OpenQueue is present we place our networking hooks without issue.

The daxin root-kit has major flaws. It assumes OpenQueue is a valid pointer as well the adapter is connected, if such criteia is not met daxin will cause a page fault do to invalid memory access. A user with the root-kit loaded can just disconnect their network adapater then reenable, fully disabling all of the hooks placed by the daxin root-kit. I've have gone out of my way to fix these issues and make this more stable.

## Acknowledgements

- https://www.security.com/threat-intelligence/daxin-malware-espionage-analysis
