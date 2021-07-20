#pragma once
#include <Tchar.h>
#include <vector>

class SoundManager {
public:
	SoundManager();
	virtual ~SoundManager();

	enum class BGM{
		Title = 0,
		RoomGame,
		ActionGame,

		Max
	};

	enum class SECommon{

		Max
	};

	enum class SETitle {
		Select = 0,

		Max
	};

	enum class SERoomGame {
		Select = 0,
		OK,  //ぷぃ
		OK2,  //フォーン
		Cancel,  //メニューなど閉じるときの音
		Mistake,
		BarUp,
		BarDown,

		Max
	};

	enum class SEActionGame {
		BoxOpen = 0,
		Cancel,
		Kettei,
		Select,

		Max
	};

	bool Init();

	bool LoadSECommon();
	void DeleteSECommon();
	bool PlaySECommon(SECommon type, int delayFrame = 0);

	bool LoadSETitle();
	void DeleteSETitle();
	bool PlaySETitle(SETitle type, int delayFrame = 0);

	bool LoadSERoomGame();
	void DeleteSERoomGame();
	bool PlaySERoomGame(SERoomGame type, int delayFrame = 0);

	bool LoadSEActionGame();
	void DeleteSEActionGame();
	bool PlaySEActionGame(SEActionGame type, int delayFrame = 0);
	

	void Process();
	bool PlayBgm(BGM bgm);
	void DirectStopBgm();
	void FadeStopBgm(int fadeFrame);

private:

	struct DelayPlay{
		int delayFrame;
		int handle;
	};

	bool LoadSE(std::vector<const TCHAR*>& fileNameList, std::vector<int>& handleList);
	void DeleteSE(std::vector<int>& handleList);
	bool PlaySE(int handle, int delayFrame);

	std::vector<const TCHAR*> _vBgmFileName;
	std::vector<struct DelayPlay> _vDelayPlay;

	std::vector<int> _vSECommonHandle;
	std::vector<int> _vSETitleHandle;
	std::vector<int> _vSETitleMenuHandle;
	std::vector<int> _vSERoomGameHandle;
	std::vector<int> _vSEActionGameHandle;
	

	BGM _nowPlayBgm;

	int _bgmFadeCount;
	float _bgmFadeFrame;
};