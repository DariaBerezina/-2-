#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/codegen.hpp"

class SwaggerComponent {
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
		oatpp::swagger::DocumentInfo::Builder builder;
		builder.setTitle("Todo Application");
		builder.setDescription("My first C++ Backend application");
		builder.setVersion("0.0.1");
		builder.setContactName("Berezina Daria");
		builder.setContactEmail("ladydashaberezina@mail.ru");
		return builder.build();
		}());

	OATPP_CREATE_COMPONENT(std::shared_ptr < oatpp::swagger::Resources>, swaggerResources)([] {
		return oatpp::swagger::Resources::loadResources("D:\\Studying\\Домашние лабы (алгоритмизац)\\1\\BaseProject_19\\BaseProject\\Dependencies\\oatp-swagger\\include\\oatpp-1.3.0\\bin\\oatpp-swagger\\res");
		}());
};