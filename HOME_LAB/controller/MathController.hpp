#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class MathController : public oatpp::web::server::api::ApiController {
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

	ResultDto::Wrapper createDto(Float32 a, Float32 b, Float32 result) {
		ResultDto::Wrapper dto = ResultDto::createShared();
		dto->value = result;
		dto->message = "Success!";
		dto->arguments = ArgumentsDto::createShared();
		dto->arguments->a = a;
		dto->arguments->b = b;

		return dto;
	};
public:
	MathController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{ }

	// ниже добавляем методы

	// сложение
	ENDPOINT_INFO(sum) {
		info->tags = std::list<oatpp::String>{ "Math"};
		info->summary = "Sum of two numbers";
		info->description = "Complete description of sum";
		std::string header1 = "<h>Zagolovok glavniy</h1>";
		std::string header2 = "<h2>Glava 1</h2>";
		std::string header = "<b>Zhirniy text</b>";
		std::string block1 = "<div>Otdelniy block texta nomer odin</div>";
		std::string block2 = "<div>Otdelniy block texta nomer dva</div>";
		std::string header3 = "<h3>Glava 2</h3>";
		std::string spisok = "<ul><li>Element 1</li><li></ul>";
		std::string link = "<a href=\https://pseudocoser.ru/\>Ssilka</a>";
		info->description = header1 + block1 + block2 + header2 + spisok + link;

		info->queryParams["a"].description = "First param of sum";
	}
	ENDPOINT("GET", "/math/sum", sum, QUERY(Float32, a, "a"), QUERY(Float32, b, "b")) {
		return ResponseFactory::createResponse(Status::CODE_200, createDto(a, b, a + b), objectMapper);
	}

	// вычитние
	ENDPOINT_INFO(substruct) {
		info->tags = std::list<oatpp::String>{ "Math" };
	}
	ENDPOINT("GET", "/math/substract", substruct, QUERY(Float32, a, "a"), QUERY(Float32, b, "b")) {
		return ResponseFactory::createResponse(Status::CODE_200, createDto(a, b, a - b), objectMapper);
	}

	// умножение
	ENDPOINT_INFO(multiply) {
		info->tags = std::list<oatpp::String>{ "Math" };
	}
	ENDPOINT("GET", "/math/multiply", multiply, QUERY(Float32, a, "a"), QUERY(Float32, b, "b")) {
		return ResponseFactory::createResponse(Status::CODE_200, createDto(a, b, a * b), objectMapper);
	}

	// деление
	ENDPOINT_INFO(divide) {
		info->tags = std::list<oatpp::String>{ "Math" };
	}
	ENDPOINT("GET", "/math/divide", divide, QUERY(Float32, a, "a"), QUERY(Float32, b, "b")) {
		return ResponseFactory::createResponse(Status::CODE_200, createDto(a, b, a / b), objectMapper);
	}
};

#include OATPP_CODEGEN_END(ApiController)