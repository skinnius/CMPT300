#ifndef _RECEIVER_H
#define _RECEIVER_H_

// start background receive thread
void Receiver_init(char* recvMessage);

// stop background receive thread and cleanup
void Receiver_shutdown(void);

// THREADSAFE !! :D
void Receiver_changeDynamicMessage(char* recvMessage);


#endif