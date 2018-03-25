start node server      --uc 127.0.0.1:uc --ui 127.0.0.1 --br 127.0.0.1:bridge -s LEFT 5001
start node udp-bridge  127.0.0.1 7001 127.0.0.1 5001 bridge
start node uc-emulator 127.0.0.1 5001 7001
