#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/Types.hpp"
#include "../dto/TodoDto.hpp"
#include "../dto/UserDto.hpp"
#include "../dto/TodoDbDto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class TodoController : public oatpp::web::server::api::ApiController {
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

	OATPP_COMPONENT(std::shared_ptr<MyClient>, dbClient);

	int checkAuth(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject> authObject) {
		auto result = dbClient->getAllUsers();

		auto users = result->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();

		OATPP_LOGI("App", objectMapper->writeToString(users)->c_str());
		for (auto i = 0; i < users->size(); i++) {
			if (users[i]->username == authObject->userId &&
				users[i]-> password == authObject->password) {
				return users[i]->id;
			}
		}

		throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401, "Unauthorized", {});
	}
	TodoDbDto::Wrapper getTodo(int id, int userId) {
		auto result = dbClient->getOneTodoById(id);
		auto todos = result->fetch < oatpp::Vector<oatpp::Object<TodoDbDto>>>();

		if (todos->size() == 0) {
			throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_404, "Заметка не найдена", {});
		}

		auto todo = todos[0];

		if (todo->userId != userId) {
			throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_403, "Не твоя заметка", {});
		}
		return todos[0];

	}

public:
	TodoController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{
		setDefaultAuthorizationHandler(std::make_shared<oatpp::web::server::handler::BasicAuthorizationHandler>("auth-handler"));
	}

	ENDPOINT("GET", "/todo/about", todosAbout) {
		std::string header = "<h1 style=\"color: blue; background: gray\">Todo Controller</h1>";
		std::string list = "<ul><li>Create todos</li><li>Update todos</li><li>Delete todos</li></ul>";

		return ResponseFactory::createResponse(Status::CODE_200, header + list);
	}

	ENDPOINT_INFO(todosAll) {
		info->tags = std::list<oatpp::String>{ "Todo" };
		info->addSecurityRequirement("Authorization required");
	}
	ENDPOINT("GET", "/todo/all", todosAll,
		AUTHORIZATION(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {
		int userId = checkAuth(authObject);
		auto result = dbClient->getUserByUserId(userId);
		auto todos = result->fetch<oatpp::Vector<oatpp::Object<TodoDbDto>>>();
		return ResponseFactory::createResponse(Status::CODE_200, todos, objectMapper);
	}

	ENDPOINT_INFO(todosOne) {
		info->tags = std::list<oatpp::String>{ "Todo" };
		info->addSecurityRequirement("Authorization required");
	}
	ENDPOINT("GET", "/todo/{id}", todosOne, PATH(Int16, id),
		AUTHORIZATION(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {
		int userId = checkAuth(authObject);
		auto todo = getTodo(id, userId);
		return ResponseFactory::createResponse(Status::CODE_200, todo, objectMapper);
	}

	ENDPOINT_INFO(todosAdd) {
		info->tags = std::list<oatpp::String>{ "Todo" };
		info->addSecurityRequirement("Authorization required");
	}
	ENDPOINT("POST", "/todo", todosAdd, BODY_DTO(Object<TodoDto>, todoDto),
		AUTHORIZATION(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {
		int userId = checkAuth(authObject);
		dbClient->addTodo(todoDto);
		return ResponseFactory::createResponse(Status::CODE_201, todoDto, objectMapper);
	}
	ENDPOINT_INFO(todoUpdate) {
		info->tags = std::list<oatpp::String>{ "Todo" };
		info->pathParams["id"].description = "Description of ID";
		info->addSecurityRequirement("Authorization required");
	}
	ENDPOINT("PUT", "/todo/{id}", todoUpdate, BODY_DTO(Object<TodoDto>, todoDto), PATH(Int16, id),
		AUTHORIZATION(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {
		int userId = checkAuth(authObject);
		auto todo = getTodo(id, userId);

		dbClient->updateTodo(todoDto, id);

		return ResponseFactory::createResponse(Status::CODE_200, todoDto, objectMapper);
	}
	ENDPOINT_INFO(todosDelete) {
		info->tags = std::list<oatpp::String>{ "Todo" };
		info->addSecurityRequirement("Authorization required");
	}
	ENDPOINT("DELETE", "/todo/{id}", todosDelete, PATH(Int16, id),
		AUTHORIZATION(std::shared_ptr<oatpp::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {
		int userId = checkAuth(authObject);
		auto todo = getTodo(id, userId);

		dbClient->deleteTodo(id);


		return ResponseFactory::createResponse(Status::CODE_204, "");
	}

};

#include OATPP_CODEGEN_END(ApiController)