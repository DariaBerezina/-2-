#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

class TodoDbDto : public oatpp::DTO {
	DTO_INIT(TodoDbDto, DTO);
	DTO_FIELD(Int16, id);
	DTO_FIELD(String, name);
	DTO_FIELD(String, description);
	DTO_FIELD(Int16, priority);
	DTO_FIELD(Int16, userId);
};

#include OATPP_CODEGEN_END(DTO)