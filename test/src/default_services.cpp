#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "kangaru/kangaru.hpp"

TEST_CASE("kgr::service must be constructible", "[default_services]") {
	struct Service {};
	struct Definition : kgr::service<Service> {};
	
	using return_value = decltype(kgr::container{}.service<Definition>());
	
	REQUIRE(kgr::detail::is_service_valid<Definition>::value);
	REQUIRE((std::is_same<return_value, Service>::value));
	(void) kgr::container{}.service<Definition>();
}

TEST_CASE("kgr::service must be constructible with arguments", "[default_services]") {
	struct Service { Service(int, float) {} };
	struct Definition : kgr::service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition>::value));
	(void) kgr::container{}.service<Definition>(1, 1.f);
}

TEST_CASE("kgr::service must be constructible with arguments optionaly", "[default_services]") {
	struct Service { Service(int = 0, float = 0.f) {} };
	struct Definition : kgr::service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition, int>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition, int, float>::value));
	(void) kgr::container{}.service<Definition>();
	(void) kgr::container{}.service<Definition>(1);
	(void) kgr::container{}.service<Definition>(1, 1.f);
}

TEST_CASE("kgr::service must be constructible with dependencies", "[default_services]") {
	struct ServiceDep1 {};
	struct ServiceDep2 {};
	struct ServiceDep3 {};
	struct ServiceDep4 {};
	struct Service1 { Service1(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>) {} };
	struct Service2 { Service2(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int, float) {} };
	struct Service3 { Service3(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int = 1, float = 1.f) {} };
	struct DefinitionDep1 : kgr::service<ServiceDep1> {};
	struct DefinitionDep2 : kgr::single_service<ServiceDep2> {};
	struct DefinitionDep3 : kgr::shared_service<ServiceDep3> {};
	struct DefinitionDep4 : kgr::unique_service<ServiceDep4> {};
	struct Definition1 : kgr::service<Service1, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition2 : kgr::service<Service2, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition3 : kgr::service<Service3, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	
	REQUIRE(kgr::detail::is_service_valid<Definition1>::value);
	REQUIRE_FALSE(kgr::detail::is_service_valid<Definition2>::value);
	REQUIRE((kgr::detail::is_service_valid<Definition2, int, float>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3, int>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3, int, float>::value));
	
	(void) kgr::container{}.service<Definition1>();
	(void) kgr::container{}.service<Definition2>(1, 1.f);
	(void) kgr::container{}.service<Definition3>();
	(void) kgr::container{}.service<Definition3>(1);
	(void) kgr::container{}.service<Definition3>(1, 1.f);
}

TEST_CASE("kgr::single_service must be constructible", "[default_services]") {
	struct Service {};
	struct Definition : kgr::single_service<Service> {};
	
	using return_value = decltype(kgr::container{}.service<Definition>());
	
	REQUIRE(kgr::detail::is_service_valid<Definition>::value);
	REQUIRE((std::is_same<return_value, Service&>::value));
	(void) kgr::container{}.service<Definition>();
}

TEST_CASE("kgr::single_service must be emplaceable with arguments", "[default_services]") {
	struct Service { Service(int, float) {} };
	struct Definition : kgr::single_service<Service> {};
	
	REQUIRE((kgr::detail::is_construction_valid<Definition, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_construction_valid<Definition>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition>::value));
	
	(void) kgr::container{}.emplace<Definition>(1, 1.f);
}

TEST_CASE("kgr::single_service must be emplaceable with arguments optionaly", "[default_services]") {
	struct Service { Service(int = 0, float = 0.f) {} };
	struct Definition : kgr::single_service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition, int>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition, int, float>::value));
	
	(void) kgr::container{}.service<Definition>();
	(void) kgr::container{}.emplace<Definition>();
	(void) kgr::container{}.emplace<Definition>(1);
	(void) kgr::container{}.emplace<Definition>(1, 1.f);
}

TEST_CASE("kgr::single_service must be constructible with dependencies", "[default_services]") {
	struct ServiceDep1 {};
	struct ServiceDep2 {};
	struct ServiceDep3 {};
	struct ServiceDep4 {};
	struct Service1 { Service1(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>) {} };
	struct Service2 { Service2(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int, float) {} };
	struct Service3 { Service3(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int = 1, float = 1.f) {} };
	struct DefinitionDep1 : kgr::service<ServiceDep1> {};
	struct DefinitionDep2 : kgr::single_service<ServiceDep2> {};
	struct DefinitionDep3 : kgr::shared_service<ServiceDep3> {};
	struct DefinitionDep4 : kgr::unique_service<ServiceDep4> {};
	struct Definition1 : kgr::single_service<Service1, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition2 : kgr::single_service<Service2, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition3 : kgr::single_service<Service3, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	
	REQUIRE(kgr::detail::is_service_valid<Definition1>::value);
	REQUIRE(kgr::detail::is_construction_valid<Definition1>::value);
	REQUIRE_FALSE(kgr::detail::is_service_valid<Definition2>::value);
	REQUIRE((kgr::detail::is_construction_valid<Definition2, int, float>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3, int>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition3, int>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition3, int, float>::value));
	
	(void) kgr::container{}.service<Definition1>();
	(void) kgr::container{}.emplace<Definition1>();
	(void) kgr::container{}.emplace<Definition2>(1, 1.f);
	(void) kgr::container{}.service<Definition3>();
	(void) kgr::container{}.emplace<Definition3>();
	(void) kgr::container{}.emplace<Definition3>(1);
	(void) kgr::container{}.emplace<Definition3>(1, 1.f);
}

TEST_CASE("kgr::shared_service must be constructible", "[default_services]") {
	struct Service {};
	struct Definition : kgr::shared_service<Service> {};
	
	using return_value = decltype(kgr::container{}.service<Definition>());
	
	REQUIRE(kgr::detail::is_service_valid<Definition>::value);
	REQUIRE((std::is_same<return_value, std::shared_ptr<Service>>::value));
	(void) kgr::container{}.service<Definition>();
}

TEST_CASE("kgr::shared_service must be emplaceable with arguments", "[default_services]") {
	struct Service { Service(int, float) {} };
	struct Definition : kgr::shared_service<Service> {};
	
	REQUIRE((kgr::detail::is_construction_valid<Definition, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_construction_valid<Definition>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition>::value));
	
	(void) kgr::container{}.emplace<Definition>(1, 1.f);
}

TEST_CASE("kgr::shared_service must be emplaceable with arguments optionaly", "[default_services]") {
	struct Service { Service(int = 0, float = 0.f) {} };
	struct Definition : kgr::shared_service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition, int>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition, int, float>::value));
	
	(void) kgr::container{}.service<Definition>();
	(void) kgr::container{}.emplace<Definition>();
	(void) kgr::container{}.emplace<Definition>(1);
	(void) kgr::container{}.emplace<Definition>(1, 1.f);
}

TEST_CASE("kgr::shared_service must be constructible with dependencies", "[default_services]") {
	struct ServiceDep1 {};
	struct ServiceDep2 {};
	struct ServiceDep3 {};
	struct ServiceDep4 {};
	struct Service1 { Service1(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>) {} };
	struct Service2 { Service2(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int, float) {} };
	struct Service3 { Service3(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int = 1, float = 1.f) {} };
	struct DefinitionDep1 : kgr::service<ServiceDep1> {};
	struct DefinitionDep2 : kgr::single_service<ServiceDep2> {};
	struct DefinitionDep3 : kgr::shared_service<ServiceDep3> {};
	struct DefinitionDep4 : kgr::unique_service<ServiceDep4> {};
	struct Definition1 : kgr::shared_service<Service1, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition2 : kgr::shared_service<Service2, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition3 : kgr::shared_service<Service3, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	
	REQUIRE(kgr::detail::is_service_valid<Definition1>::value);
	REQUIRE(kgr::detail::is_construction_valid<Definition1>::value);
	REQUIRE_FALSE(kgr::detail::is_service_valid<Definition2>::value);
	REQUIRE((kgr::detail::is_construction_valid<Definition2, int, float>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3, int>::value));
	REQUIRE((kgr::detail::is_construction_valid<Definition3, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition3, int>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition3, int, float>::value));
	
	(void) kgr::container{}.service<Definition1>();
	(void) kgr::container{}.emplace<Definition1>();
	(void) kgr::container{}.emplace<Definition2>(1, 1.f);
	(void) kgr::container{}.service<Definition3>();
	(void) kgr::container{}.emplace<Definition3>();
	(void) kgr::container{}.emplace<Definition3>(1);
	(void) kgr::container{}.emplace<Definition3>(1, 1.f);
}

TEST_CASE("kgr::unique_service must be constructible", "[default_services]") {
	struct Service {};
	struct Definition : kgr::unique_service<Service> {};
	
	using return_value = decltype(kgr::container{}.service<Definition>());
	
	REQUIRE(kgr::detail::is_service_valid<Definition>::value);
	REQUIRE((std::is_same<return_value, std::unique_ptr<Service>>::value));
	(void) kgr::container{}.service<Definition>();
}

TEST_CASE("kgr::unique_service must be constructible with arguments", "[default_services]") {
	struct Service { Service(int, float) {} };
	struct Definition : kgr::unique_service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition, int, float>::value));
	REQUIRE_FALSE((kgr::detail::is_service_valid<Definition>::value));
	(void) kgr::container{}.service<Definition>(1, 1.f);
}

TEST_CASE("kgr::unique_service must be constructible with arguments optionaly", "[default_services]") {
	struct Service { Service(int = 0, float = 0.f) {} };
	struct Definition : kgr::unique_service<Service> {};
	
	REQUIRE((kgr::detail::is_service_valid<Definition>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition, int>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition, int, float>::value));
	(void) kgr::container{}.service<Definition>();
	(void) kgr::container{}.service<Definition>(1);
	(void) kgr::container{}.service<Definition>(1, 1.f);
}

TEST_CASE("kgr::unique_service must be constructible with dependencies", "[default_services]") {
	struct ServiceDep1 {};
	struct ServiceDep2 {};
	struct ServiceDep3 {};
	struct ServiceDep4 {};
	struct Service1 { Service1(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>) {} };
	struct Service2 { Service2(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int, float) {} };
	struct Service3 { Service3(ServiceDep1, ServiceDep2&, std::shared_ptr<ServiceDep3>, std::unique_ptr<ServiceDep4>, int = 1, float = 1.f) {} };
	struct DefinitionDep1 : kgr::service<ServiceDep1> {};
	struct DefinitionDep2 : kgr::single_service<ServiceDep2> {};
	struct DefinitionDep3 : kgr::shared_service<ServiceDep3> {};
	struct DefinitionDep4 : kgr::unique_service<ServiceDep4> {};
	struct Definition1 : kgr::unique_service<Service1, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition2 : kgr::unique_service<Service2, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	struct Definition3 : kgr::unique_service<Service3, kgr::dependency<DefinitionDep1, DefinitionDep2, DefinitionDep3, DefinitionDep4>> {};
	
	REQUIRE(kgr::detail::is_service_valid<Definition1>::value);
	REQUIRE_FALSE(kgr::detail::is_service_valid<Definition2>::value);
	REQUIRE((kgr::detail::is_service_valid<Definition2, int, float>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3, int>::value));
	REQUIRE((kgr::detail::is_service_valid<Definition3, int, float>::value));
	
	(void) kgr::container{}.service<Definition1>();
	(void) kgr::container{}.service<Definition2>(1, 1.f);
	(void) kgr::container{}.service<Definition3>();
	(void) kgr::container{}.service<Definition3>(1);
	(void) kgr::container{}.service<Definition3>(1, 1.f);
}
