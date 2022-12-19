#include "process.h"

/* Code make by zabbix | zabbix@ztrunks.space */
/* Please read the win32 docs if you dont understain the code */

/* if you need this function in c i got some code for you
https://gist.github.com/zabbix-byte/4d60cc0e10ed94c91fa7ead9a10e6a58 */

DWORD get_process_id(const wchar_t *name)
{
	int proc_id = 0;
	HANDLE snap_p = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snap_p != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof(proc_entry);

		if (Process32First(snap_p, &proc_entry))
		{
			do
			{
				if (!_wcsicmp(name, proc_entry.szExeFile))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(snap_p, &proc_entry));
		}
	}

	CloseHandle(snap_p);
	return proc_id;
}

uintptr_t get_module_base_address(DWORD proc_id, const wchar_t *module_name)
{
	uintptr_t base_module = 0;
	/* TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32 this line got the 64 bits and 32 bits modules */
	HANDLE snap_m = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);

	if (snap_m != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 mod_entry;
		mod_entry.dwSize = sizeof(mod_entry);

		if (Module32First(snap_m, &mod_entry))
		{
			do
			{
				if (!_wcsicmp(module_name, mod_entry.szModule))
				{
					base_module = (uintptr_t)mod_entry.modBaseAddr;
					break;
				}

			} while (Module32Next(snap_m, &mod_entry));
		}
	}

	CloseHandle(snap_m);
	return base_module;

}

/*  multi level pointer function | This are a extra function for the ESP is not required */
/* but in a regular project you need to useit */
uintptr_t f_dmmaddy(HANDLE process, uintptr_t ptr, vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(process, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}