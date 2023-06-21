#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {
	DTO_INIT(UserDto, DTO);
	DTO_FIELD(Int16, id);
	DTO_FIELD(String, username);
	DTO_FIELD(String, password);
	DTO_FIELD(Int16, role);
};

#include OATPP_CODEGEN_END(DTO)