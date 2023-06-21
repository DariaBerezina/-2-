#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "SwaggerComponent.hpp"

#include "db/MyClient.hpp"
#include"oatpp-sqlite/orm.hpp"

class AppComponent
{
	public:
		SwaggerComponent swaggerComponent;
		OATPP_CREATE_COMPONENT(std::string, helloString)([]{
				return "Hello!";
			}());

		OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([]{
				return oatpp::web::server::HttpRouter::createShared();
			}());

		OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
			OATPP_COMPONENT(std::shared_ptr < oatpp::web::server::HttpRouter>, router);
			return oatpp::web::server::HttpConnectionHandler::createShared(router);
			}());

		OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
			return oatpp::network::tcp::server::ConnectionProvider::createShared({ "localhost", 8000, oatpp::network::Address::IP_4 });
			}());

		OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper)([] {
			return oatpp::parser::json::mapping::ObjectMapper::createShared();
			}());

		OATPP_CREATE_COMPONENT(std::shared_ptr<MyClient>, myDatabaseClient)([] {
			auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>("myDatabase.db");
			auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);
			return std::make_shared<MyClient>(executor);
			}());

};