#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


/*
 struct sockaddr {
   unsigned short sa_family;   // address family, AF_xxx
   char           sa_data[14]; // 14 bytes of protocol address
 };
 sa_family:
    AF_INET   for IPv4 network protocol (IPv4-only)
    AF_INET6  for IPv6 network protocol (and in some cases, backward compatible with IPv4)
    AF_UNIX   for local socket (using a file)
    AF_UNSPEC <unspecified>
 sa_data:
*/

const char*
get_ip_family_strptr(const int x) {
  static const char* lStrings[5] = {
    "AF_INET",
    "AF_INET6",
    "AF_UNIX",
    "AF_UNSPEC",
    "<AF_IP_FAM_UNKNOWN>"
  };
  const char* lRes = lStrings[4];
  if(AF_INET == x) {
    lRes = lStrings[0];
  } else
  if(AF_INET6 == x) {
    lRes = lStrings[1];
  } else
  if(AF_UNIX == x) {
    lRes = lStrings[2];
  } else
  if(AF_UNSPEC == x) {
    lRes = lStrings[3];
  } else {
    lRes = lStrings[4];
  }
  return lRes;
}

const char*
get_ip_socket_type_strptr(const int x) {
  static const char* lStrings[5] = {
    "SOCK_STREAM",
    "SOCK_DGRAM",
    "SOCK_RAW",
    "SOCK_SEQPACKET",
    "<SOCK_UNKOWN>"
  };

  const char* lRes = lStrings[4];
  if(SOCK_STREAM == x) {
    lRes = lStrings[0];
  } else
  if(SOCK_DGRAM == x) {
    lRes = lStrings[1];
  } else
  if(SOCK_RAW == x) {
    lRes = lStrings[2];
  } else
  if(SOCK_SEQPACKET == x) {
    lRes = lStrings[3];
  } else {
    lRes = lStrings[4];
  }
  return lRes;
}
  
void
print_addrinfo(const struct addrinfo* aAddrInfo) {
  int i = 0;

  printf("  ai_flags   : %d\n", aAddrInfo->ai_flags);
  printf("  ai_family  : %s\n", get_ip_family_strptr(aAddrInfo->ai_family));
  printf("  ai_socktype: %s (%d)\n", get_ip_socket_type_strptr(aAddrInfo->ai_socktype), aAddrInfo->ai_socktype);
  printf("  ai_protocol: %d\n", aAddrInfo->ai_protocol);
  printf("  ai_addrlen : %d\n", aAddrInfo->ai_addrlen);
  printf("  ai_addr    : 0x%X\n", aAddrInfo->ai_addr);
  printf("    .. family: IF_%d\n", aAddrInfo->ai_addr->sa_family);
  printf("    .. data  : ");
  for(i = 0; i < aAddrInfo->ai_addrlen; ++i) {
    if(0 < i) {
      printf(":");
    }
    printf("%u", (unsigned char) aAddrInfo->ai_addr->sa_data[i]);
  }
  printf("\n");
  printf("  ai_canoname: %s\n", aAddrInfo->ai_canonname);

  if(0 != aAddrInfo->ai_next) {
    printf("next:\n");
    print_addrinfo(aAddrInfo->ai_next);
  }
}

// #define RESTRICT_TO_SOCK_DGRAM

void
printHostInfo(const char* aHostName) {
  printf("host name: %s\n", aHostName);
  int lRc = 0;


  const char*      lService  = 0; 
  struct addrinfo* lAddrInfo = 0;
  #ifdef RESTRICT_TO_SOCK_DGRAM
    struct addrinfo  lHints;
    memset(&lHints, 0, sizeof(lHints));
    lHints.ai_socktype = SOCK_DGRAM;
    lRc = getaddrinfo(aHostName, lService, &lHints, &lAddrInfo);
  #else
    struct addrinfo* lHints    = 0; // SOCK_DGRAM, SOCK_STREAM, SOCK_SEQPACKET, SOCK_RAW
    lRc = getaddrinfo(aHostName, lService, lHints, &lAddrInfo);
  #endif
  
 
  if(0 == lRc) {
    print_addrinfo(lAddrInfo);
  } else {
    printf("Info: calling getaddrinfo failed: %s.\n", gai_strerror(lRc));
  } 
}


int
main(const int argc, const char* argv[]) {
  if(2 != argc) {
    printf("usage: %s <hostname>\n", argv[0]);
    return -1;
  }
  printHostInfo(argv[1]);
  return 0;
}
