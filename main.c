#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>


void get_mac_address(char *mac_address) {       //Creo le due struct che conterranno i dati delle interfacce
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {    //il metodo crea una lista collegata di struct ifaddrs, inizializzando *ifaddr
        perror("Error getting interfaces");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {      //iterazione su tutte le interfacce
        if (ifa->ifa_addr == NULL)      //se l'interaffia corrrente non contiene i dati richiesti, essa viene scartata
            continue;

        if ((ifa->ifa_flags & IFF_UP) && (ifa->ifa_addr->sa_family == AF_PACKET)) {     //controllo se l'interfaccia è attiva ed è un AF_PACKET
            char *interface_name = ifa->ifa_name;
            if (interface_name[0] == 'e' || interface_name[0] == 'w') {
                struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;     //cast da iffadrs a sockaddr_ll
                for (int i = 0; i < s->sll_halen; i++) {        //estrazione del valore MAC un byte alla volta...
                    sprintf(mac_address+i*3, "%02X:", s->sll_addr[i]);      //...perché è necessario costruire una sringa
                }
                mac_address[strlen(mac_address)-1] = '\0';  //rimozione dell'ultimo carattere ":"
                break;
            }
        }
    }
    freeifaddrs(ifaddr);       //liberazione dello spazzio utilizzato
}

int main() {
    char mac_address[17];
    get_mac_address(mac_address);
    printf("MAC address: %s\n", mac_address);
    return 0;
}
