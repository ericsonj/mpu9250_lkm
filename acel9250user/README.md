# Test MPU-9250 LKM

Test module with program to read `/dev/acel9250`  

## How to use

`(host)$ export ARM=arm`  
`(host)$ export export CROSS_COMPILE=arm-linux-gnu-`  
`(host)$ make`   


`(bbb)# ./acel9250user > /tmp/out ; cat /tmp/out`   
