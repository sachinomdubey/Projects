# STEPS FOR SATCOM DEMO
## FREE-5GC 
Make sure the FREE-5GC running. Follow the document named **Doc** to setup and run FREE-5GC. 

## gNB 
**NOTE :** For Pre-requisites and detailed installation steps , Please refer the file "**UE_gNB_setup_user_guide.docx**". 

After pre-requisite installations (JSON, ROHC, Nettle, Libexplain, DPDK, ASN1, SCTP, AESN1 and IPSEC), Follow the below steps in gNB workstation terminal:
#### Exporting environment variables:

    export RTE_SDK=<path to DPDK folder installed>
    
    export RTE_TARGET=x86_64-native-linuxapp-gcc
#### Loading Huge pages:

    sudo su
    
    echo 4096 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
    
    exit
#### Compiling and running the gNB App:

    cd ~/Documents/simran_wsp/bs_working/review-bs/5gnrps/src/gnbapp/test

    make clean
    
    make static -j10

    sudo ./gnbapp enp1s0 -- -diersg
    

## UE
**NOTE :** For Pre-requisites and detailed installation steps , Please refer the file "**User Guide.docx**". 

After pre-requisite installations (JSON, ROHC, Nettle, Libexplain, DPDK, ASN1, SCTP, AESN1 and IPSEC), Follow the below steps in UE workstation terminal:
#### Exporting environment variables:

    export RTE_SDK=/home/greyteal/Documents/6G_Project/dpdk-stable-20.02.1
    
	export RTE_TARGET=x86_64-native-linuxapp-gcc
#### Loading Huge pages:

    sudo su
    
    echo 4096 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
    
    exit
#### Compiling and running the UE App:

    cd ~/Documents/UE/5gnrps/src/ueapp/test

    make clean
    
    make CPUSOC=1 JSON=1 -j10

    sudo ./ueapp enp1s0 0 -- -dierns

#### Note the PDU session IP address and open a seperate terminal to run the following commands:

    sudo ifconfig tun00 10.60.0.1 up

    sudo ip route add 192.168.134.224 dev tun00

Check the PDU session using ping (5 packets):

    ping -I tun00 192.168.134.224 -c 5

## Streaming video from 5GC to UE via gNB

To stream a video (videoplayback.mp4) from 5GC to UE via gNB, run the following command in terminal of 5GC workstation:

    cd ~/Documents

    vlc videoplayback.mp4 --sout=udp://10.60.0.1:1234

This will create a video stream originating from 5GC and streamed at UE via gNB. Now, to view the stream at UE workstation, run the following command in terminal:

    vlc udp"//@:1234 --miface=10.60.0.1:1234
  


