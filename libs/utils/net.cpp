
#include <vector>
#include <cstdlib>
#include <cstdio>
using namespace std;
extern "C"{
#ifndef  WIN32
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#else
#include <windows.h>
#include <winerror.h>
#include <iphlpapi.h>
#include <winsock2.h>
#endif
}
struct mac_addr{
  unsigned char addr[6];
};

bool 
operator ==(const mac_addr& a,const mac_addr& b){
  for(int j=0;j<6;j++)
    if(a.addr[j]!=b.addr[j])
      return false;
  return true;
}

bool 
operator !=(const mac_addr& a,const mac_addr& b){
  return !(a==b);
}

#ifndef  WIN32

struct mysocket{
  int fd;
  mysocket(){
    fd=-1;
  }
  int
  init(){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd<0)
      return -1;
    else
      return 0;
  }
  
  ~mysocket(){
    if(fd>=0)
      close(fd);
    fd=-1;
  }
  operator int(){
    return fd;
  }
};

int
get_mac_addr(vector<mac_addr>& list){
  mysocket fd;
  if(fd.init()<0)
    return -1;
  
  struct ifreq ifr[16];
  struct ifconf ifc;

  {
    ifc.ifc_len = sizeof(ifr);
    ifc.ifc_buf = (__caddr_t)ifr;
    ioctl(fd, SIOCGIFCONF, &ifc);
    int nifs = ifc.ifc_len / sizeof(struct ifreq);
    
    for (int i=0; i<nifs; i++) {
      ioctl(fd, SIOCGIFHWADDR, ifr+i);
      mac_addr adr;
      adr.addr[0]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[0];
      adr.addr[1]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[1];
      adr.addr[2]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[2];
      adr.addr[3]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[3];
      adr.addr[4]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[4];
      adr.addr[5]=(unsigned char)ifr[i].ifr_hwaddr.sa_data[5];
      list.push_back(adr);
    }
  }
    
  return 0;
}


#else

int
get_mac_addr(vector<mac_addr>& list){

  PIP_ADAPTER_INFO pAdapterInfo;
  PIP_ADAPTER_INFO pAdapter = NULL;
  int dwRetVal = 0;

  ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
  if (pAdapterInfo == NULL) {
    return -1;
  }

  if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
    if (pAdapterInfo == NULL) {
      return -1;
    }
  }

  if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
    pAdapter = pAdapterInfo;
    while (pAdapter) {
      if(pAdapter->AddressLength<6){
	free(pAdapterInfo);
	return -1;
      }
      
      mac_addr adr;
      adr.addr[0]=(unsigned char)pAdapter->Address[0];
      adr.addr[1]=(unsigned char)pAdapter->Address[1];
      adr.addr[2]=(unsigned char)pAdapter->Address[2];
      adr.addr[3]=(unsigned char)pAdapter->Address[3];
      adr.addr[4]=(unsigned char)pAdapter->Address[4];
      adr.addr[5]=(unsigned char)pAdapter->Address[5];
      list.push_back(adr);

      
      pAdapter = pAdapter->Next;
    }
    
  }else{
    if (pAdapterInfo)
      free(pAdapterInfo);
    return -1;
  }

  if (pAdapterInfo)
    free(pAdapterInfo);
  return 0;
}
#endif

int
check_mac_addr(){
  vector<mac_addr> sysadr;
  mac_addr ipaddrs[]={
    {0xe8,0x9d,0x87,0xf8,0xc6,0x23},
    {0xbc,0x5f,0xf4,0x1b,0xd6,0x05}
  };
  mac_addr ladr={0x00,0x00,0x00,0x00,0x00,0x00};

  if(get_mac_addr(sysadr)<0)
    return -1;
  
  for(int i=0;i<sizeof(ipaddrs)/sizeof(mac_addr);i++)
    for(int j=0;j<sysadr.size();j++){
      if(sysadr[j]==ladr)
	continue;
      if(sysadr[j]==ipaddrs[i])
	return 0;
    }
  return -1;
}

// int
// main(){
//   vector<mac_addr> sysadr;
//   get_mac_addr(sysadr);
//   for(int j=0;j<sysadr.size();j++)
//     printf("%02x%02x%02x%02x%02x%02x\n",
// 	   (int)sysadr[j].addr[0],
// 	   (int)sysadr[j].addr[1],
// 	   (int)sysadr[j].addr[2],
// 	   (int)sysadr[j].addr[3],
// 	   (int)sysadr[j].addr[4],
// 	   (int)sysadr[j].addr[5]
// 	   );
//   printf("%s\n",(check_mac_addr()<0?"NG":"OK"));
  
//   return 0;
// }
