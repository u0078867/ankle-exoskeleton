start node trigger     5010 192.168.1.124:srvleft vicon
start node server      --uc 192.168.1.126 --ui 192.168.1.124 --br 192.168.1.124:bridge -s LEFT --trsrv 192.168.1.124:5010 --trcltid srvleft  5001
start node udp-bridge  192.168.1.124 3333 192.168.1.124 5001 bridge

