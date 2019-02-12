#include "publish.h"
#include "subscribe.h"
#include "stdio.h"
#include "MQTTAsyncClient.h"
#include <string>
#include <cctype>

#include "JsonProto.h"

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



	DDRStatus status;
	CJsonSerializer::Deserialize(&status,content);

	std::cout << "\tname: '" << status.Name << "'\n" << std::endl;
	std::cout << "\tlspeed: '" << status.LineSpeed << "'\n" << std::endl;
	std::cout << "\tangspeed: '" << status.AngSpeed << "'\n" << std::endl;



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


std::string GetJsonStr()
{

	DDRStatus status;
	status.Name = "JN";
	status.LineSpeed = 123;
	status.AngSpeed = 987;
	status.IntArray.push_back(3);
	status.IntArray.push_back(4);

	std::string jsonStr;
	CJsonSerializer::Serialize(&status, jsonStr);

	return jsonStr;
}

int main(int argc, char* argv[])
{
	std::string ADDRESS = "tcp://iot.eclipse.org:1883";
	std::string  TOPIC = "DDRStatus";




	if (argc == 2)
	{
		if (*argv[1] == 's')
		{
			printf("Subscribe");

			std::string  CLIENTID = "DDR Subscribe";
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


			char c;
			do 
			{
				c = std::tolower(std::cin.get());

				if (c == 'p')
				{

					client.Publish(TOPIC, GetJsonStr());
				}

			} while (c != 'q');

		}
		else if (*argv[1] == 'p')
		{
			printf("Publish");

			std::string  CLIENTID = "DDR Publish";
			DDRFramework::MQTTAsyncClient client(ADDRESS, CLIENTID);
			client.m_OnConnect = OnConnect;
			client.m_OnConnectionLost = OnConnectionLost;
			client.m_OnMsgArrived = OnMsgArrived;
			client.m_OnDeliveryComplete = OnDeliveryComplete;

			client.Connect();





			client.Publish(TOPIC, GetJsonStr());
		}
	}
	else
	{
		std::string  CLIENTID = "DDR Publish";
		DDRFramework::MQTTAsyncClient client(ADDRESS, CLIENTID);
		client.m_OnConnect = OnConnect;
		client.m_OnConnectionLost = OnConnectionLost;
		client.m_OnMsgArrived = OnMsgArrived;
		client.m_OnDeliveryComplete = OnDeliveryComplete;

		client.Connect();


		client.Publish(TOPIC, GetJsonStr());
		//MQTT_PUBLISH::mqtt_publish();
		//MQTT_SUBSCRIBE::mqtt_subscribe();
	}
}