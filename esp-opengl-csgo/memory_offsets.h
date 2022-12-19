#include <iostream>

/*
thanks frk1 for: https://github.com/frk1/hazedumper/blob/master/csgo.hpp
*/
namespace offsets {
	/* addreses */
	uintptr_t addr_local_player = 0xDEA964;
	uintptr_t addr_entity_lit = 0x4DFFF14;
	uintptr_t addr_view_matrix = 0x4DF0D44;

	/* Net vars */
	uintptr_t net_health = 0x100;
	uintptr_t net_vec_origin = 0x138;
	uintptr_t net_team_num = 0xF4;
}