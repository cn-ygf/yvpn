#pragma once
#include <Iphlpapi.h>  

#pragma comment(lib, "Iphlpapi.lib")  
#pragma comment(lib, "WS2_32.lib")  


PMIB_IPFORWARDTABLE MyGetIpForwardTable(BOOL bOrder);
void MyFreeIpForwardTable(PMIB_IPFORWARDTABLE pIpRouteTab);
int RemoveDefaultRoute(char *lpszCurrentVpnAddress);
int AddDefaultRoute();