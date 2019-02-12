#include "publish.h"
#include "subscribe.h"
#include "stdio.h"
#include "MQTTAsyncClient.h"
#include <string>
#include <cctype>

int main_dep(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (*argv[1] == 's')
		{
			printf("Subscribe");
			MQTT_SUBSCRIBE::mqtt_subscribe();
		}
		else if (*argv[1] == 'p')
		{
			printf("Publish");

			MQTT_PUBLISH::mqtt_publish();
		}
	}
	else
	{
		MQTT_PUBLISH::mqtt_publish();
		//MQTT_SUBSCRIBE::mqtt_subscribe();
	}
	return 0;
}

void OnMsgArrived(std::string topic, std::string content)
{
	std::cout << "Message arrived" << std::endl;
	std::cout << "\ttopic: '" << topic << "'" << std::endl;
	std::cout << "\tpayload: '" << content << "'\n" << std::endl;


}
void OnConnect(const std::string& cause)
{
	std::cout << "\nConnection success" << std::endl;

}
void OnConnectionLost(const std::string& cause)
{
	std::cout << "\nConnection lost" << std::endl;
	if (!cause.empty())
		std::cout << "\tcause: " << cause << std::endl;
}
void OnDeliveryComplete(mqtt::delivery_token_ptr token)
{

	std::cout << "Message delivery_complete" << std::endl;
}


int main(int argc, char* argv[])
{
	std::string ADDRESS = "tcp://iot.eclipse.org:1883";
	std::string  TOPIC = "MQTT Examples";
	std::string  PAYLOAD = "Hello World!";


	if (argc == 2)
	{
		if (*argv[1] == 's')
		{
			printf("Subscribe");

			std::string  CLIENTID = "ExampleClientSub";
			DDRFramework::MQTTAsyncClient client(ADDRESS, CLIENTID);
			client.m_OnConnect = OnConnect;
			client.m_OnConnectionLost = OnConnectionLost;
			client.m_OnMsgArrived = OnMsgArrived;
			client.m_OnDeliveryComplete = OnDeliveryComplete;

			client.Connect();

			while (!client.m_IsConnected)
			{
				this_thread::sleep_for(chrono::seconds(1));
			}

			client.Subscribe(TOPIC);


			while (std::tolower(std::cin.get()) != 'q')
			{
				;
			}

		}
		else if (*argv[1] == 'p')
		{
			printf("Publish");

			std::string  CLIENTID = "ExampleClientPub";
			DDRFramework::MQTTAsyncClient client(ADDRESS, CLIENTID);
			client.m_OnConnect = OnConnect;
			client.m_OnConnectionLost = OnConnectionLost;
			client.m_OnMsgArrived = OnMsgArrived;
			client.m_OnDeliveryComplete = OnDeliveryComplete;

			client.Connect();
			client.Publish(TOPIC,PAYLOAD);
		}
	}
	else
	{
		std::string  CLIENTID = "ExampleClientPub";
		DDRFramework::MQTTAsyncClient client(ADDRESS, CLIENTID);
		client.m_OnConnect = OnConnect;
		client.m_OnConnectionLost = OnConnectionLost;
		client.m_OnMsgArrived = OnMsgArrived;
		client.m_OnDeliveryComplete = OnDeliveryComplete;

		client.Connect();
		client.Publish(TOPIC, PAYLOAD);
		//MQTT_PUBLISH::mqtt_publish();
		//MQTT_SUBSCRIBE::mqtt_subscribe();
	}
}