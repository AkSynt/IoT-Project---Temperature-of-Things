#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif



 
#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include <iostream>
#include <string> 





 
#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Internet of Things - HTTP</h1>" "\r\n"                              \
"      <p>Temperature Monitor!</p>" "\r\n"                                 \
"      <p><button onclick=location=URL>Get Temperature!</button></p>" "\r\n" \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>"
 

#define HTTP_MESSAGE_BODY_A ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Internet of Things - HTTP</h1>" "\r\n"                              \
"      <p>Temperature Monitor!</p>" "\r\n"                                 \
"      <p><button onclick=location=URL>Get Temperature!</button></p>" "\r\n"

#define HTTP_MESSAGE_BODY_B ""                                     \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>"

  
#define HTTP_RESPONSE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"
 





int main()
{	
	printf("======================================\n");
    printf("Preparing to provide IP address. Wait..\n");
    
    EthernetInterface eth;
    eth.connect();
    
    printf("The Board address is '%s'\n", eth.get_ip_address());
	printf("======================================\n");
    
	//Create TCP Server
	//also TCP adress
	
    TCPServer srv;
    TCPSocket clt_sock;
    SocketAddress clt_addr;
    
    /* Open the server on ethernet stack */
	
    srv.open(&eth);
    
    /* Bind the HTTP port (TCP 80) to the server */
	
    srv.bind(eth.get_ip_address(), 80);
    
    /* Can handle more than one simultaneous connections */
	
    srv.listen();
    
    while (true) {
		
        srv.accept(&clt_sock, &clt_addr);
        printf("accept %s:%d\n", clt_addr.get_ip_address(), clt_addr.get_port());
		
		string response;
        //This is a C string
        
       
        char temp_str[64];
        
        
        //Read the value
        
		AnalogIn adc(ADC_TEMP);
		
        
        
        
		float temp = ((adc.read()*3.3 - 0.76)/2.5) + 25;
        
        //Convert to a C String
        
        sprintf(temp_str, "%5.3f", temp);
        
        
        //Build the C++ string response
//        response = HTTP_MESSAGE_BODY;
//        response += temp_str;
  
  response = HTTP_MESSAGE_BODY_A;
  response += "      <p>";
  response +=temp_str;
  response +="</p>" "\r\n";                                 \
  response += HTTP_MESSAGE_BODY_B;
  
                
        //Send static response
        clt_sock.send(response.c_str(), response.size()+1);
		clt_sock.close();
    }
	
	
}
 
            