#ifndef _GAME_APP_H_
#define _GAME_APP_H_

class GameApp
{
public:
	GameApp() {}
	virtual ~GameApp() {}

	void Run();
	virtual void InitApp() {}

	/**
	 * @brief ÿ֡���£�������Render֮ǰ
	 */
	void Tick(float DeltaSeconds) {}

	/**
	 * @brief ��Ⱦ����
	 */
	void Render(float DeltaSeconds) {}

	/// ����CallBack
	//////////////////////////////////////////////////////////////////////////
private:
	void OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext) {}
};

#endif