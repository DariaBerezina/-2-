#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "../dto/TodoDto.hpp"
#include "oatpp-sqlite/orm.hpp"


#include OATPP_CODEGEN_BEGIN(DbClient)
class MyClient : public oatpp::orm::DbClient {
public:
	MyClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
		: oatpp::orm::DbClient(executor)
	{
		oatpp::orm::SchemaMigration migrations(executor);
		migrations.addFile(1, "migrations/1.sql");
		migrations.addFile(2, "migrations/2.sql");
		migrations.addFile(3, "migrations/3.sql");

		migrations.migrate();
	}

	QUERY(getAllUsers,
		"SELECT * FROM users"
	)

	QUERY(getUserByUserId,
		"SELECT * FROM todos WHERE userId=:userId;",
		PARAM(oatpp::Int16, userId)
	)

	QUERY(getOneTodoById,
		"SELECT * FROM todos WHERE id=:id;",
		PARAM(oatpp::Int16, id)
	)
	QUERY(addTodo,
		"INSERT INTO todos"
		"(name, description, priority, userId) VALUES"
		"(:todoDto.name, :todoDto.description, :todoDto.priority,:todoDto.userId);",
		PARAM(oatpp::Object<TodoDto>, todoDto)
	)

	QUERY(updateTodo,
		"Update todos SET "
		"name = :todoDto.name, description = :todoDto.description, priority = :todoDto.priority "
		"WHERE id = :id;",
		PARAM(oatpp::Object<TodoDto>, todoDto),
		PARAM(oatpp::Int16, id)
	)
	QUERY(deleteTodo,
		"DELETE FROM todos "
		"WHERE id = :id;",
		PARAM(oatpp::Int16, id)
	)
};


#include OATPP_CODEGEN_END(DbClient)