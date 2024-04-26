#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <stdio.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

using Microsoft::WRL::ComPtr;

// 输出显卡相关信息

int main(int argc, char* argv[]) {

	ComPtr<IDXGIFactory3> dxgiFactory3;

	HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory3));
	if (S_OK != hr) {
		printf("create IDXGIFactory3 failed!\n");
		return EXIT_FAILURE;
	}

	ComPtr<IDXGIAdapter1> dxgiAdapter;
	for (UINT i = 0; dxgiFactory3->EnumAdapters1(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		printf("\n===================================================================================\n");
		printf("Adapter %d:\n", i);
		DXGI_ADAPTER_DESC1 adapterDesc1 = { 0 };
		hr = dxgiAdapter->GetDesc1(&adapterDesc1);
		if(S_OK != hr) {
			printf("IDXGIAdapter1::GetDesc1() failed!\n");
			continue;
		}
		wprintf(L"Description: %s\n", adapterDesc1.Description);
		ComPtr<ID3D12Device> d3d12Dev;
		hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3d12Dev));
		if (S_OK != hr) {
			printf("D3D12CreateDevice() failed!\n");
			continue;
		}
		printf("Device NodeCount: %d\n", d3d12Dev->GetNodeCount());
		ComPtr<IDXGIOutput> dxgiOutput;
		for (UINT j = 0; dxgiAdapter->EnumOutputs(j, &dxgiOutput) != DXGI_ERROR_NOT_FOUND; j++) {
			DXGI_OUTPUT_DESC outputDesc = { 0 };
			hr = dxgiOutput->GetDesc(&outputDesc);
			if (S_OK != hr) {
				continue;
			}
			wprintf(L" - Output %d: %s\n", j, outputDesc.DeviceName);
		}
	}

	return 0;
}