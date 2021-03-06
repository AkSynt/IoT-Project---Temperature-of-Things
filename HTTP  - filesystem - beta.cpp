#include "mbed.h"
#include "easy-connect.h"
#include "http_server.h"
#include "http_response_builder.h"
#include "SDFileSystem.h"
 



Serial pc(USBTX, USBRX);
DigitalOut led(LED1);

SDFileSystem sd(PC_12, PC_11, PC_10, PC_9, "sd"); );


// Requests come in here
void request_handler(ParsedHttpRequest* request, TCPSocket* socket) {

    printf("Request came in: %s %s\n", http_method_str(request->get_method()), request->get_url().c_str());

    if (request->get_method() == HTTP_GET && request->get_url() == "/") {
        HttpResponseBuilder builder(200);
        builder.set_header("Content-Type", "text/html; charset=utf-8");

        char response[] = "<html><head><title>Hello from mbed</title></head>"
            "<body>"
                "<h1>mbed webserver</h1>"
                "<button id=\"toggle\">Toggle LED</button>"
                "<script>document.querySelector('#toggle').onclick = function() {"
                    "var x = new XMLHttpRequest(); x.open('POST', '/toggle'); x.send();"
                "}</script>"
            "</body></html>";

        builder.send(socket, response, sizeof(response) - 1);
    }
    else if (request->get_method() == HTTP_POST && request->get_url() == "/toggle") {
        printf("toggle LED called\n");
        led = !led;

        HttpResponseBuilder builder(200);
        builder.send(socket, NULL, 0);
    }
    else {
        HttpResponseBuilder builder(404);
        builder.send(socket, NULL, 0);
    }
}


int main() {
    pc.baud(115200);
    sd.init();
    fs.mount(&sd);
    
    
    

    // Connect to the network (see mbed_app.json for the connectivity method used)
    NetworkInterface* network = easy_connect(true);
    if (!network) {
        printf("Cannot connect to the network, see serial output\n");
        return 1;
    }

    HttpServer server(network);
    nsapi_error_t res = server.start(8080, &request_handler);

    if (res == NSAPI_ERROR_OK) {
        printf("Server is listening at http://%s:8080\n", network->get_ip_address());
    }
    else {
        printf("Server could not be started... %d\n", res);
    }

    wait(osWaitForever);
    
         char temp_str[64];
        
        
        //Read the value
        
        AnalogIn adc(ADC_TEMP);
        
        
        
        
        float temp = ((adc.read()*3.3 - 0.76)/2.5) + 25; 
        
        sprintf(temp_str, "%5.3f", temp);  
 
    mkdir("/sd/mydir", 0777);
    
    FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
    if(fp == NULL) {
        error("Could not open file for write\n");
    }
    fprintf(fp, temp_str);
    fclose(fp); 
    
    
    
    
}
