#ifndef KGR_KANGARU_INCLUDE_KANGARU_PREDICATE_HPP
#define KGR_KANGARU_INCLUDE_KANGARU_PREDICATE_HPP

#include "type_id.hpp"

namespace kgr {

/*
 * This predicate always returns true.
 * 
 * This is the default predicate used by the container.
 */
struct All {
	constexpr inline bool operator()(type_id_t) const {
		return true;
	}
};

/*
 * This predicate returns true for all services except those specified.
 */
template<typename First, typename... Ts>
struct NoneOf {
	constexpr bool operator()(type_id_t id) const {
		return !compare<First, Ts...>(id);
	}
	
private:
	template<typename Compared, typename Second, typename... Rest>
	constexpr bool compare(type_id_t id) const {
		return id == type_id<Compared>() && compare<Second, Rest...>(id);
	}
	
	template<typename Compared>
	constexpr bool compare(type_id_t id) const {
		return id == type_id<Compared>();
	}
};

/*
 * Predicate that returns false for all services, except those passed as argument.
 */
template<typename First, typename... Ts>
struct AnyOf {
	constexpr bool operator()(type_id_t id) const {
		return compare<First, Ts...>(id);
	}
	
private:
	template<typename Compared, typename Second, typename... Rest>
	constexpr bool compare(type_id_t id) const {
		return id == type_id<Compared>() && compare<Second, Rest...>(id);
	}
	
	template<typename Compared>
	constexpr bool compare(type_id_t id) const {
		return id == type_id<Compared>();
	}
};

} // namespace kgr

#endif // KGR_KANGARU_INCLUDE_KANGARU_PREDICATE_HPP
