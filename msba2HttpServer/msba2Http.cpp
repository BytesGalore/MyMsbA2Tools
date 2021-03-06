#include <boost/network/protocol/http/server.hpp>
#include "pageHandler.hpp"
#include <string>


using namespace boost::network;



struct msba2_server;
typedef http::server<msba2_server> server;
pageHandler ph;

struct msba2_server {
    void operator() (server::request const &request,
                     server::response &response) {

    	std::string strReqVal = "";
    	for(size_t i = 0; i < request.headers.size(); i++)
		{
			if(request.headers.at(i).name == "Accept")
				strReqVal = request.headers.at(i).value;

		}
    	std::string strReq = request.destination;

    	if(strReq.find(".js") != std::string::npos)
    	{
    		strReqVal = "text/javascript";
    	}

    	if(strReq.find(".xml") != std::string::npos
    			|| strReq.find(".svg") != std::string::npos)
		{
			strReqVal = "application/xml";
		}

		std::string strRes = ph.handle(strReq, strReqVal);
		if(!strRes.empty())
		{

			response = server::response::stock_reply(server::response::ok, strRes);
    	    response.headers[1].value = strReqVal;
    	}


    }
    void log( const std::string &message ) const {}
};

void startHttpServer(std::string strAddr, std::string strPort)
{
	try {
	        msba2_server handler;
	        server::options options(handler);
	        server server_(options.address(strAddr).port(strPort));
	        server_.run();
	    }
	    catch (std::exception &e) {
	        std::cerr << e.what() << std::endl;
	    }
}


int main(int argc, char * argv[]) {using namespace boost::network;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }

    startHttpServer(std::string(argv[1]), std::string(argv[2]));

    return 0;
}
