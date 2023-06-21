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
			//получаем ObjectMapper
			OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
			return ResponseFactory::createResponse(Status::CODE_200, helloString);
		}

};
class SumHandler : public oatpp::web::server::HttpRequestHandler{
public:
	std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override	
	{
		// получаем  ObjectMapper
		OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
		// создаем ответ, указываем код и сообщение
		// код 200 означает, что все хорошо (при ошибках возвращаются другие коды)
		// return ResponseFactory::createResponse(Status::CODE_200, "0")

		/*auto aPtr = request->getQueryParameter("a").get();
		auto bPtr = request->getQueryParameter("b").get();*/

		//создаем пустой массив
		auto list = HelloDto::createShared();

		//наполняем массив 
		auto element1 = ElementDto::createShared();
		//инициализируем пустой массив
		list->elements = {};
		element1->id = 1;
		element1->name = "Mikhail";
		auto element2 = ElementDto::createShared();
		element2->id = 2;
		element2->name = "Katya";
		// используем push_back как при работе с веторами из std
		list->elements->push_back(element1);
		list->elements->push_back(element2);

		// можно получить параметры типа Query String
		auto aPtr = request->getQueryParameter("a").get();
		auto bPtr = request->getQueryParameter("b").get();


		// получаем название заголовка в виде строки (std::string)
		auto acceptLanguageHeader = request->getHeader("Accept-Language").get();
		// записываем его в консоль как Warning
		OATPP_LOGW("App", acceptLanguageHeader->c_str());

		if (aPtr == nullptr || bPtr == nullptr)
		{	// первый параметр - к чему относится сообщение ( как бы тема, к которой группируются сообщения)
			OATPP_LOGE("App", "Не все параметры указаны!");
			return ResponseFactory::createResponse(Status::CODE_400, "Не все параметры указаны!");
		}

		// преобразуем строки в float, чтобы дроби можно было складывать)
		float a = atof(aPtr->c_str());
		float b = atof(bPtr->c_str());

		//формируем тело ответа (DTO)
		auto dto = ResultDto::createShared();

		dto->value = a + b;
		dto->message = "Success!";
		dto->arguments = ArgumentsDto::createShared();
		dto->arguments->a = a;
		dto->arguments->b = b;
		// создаем ответ, но не возвращаем его пока
		auto response = ResponseFactory::createResponse(Status::CODE_200, dto, objectMapper);
		// добавляем заголовок
		response->putHeaderIfNotExists("SumReasult", std::to_string(a + b));

		OATPP_LOGD("App", "Результат выполнен успешно!");

		//а теперь возвращаем ответ уже с заголовком
		return response;

	}

};
// функция, в которой реализуем запуск сервера
void runServer()
{
	AppComponent components;

	//получаем роутер
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter);

	oatpp::web::server::api::Endpoints docEndpoints;
	docEndpoints.append(httpRouter->addController(std::make_shared<MathController>())->getEndpoints());
	docEndpoints.append(httpRouter->addController(std::make_shared<TodoController>())->getEndpoints());

	httpRouter->addController(oatpp::swagger::Controller::createShared(docEndpoints));
	/*httpRouter->route("GET", "/sum", std::make_shared<SumHandler>());
	httpRouter->route("GET", "/hello", std::make_shared<HelloHandler>());*/

	//получаем ConnectionHandler
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler);
	//получаем ConnectionProvider
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider);

	//создаем сервер
	oatpp::network::Server server(serverConnectionProvider, serverConnectionHandler);

	OATPP_LOGI("App", "Сервер запущен!");

	server.run();
}
int main() {
	setlocale(LC_CTYPE, "Rus");
	oatpp::base::Environment::init();
	// вызов функции для запуска сервера
	runServer();
	return 0;
}