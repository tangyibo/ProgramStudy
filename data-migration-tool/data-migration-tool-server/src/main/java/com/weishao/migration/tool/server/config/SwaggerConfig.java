package com.weishao.migration.tool.server.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.DefaultServletHandlerConfigurer;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

@Configuration
@EnableSwagger2
public class SwaggerConfig implements WebMvcConfigurer {

	@Value("${swagger.show}")
	private boolean swaggerShow;

	@Bean
	public Docket createRestApi() {
		return new Docket(DocumentationType.SWAGGER_2).enable(this.swaggerShow)// 是否开启swagger
				.groupName("DBQuery").apiInfo(this.apiInfo()).select()
				.apis(RequestHandlerSelectors.basePackage("com.weishao.migration.tool.server.controller")) // 对该包下的api进行监控
				.paths(PathSelectors.any()) // 对该包下的所有路径进行监控
				.build();
	}

	private ApiInfo apiInfo() {
		return new ApiInfoBuilder().title("DBQuery服务api文档").description("DBQuery服务api文档")
				.termsOfServiceUrl("http://127.0.0.1:8088").version("1.0").build();
	}

	public void addResourceHandlers(ResourceHandlerRegistry registry) {
		registry.addResourceHandler("/**").addResourceLocations("classpath:/static/");
		registry.addResourceHandler("swagger-ui.html").addResourceLocations("classpath:/META-INF/resources/");
		registry.addResourceHandler("/webjars/**").addResourceLocations("classpath:/META-INF/resources/webjars/");
	}

	public void configureDefaultServletHandling(DefaultServletHandlerConfigurer configurer) {
		configurer.enable();
	}
}
