g++ -c client1.cpp -o client.o
g++ client.o -lws2_32 -lwinmm -lgdi32 -luser32 -lkernel32 -lcomctl32 -o TalkTalk.exe
