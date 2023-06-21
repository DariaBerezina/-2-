#include <iostream>
#include <oatpp/network/Server.hpp>
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "AppComponent.hpp"
#include "dto/ResultDto.hpp"
#include "dto/HelloDto.hpp"
#include "controller/MathController.hpp"
#include "controller/TodoController.hpp"
#include "oatpp-swagger/Controller.hpp"

class HelloHandler : public oatpp::web::server::HttpRequestHandler
{
	public:
		std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
		{
			OATPP_COMPONENT(std::string, helloString);
			//�������� ObjectMapper
			OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
			return ResponseFactory::createResponse(Status::CODE_200, helloString);
		}

};
class SumHandler : public oatpp::web::server::HttpRequestHandler{
public:
	std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override	
	{
		// ��������  ObjectMapper
		OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
		// ������� �����, ��������� ��� � ���������
		// ��� 200 ��������, ��� ��� ������ (��� ������� ������������ ������ ����)
		// return ResponseFactory::createResponse(Status::CODE_200, "0")

		/*auto aPtr = request->getQueryParameter("a").get();
		auto bPtr = request->getQueryParameter("b").get();*/

		//������� ������ ������
		auto list = HelloDto::createShared();

		//��������� ������ 
		auto element1 = ElementDto::createShared();
		//�������������� ������ ������
		list->elements = {};
		element1->id = 1;
		element1->name = "Mikhail";
		auto element2 = ElementDto::createShared();
		element2->id = 2;
		element2->name = "Katya";
		// ���������� push_back ��� ��� ������ � �������� �� std
		list->elements->push_back(element1);
		list->elements->push_back(element2);

		// ����� �������� ��������� ���� Query String
		auto aPtr = request->getQueryParameter("a").get();
		auto bPtr = request->getQueryParameter("b").get();


		// �������� �������� ��������� � ���� ������ (std::string)
		auto acceptLanguageHeader = request->getHeader("Accept-Language").get();
		// ���������� ��� � ������� ��� Warning
		OATPP_LOGW("App", acceptLanguageHeader->c_str());

		if (aPtr == nullptr || bPtr == nullptr)
		{	// ������ �������� - � ���� ��������� ��������� ( ��� �� ����, � ������� ������������ ���������)
			OATPP_LOGE("App", "�� ��� ��������� �������!");
			return ResponseFactory::createResponse(Status::CODE_400, "�� ��� ��������� �������!");
		}

		// ����������� ������ � float, ����� ����� ����� ���� ����������)
		float a = atof(aPtr->c_str());
		float b = atof(bPtr->c_str());

		//��������� ���� ������ (DTO)
		auto dto = ResultDto::createShared();

		dto->value = a + b;
		dto->message = "Success!";
		dto->arguments = ArgumentsDto::createShared();
		dto->arguments->a = a;
		dto->arguments->b = b;
		// ������� �����, �� �� ���������� ��� ����
		auto response = ResponseFactory::createResponse(Status::CODE_200, dto, objectMapper);
		// ��������� ���������
		response->putHeaderIfNotExists("SumReasult", std::to_string(a + b));

		OATPP_LOGD("App", "��������� �������� �������!");

		//� ������ ���������� ����� ��� � ����������
		return response;

	}

};
// �������, � ������� ��������� ������ �������
void runServer()
{
	AppComponent components;

	//�������� ������
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter);

	oatpp::web::server::api::Endpoints docEndpoints;
	docEndpoints.append(httpRouter->addController(std::make_shared<MathController>())->getEndpoints());
	docEndpoints.append(httpRouter->addController(std::make_shared<TodoController>())->getEndpoints());

	httpRouter->addController(oatpp::swagger::Controller::createShared(docEndpoints));
	/*httpRouter->route("GET", "/sum", std::make_shared<SumHandler>());
	httpRouter->route("GET", "/hello", std::make_shared<HelloHandler>());*/

	//�������� ConnectionHandler
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler);
	//�������� ConnectionProvider
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider);

	//������� ������
	oatpp::network::Server server(serverConnectionProvider, serverConnectionHandler);

	OATPP_LOGI("App", "������ �������!");

	server.run();
}
int main() {
	setlocale(LC_CTYPE, "Rus");
	oatpp::base::Environment::init();
	// ����� ������� ��� ������� �������
	runServer();
	return 0;
}