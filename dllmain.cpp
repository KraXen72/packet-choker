#include <WinSock2.h>

LPVOID Utils_DetourFn64(PCHAR pSource, LPVOID lpDestination, DWORD dwLen)
{
	DWORD dwMinLen = 14;
	if (dwLen < dwMinLen) return NULL;

	BYTE stub[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	PCHAR pGateway = (PCHAR)VirtualAlloc(NULL, dwLen + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	DWORD dwOldProtect;
	VirtualProtect(pSource, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	memcpy(pGateway, pSource, dwLen);
	memcpy(pGateway + dwLen, stub, sizeof(stub));

	*(PCHAR*)(pGateway + dwLen + 0x6) = pSource + dwLen;
	*(LPVOID*)(stub + 0x6) = lpDestination;

	memcpy(pSource, stub, sizeof(stub));

	VirtualProtect(pSource, dwLen, dwOldProtect, &dwOldProtect);
	return pGateway;
}

PCHAR GetFunction(LPCSTR szFunction) {
	return (PCHAR)GetProcAddress(GetModuleHandleA("WS2_32.dll"), szFunction);
}

typedef int(__stdcall *fn_send)(
	SOCKET     s,
	const char *buf,
	int        len,
	int        flags
	);

typedef int(__stdcall *fn_sendto)(
	SOCKET         s,
	const char     *buf,
	int            len,
	int            flags,
	const sockaddr *to,
	int            tolen
	);

fn_send g_send;
fn_sendto g_sendto;

DWORD dwLagSwitch = NULL;

bool LagSwitch(void) {

	DWORD dwAttributes = GetFileAttributesA("D:\\t");

	return (dwAttributes != INVALID_FILE_ATTRIBUTES) && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

int WSAAPI hk_send(
	SOCKET     s,
	const char *buf,
	int        len,
	int        flags
	) {

	if (!LagSwitch())
		return g_send(s, buf, len, flags);

	dwLagSwitch++;

	if (dwLagSwitch > 4) {
		dwLagSwitch = 0;
		return g_send(s, buf, len, flags);
	}
	else
		return len;
}

int hk_sendto(
	SOCKET         s,
	const char     *buf,
	int            len,
	int            flags,
	const sockaddr *to,
	int            tolen
	) {

	if (!LagSwitch())
		return g_sendto(s, buf, len, flags, to, tolen);

	dwLagSwitch++;

	if (dwLagSwitch > 4) {
		dwLagSwitch = 0;
		return g_sendto(s, buf, len, flags, to, tolen);
	}
	else
		return len;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	g_send = (fn_send)Utils_DetourFn64(GetFunction("send"), hk_send, 15);
	g_sendto = (fn_sendto)Utils_DetourFn64(GetFunction("sendto"), hk_sendto, 15);
	
	return TRUE;
}
