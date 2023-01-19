#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>


void get_mac_address(char *mac_address) {
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {        //Ogni interfaccia che non rispetta il controllo viene scartata
        perror("Error getting interfaces");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {      //Questo ciclo for ha come side effect
        if (ifa->ifa_addr == NULL)                              //la rimozione delle interfacce con dati non validi
            continue;

        if ((ifa->ifa_flags & IFF_UP) && (ifa->ifa_addr->sa_family == AF_PACKET)) {     //ogni volta che un tentativo fallisce, si ritenta con la prossima interfaccia di ifa
            char *interface_name = ifa->ifa_name;
            if (interface_name[0] == 'e' || interface_name[0] == 'w') {
                struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                for (int i = 0; i < s->sll_halen; i++) {
                    sprintf(mac_address+i*3, "%02X:", s->sll_addr[i]);
                }
                mac_address[strlen(mac_address)-1] = '\0';
                break;
            }
        }
    }
    freeifaddrs(ifaddr);
}

int main() {
    char mac_address[17];
    get_mac_address(mac_address);
    printf("MAC address: %s\n", mac_address);
    return 0;
}