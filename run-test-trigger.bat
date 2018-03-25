start node    trigger  5010 127.0.0.1:srvleft 127.0.0.1:srvright vicon
start node    server   --uc 127.0.0.1:uc --ui 127.0.0.1 -s LEFT  --trsrv 127.0.0.1:5010 --trcltid srvleft  5001
start node    server   --uc 127.0.0.1:uc --ui 127.0.0.1 -s RIGHT --trsrv 127.0.0.1:5010 --trcltid srvright 5002
start node uc-emulator 127.0.0.1 5001
start node uc-emulator 127.0.0.1 5002
