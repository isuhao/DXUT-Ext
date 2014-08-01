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
	 * @brief 每帧更新，发生在Render之前
	 */
	void Tick(float DeltaSeconds) {}

	/**
	 * @brief 渲染更新
	 */
	void Render(float DeltaSeconds) {}

	/// 各种CallBack
	//////////////////////////////////////////////////////////////////////////
private:
	void OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext) {}
};

#endif