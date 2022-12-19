#include <iostream>
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>


/* Code make by zabbix | zabbix@ztrunks.space */

using namespace std;

DWORD get_process_id(const wchar_t* name);

uintptr_t get_module_base_address(DWORD p_id, const wchar_t* m_name);

/* This are a extra function for the ESP is not required */
uintptr_t f_dmmaddy(HANDLE process, uintptr_t ptr, vector<unsigned int>offsets);

/*
Custom namespace to handle the methods of the memory
*/
namespace Memory {
	template <typename T>

	constexpr T Read(HANDLE h_proc, const uintptr_t& addr) 
	{
		T value = {};
		::ReadProcessMemory(h_proc, reinterpret_cast<const void*>(addr), &value, sizeof(T), NULL);

		return value;
	}

	template <typename T>
	constexpr void Write(HANDLE h_proc, const uintptr_t& addr, const T& value)
	{
		T value = {};
		::WriteProcessMemory(h_proc, reinterpret_cast<const void*>(addr), &value, sizeof(T), NULL);
		return value;
	
	}


}