#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO) 

class ElementDto : public oatpp::DTO {
	DTO_INIT(ElementDto, DTO);
	DTO_FIELD(Int16, id);
	DTO_FIELD(String, name);
};

class HelloDto : public oatpp::DTO{
	DTO_INIT(HelloDto, DTO);
	DTO_FIELD(List<Object<ElementDto>>, elements);
};

#include OATPP_CODEGEN_END(DTO)