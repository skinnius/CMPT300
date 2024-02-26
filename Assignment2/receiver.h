#ifndef _RECEIVER_H_
#define _RECEIVER_H_

// start background receive thread
void Receiver_init(char* rm);

// stop background receive thread and cleanup
void Receiver_shutdown(void);

// THREADSAFE !! :D
void Receiver_changeDynamicMessage(char* recvMessage);


#endif